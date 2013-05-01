/*  
 *  checkpower.c - Registering early suspend handler and print when
 *  events(early suspend and late resume) happen.
 */
#include <linux/module.h>	    /* Needed by all modules */
#include <linux/kernel.h>	    /* Needed for KERN_INFO */
#include <linux/init.h>		    /* Needed for the macros */
#include <linux/earlysuspend.h> /* Needed for register_early_suspend */
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kobject.h>
#include <linux/device.h>

#define DRIVER_DESC   "A lkm to get notification of early suspend and late resume events"

#if 1
#define DBGPOW printk
#else
#define DBGPOW (char *fmt, ...)
#endif

#define LEGACY_TIMAD 0

#if LEGACY_TIMAD
#define PROCFS_MAX_SIZE 1024
static struct proc_dir_entry *timad_proc_file;
static char procfs_buffer[PROCFS_MAX_SIZE];
static unsigned long procfs_buffer_size = 0;
static pid_t pid = 0;

struct siginfo sinfo;
struct task_struct *task;
#endif

extern struct device *tima_uevent_dev;

static int power_state = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
static DEFINE_SPINLOCK(power_state_lock);


static void notify_service(void)
{
    char *envp[4];
    char *buf;

    buf = kmalloc(32,GFP_ATOMIC);  
    if(!buf){
	printk(KERN_ERR "%s kmalloc failed\n", __func__);
	return;
    }
    snprintf(buf , 32 , "POWER_STATE=%d",power_state);

    envp[0] = kstrdup("TIMA_STATUS=0", GFP_KERNEL); // TIMA_STATUS=0 if no malicious entity detected
    envp[1] = buf;
    envp[2] = kstrdup("TIMA_RESULT=normal power state change", GFP_KERNEL);
    envp[3] = NULL;

    kobject_uevent_env(&tima_uevent_dev->kobj, KOBJ_CHANGE, envp);
    kfree(envp[0]);
    kfree(envp[1]);
    kfree(envp[2]);
}

#if LEGACY_TIMAD
static void notify_timad()
{
    if (pid) {
	memset(&sinfo, 0, sizeof(struct siginfo));
	sinfo.si_signo = SIGUSR1;
	sinfo.si_code = SI_USER;
	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if(task == NULL) {
	    printk("Cannot find PID from user program\r\n");
	    return;
	}
	send_sig_info(SIGUSR1, &sinfo, task);
	printk("[checkpower] notify_timad about power state\n");
    } else {
	printk("does not know timad's pid, cannot send out signal\n");
    }   
}
#endif

static void early_suspend_handler(struct early_suspend *h)
{
    unsigned long irq_flags;
    DBGPOW(KERN_INFO "[checkpower] Device is going to be suspended. Early suspend level = %d\n", h->level);
    spin_lock_irqsave(&power_state_lock, irq_flags);
    power_state = h->level;
    spin_unlock_irqrestore(&power_state_lock, irq_flags);
#if LEGACY_TIMAD
    notify_timad();
#endif
    notify_service();    
}

static void late_resume_handler(struct early_suspend *h)
{
    unsigned long irq_flags;
    DBGPOW(KERN_INFO "[checkpower] Device is going to be re-activated. Early suspend level = %d\n", h->level);
    spin_lock_irqsave(&power_state_lock, irq_flags);
    power_state = h->level;
    spin_unlock_irqrestore(&power_state_lock, irq_flags);
#if LEGACY_TIMAD
    notify_timad();
#endif
    notify_service();
}

/* Three early suspend levels */
static struct early_suspend early_suspend_blank_screen_desc = 
{
    .level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN,
    .suspend = early_suspend_handler,
    .resume = late_resume_handler,
};

static struct early_suspend early_suspend_stop_drawing_desc = 
{
    .level = EARLY_SUSPEND_LEVEL_STOP_DRAWING,
    .suspend = early_suspend_handler,
    .resume = late_resume_handler,
};

static struct early_suspend early_suspend_disable_fb_desc = 
{
    .level = EARLY_SUSPEND_LEVEL_DISABLE_FB,
    .suspend = early_suspend_handler,
    .resume = late_resume_handler,
};

#if LEGACY_TIMAD
int procfile_read(char *buffer,
	      char **buffer_location,
	      off_t offset, int buffer_length, int *eof, void *data)
{
    int ret;
    
    if (offset > 0) {
	/* we have finished to read, return 0 */
	ret  = 0;
    } else {
	/* fill the buffer, return the buffer size */
	ret = sprintf(buffer, "%d", power_state);
    }

    return ret;
}


int procfile_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    /* get buffer size */
    procfs_buffer_size = count;
    if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
        procfs_buffer_size = PROCFS_MAX_SIZE;
    }
    
    /* write data to the buffer */
    if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
	pr_err("checkpower: copy_from_user failed\n");
        return -EFAULT;
    }
    
    kstrtoint(procfs_buffer, 10, &pid);
    //pr_err("checkpower: get pid %d from /proc/timad\n", pid);
    return procfs_buffer_size;
}
#endif

static int __init checkpower_init(void)
{
#if LEGACY_TIMAD
    timad_proc_file = create_proc_entry("timad", 0644, NULL);

    if (timad_proc_file == NULL) {
        remove_proc_entry("timad", NULL);
        printk(KERN_ALERT "Error: Could not initialize /proc/timad\n");
        return -ENOMEM;
    }

    timad_proc_file->read_proc = procfile_read;
    timad_proc_file->write_proc = procfile_write;
    timad_proc_file->uid = 0;
    timad_proc_file->gid = 0;
    printk("Registering /proc/timad Interface \n");
#endif
    register_early_suspend(&early_suspend_blank_screen_desc);
    register_early_suspend(&early_suspend_stop_drawing_desc);
    register_early_suspend(&early_suspend_disable_fb_desc);
    DBGPOW(KERN_INFO "[checkpower] Early suspend handler registered.\n");
    return 0;
}

static void __exit checkpower_exit(void)
{
#if LEGACY_TIMAD
    remove_proc_entry("timad", NULL);
    printk("Deregistering /proc/timad Interface\n");
#endif
    unregister_early_suspend(&early_suspend_blank_screen_desc);
    unregister_early_suspend(&early_suspend_stop_drawing_desc);
    unregister_early_suspend(&early_suspend_disable_fb_desc);
    DBGPOW(KERN_INFO "[checkpower] Early suspend handler unregistered.\n");
}

module_init(checkpower_init);
module_exit(checkpower_exit);

MODULE_LICENSE("GPL"); /* Need to confirm if leave it as GPL */
MODULE_DESCRIPTION(DRIVER_DESC);
