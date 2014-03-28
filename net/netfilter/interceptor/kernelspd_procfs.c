/**
   @copyright
   Copyright (c) 2013, INSIDE Secure Oy. All rights reserved.
*/

#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/module.h>

#include "kernelspd_internal.h"

#define KERNELSPD_PROCFS_COMMAND_BYTECOUNT_MAX 0x7fffffff

static struct proc_dir_entry *spd_control_file;
static int initialised = 0;
static int active = 0;
static int open = 0;
static char interfaces[1024] = "";

static int
spd_proc_open(
        struct inode *inode,
        struct file *file)
{
  if (open != 0)
    {
      DEBUG_FAIL(proc, "Kernel SPD device already open.");
      return -EFAULT;
    }

  if (!try_module_get(THIS_MODULE))
    {
      DEBUG_FAIL(proc, "Kernel module being removed.");
      return -EFAULT;
    }

  DEBUG_HIGH(proc, "Kernel SPD device opened.");
  open = 1;

  return 0;
}


int
process_command(
        struct KernelSpdCommand *cmd,
        const char __user *data,
        size_t data_len)

{
  int status = 0;

  DEBUG_LOW(proc, "Processing command id %d.", cmd->command_id);

  switch (cmd->command_id)
    {
    case KERNEL_SPD_ACTIVATE:
      {
        int ifnames_len = cmd->bytecount - sizeof cmd;

        if (ifnames_len >= sizeof interfaces - 1)
          {
            DEBUG_FAIL(def, "Interface name list too long.\n");
            return -EFAULT;
          }

        if (ifnames_len <= 0)
          {
            DEBUG_FAIL(def, "Interface name list needed.\n");
            return -EFAULT;
          }

        if (active != 0)
          {
            DEBUG_FAIL(def, "Kernel SPD Already active.\n");
            status = -EFAULT;
          }
        else
          {
            status = copy_from_user(interfaces, data, ifnames_len);
            if (status != 0)
              {
                DEBUG_FAIL(def, "Copy from user failed.\n");
                status = -EFAULT;
                break;
              }

            interfaces[ifnames_len] = '\0';
            status = cmd->bytecount;

            if (spd_hooks_init() != 0)
              {
                DEBUG_FAIL(proc, "Kernel SPD Failed activating NF Hooks.\n");
                status = -EFAULT;
              }

            active = 1;

            DEBUG_HIGH(
                    kernel,
                    "Kernel SPD activated. Unprotected interfaces: %s.\n",
                    interfaces);
          }
      }
      break;

    case KERNEL_SPD_DEACTIVATE:
      {
        if (active == 0)
          {
            DEBUG_FAIL(proc, "Kernel SPD not active.");
          }
        else
          {
            DEBUG_HIGH(proc, "Kernel SPD deactivated.");
            spd_hooks_uninit();
          }

        active = 0;
        status = cmd->bytecount;
      }
      break;

    case KERNEL_SPD_INSERT_ENTRY:
      {
        struct IPSelectorDbEntry *entry;
        const int payload_bytecount = cmd->bytecount - sizeof *cmd;

        if (!KERNEL_SPD_ID_VALID(cmd->spd_id))
        {
          DEBUG_FAIL(
                  proc,
                  "Invalid SPD id %d.\n",
                  cmd->spd_id);

          status = -EFAULT;
          break;
        }

        entry = vmalloc(sizeof *entry + payload_bytecount);
        if (entry == NULL)
          {
            DEBUG_FAIL(
                    proc,
                    "vmalloc(%d) failed.",
                    (int) (sizeof *entry + payload_bytecount));

            status = -EFAULT;
            break;
          }

        status = copy_from_user(entry + 1, data, payload_bytecount);
        if (status != 0)
          {
            DEBUG_FAIL(proc, "Copy from user failed.");

            vfree(entry);
            status = -EFAULT;
            break;
          }

        entry->action = cmd->action_id;
        entry->id = cmd->entry_id;
        entry->priority = cmd->priority;

        if (ip_selector_db_entry_check(
                    entry,
                    sizeof *entry + payload_bytecount)
            < 0)
          {
            DEBUG_FAIL(proc, "Selector check failed.");

            vfree(entry);
            status = -EFAULT;
            break;
          }

        DEBUG_DUMP(
                proc,
                debug_dump_ip_selector_group,
                entry + 1,
                payload_bytecount,
                "Insert entry %d to spd id %d action %d priority %d precedence %d:",
                entry->id,
                cmd->spd_id,
                entry->action,
                entry->priority,
                cmd->precedence);

        write_lock_bh(&spd_lock);
        ip_selector_db_entry_add(&spd, cmd->spd_id, entry, cmd->precedence);
        write_unlock_bh(&spd_lock);

        status = cmd->bytecount;
      }
      break;

    case KERNEL_SPD_REMOVE_ENTRY:

      if (!KERNEL_SPD_ID_VALID(cmd->spd_id))
        {
          DEBUG_FAIL(
                  proc,
                  "Invalid SPD id %d.",
                  cmd->spd_id);

          status = -EFAULT;
          break;
        }

        {
          struct IPSelectorDbEntry *removed;

          write_lock_bh(&spd_lock);
          removed =
              ip_selector_db_entry_remove(
                      &spd,
                      cmd->spd_id,
                      cmd->entry_id);

          write_unlock_bh(&spd_lock);

          if (removed != NULL)
            {
              DEBUG_DUMP(
                      proc,
                      debug_dump_ip_selector_group,
                      removed + 1,
                      -1,
                      "Removed entry %d to spd id %d action %d "
                      "priority %d:",
                      removed->id,
                      cmd->spd_id,
                      removed->action,
                      removed->priority);

              vfree(removed);
            }
          else
            {
              DEBUG_FAIL(
                      proc,
                      "Remove failed: Entry %d not found from spd id %d.",
                      cmd->entry_id,
                      cmd->spd_id);
            }
        }

      status = cmd->bytecount;
      break;

    default:
      DEBUG_FAIL(proc, "Unknown command id %d.", cmd->command_id);
      break;
    }

  DEBUG_LOW(proc, "Returning %d", status);

  return status;
}


static ssize_t
spd_proc_write(
        struct file *file,
        const char __user *data,
        size_t data_len,
        loff_t *pos)
{
  size_t bytes_read = 0;

  DEBUG_LOW(proc, "Write of %d bytes.", (int) data_len);
  while (bytes_read < data_len)
    {
      struct KernelSpdCommand command;
      int status;

      if (data_len < sizeof command)
        {
          DEBUG_FAIL(
                  proc,
                  "Data length %d less than sizeof command %d bytes.",
                  (int) data_len,
                  (int) sizeof command);

          bytes_read = -EFAULT;
          break;
        }

      status = copy_from_user(&command, data, sizeof command);
      if (status != 0)
        {
          DEBUG_FAIL(proc, "Copy from user failed.");
          bytes_read = -EFAULT;
          break;
        }

      if (command.bytecount < sizeof command)
        {
          DEBUG_FAIL(
                  proc,
                  "Command bytecount %d less than command size %d.",
                  (int) command.bytecount,
                  (int) sizeof command);

          bytes_read = -EINVAL;
          break;
        }

      if (command.bytecount > KERNELSPD_PROCFS_COMMAND_BYTECOUNT_MAX)
        {
          DEBUG_FAIL(
                  proc,
                  "Command bytecount %d bigger than max command size %d.",
                  (int) command.bytecount,
                  (int) KERNELSPD_PROCFS_COMMAND_BYTECOUNT_MAX);

          bytes_read = -EINVAL;
          break;
        }

      bytes_read += sizeof command;

      status = process_command(&command, data + bytes_read, data_len - bytes_read);
      if (status < 0)
        {
          bytes_read = -EFAULT;
          break;
        }

      bytes_read += status;
    }

  return bytes_read;
}


static void
spd_proc_cleanup_selectors(
        void)
{
  for (;;)
    {
      struct IPSelectorDbEntry *entry;

      write_lock_bh(&spd_lock);
      entry = ip_selector_db_entry_remove_next(&spd);
      write_unlock_bh(&spd_lock);

      if (entry != NULL)
        {
          vfree(entry);
        }
      else
        {
          break;
        }
    }
}


static int
spd_proc_release(
        struct inode *inode,
        struct file *file)
{
  DEBUG_HIGH(proc, "Kernel SPD device closed.");

  if (open != 0)
    {
      DEBUG_HIGH(proc, "Kernel SPD deactivated.");
      spd_hooks_uninit();
      active = 0;

      spd_proc_cleanup_selectors();
      module_put(THIS_MODULE);
      open = 0;
    }

  return 0;
}


static const struct file_operations spd_proc_fops =
{
  .open = spd_proc_open,
  .write = spd_proc_write,
  .release = spd_proc_release
};


int
spd_proc_init(
        void)
{
  spd_control_file =
      proc_create(
              LINUX_SPD_PROC_FILENAME,
              S_IFREG | S_IRUSR | S_IWUSR,
              NULL, &spd_proc_fops);

  if (spd_control_file == NULL)
    {
      DEBUG_FAIL(
              proc,
              "Failure creating proc entry %s.",
              LINUX_SPD_PROC_FILENAME);

      return -1;
    }

  unprotected_ifaces = interfaces;
  initialised = 1;

  DEBUG_MEDIUM(
          proc,
          "Created proc entry %s.",
          LINUX_SPD_PROC_FILENAME);

  return 0;
}


void
spd_proc_uninit(
        void)
{
  if (initialised != 0)
    {
      DEBUG_MEDIUM(
              proc,
              "Removing proc entry %s.",
              LINUX_SPD_PROC_FILENAME);

      DEBUG_HIGH(proc, "Kernel SPD deactivated.");
      spd_hooks_uninit();
      active = 0;

      spd_proc_cleanup_selectors();

      remove_proc_entry(LINUX_SPD_PROC_FILENAME, NULL);
      initialised = 0;
    }
  else
    {
      DEBUG_FAIL(proc, "Already uninitialised.");
    }
}
