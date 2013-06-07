/* Copyright (c) 2008-2012, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/diagchar.h>
#include <linux/sched.h>
#ifdef CONFIG_DIAG_OVER_USB
#include <mach/usbdiag.h>
#endif
#include <asm/current.h>
#include "diagchar_hdlc.h"
#include "diagmem.h"
#include "diagchar.h"
#include "diagfwd.h"
#include "diagfwd_cntl.h"
#include "diag_dci.h"
#ifdef CONFIG_DIAG_SDIO_PIPE
#include "diagfwd_sdio.h"
#endif
#ifdef CONFIG_DIAG_BRIDGE_CODE
#include "diagfwd_hsic.h"
#include "diagfwd_smux.h"
#endif
#include <linux/timer.h>
#include "diag_debugfs.h"
#include "diag_masks.h"

MODULE_DESCRIPTION("Diag Char Driver");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0");

#define INIT	1
#define EXIT	-1
struct diagchar_dev *driver;
struct diagchar_priv {
	int pid;
};
/* The following variables can be specified by module options */
 /* for copy buffer */
static unsigned int itemsize = 4096; /*Size of item in the mempool */
static unsigned int poolsize = 10; /*Number of items in the mempool */
/* for hdlc buffer */
static unsigned int itemsize_hdlc = 8192; /*Size of item in the mempool */
static unsigned int poolsize_hdlc = 8;  /*Number of items in the mempool */
/* for write structure buffer */
static unsigned int itemsize_write_struct = 20; /*Size of item in the mempool */
static unsigned int poolsize_write_struct = 8; /* Num of items in the mempool */
/* This is the max number of user-space clients supported at initialization*/
static unsigned int max_clients = 15;
static unsigned int threshold_client_limit = 30;
/* This is the maximum number of pkt registrations supported at initialization*/
unsigned int diag_max_reg = 600;
unsigned int diag_threshold_reg = 750;

/* Timer variables */
static struct timer_list drain_timer;
static int timer_in_progress;
void *buf_hdlc;
module_param(itemsize, uint, 0);
module_param(poolsize, uint, 0);
module_param(max_clients, uint, 0);

/* delayed_rsp_id 0 represents no delay in the response. Any other number
    means that the diag packet has a delayed response. */
static uint16_t delayed_rsp_id = 1;
#define DIAGPKT_MAX_DELAYED_RSP 0xFFFF
/* This macro gets the next delayed respose id. Once it reaches
 DIAGPKT_MAX_DELAYED_RSP, it stays at DIAGPKT_MAX_DELAYED_RSP */

#define DIAGPKT_NEXT_DELAYED_RSP_ID(x) 				\
((x < DIAGPKT_MAX_DELAYED_RSP) ? x++ : DIAGPKT_MAX_DELAYED_RSP)

#define COPY_USER_SPACE_OR_EXIT(buf, data, length)		\
do {								\
	if ((count < ret+length) || (copy_to_user(buf,		\
			(void *)&data, length))) {		\
		ret = -EFAULT;					\
		goto exit;					\
	}							\
	ret += length;						\
} while (0)

static void drain_timer_func(unsigned long data)
{
	queue_work(driver->diag_wq , &(driver->diag_drain_work));
}

void diag_drain_work_fn(struct work_struct *work)
{
	int err = 0;
	timer_in_progress = 0;

	mutex_lock(&driver->diagchar_mutex);
	if (buf_hdlc) {
		err = diag_device_write(buf_hdlc, APPS_DATA, NULL);
		if (err) {
			/*Free the buffer right away if write failed */
			diagmem_free(driver, buf_hdlc, POOL_TYPE_HDLC);
			diagmem_free(driver, (unsigned char *)driver->
				 write_ptr_svc, POOL_TYPE_WRITE_STRUCT);
		}
		buf_hdlc = NULL;
#ifdef DIAG_DEBUG
		pr_debug("diag: Number of bytes written "
				 "from timer is %d ", driver->used);
#endif
		driver->used = 0;
	}
	mutex_unlock(&driver->diagchar_mutex);
}

#ifdef CONFIG_DIAG_BRIDGE_CODE
void diag_clear_hsic_tbl(void)
{
	int i;

	driver->num_hsic_buf_tbl_entries = 0;
	for (i = 0; i < driver->poolsize_hsic_write; i++) {
		if (driver->hsic_buf_tbl[i].buf) {
			/* Return the buffer to the pool */
			diagmem_free(driver, (unsigned char *)
				(driver->hsic_buf_tbl[i].buf),
				POOL_TYPE_HSIC);
			driver->hsic_buf_tbl[i].buf = 0;
		}
		driver->hsic_buf_tbl[i].length = 0;
	}
}
#else
void diag_clear_hsic_tbl(void) { }
#endif

void diag_read_smd_work_fn(struct work_struct *work)
{
	__diag_smd_send_req();
}

void diag_read_smd_lpass_work_fn(struct work_struct *work)
{
	__diag_smd_lpass_send_req();
}

void diag_read_smd_wcnss_work_fn(struct work_struct *work)
{
	__diag_smd_wcnss_send_req();
}

void diag_add_client(int i, struct file *file)
{
	struct diagchar_priv *diagpriv_data;

	driver->client_map[i].pid = current->tgid;
	diagpriv_data = kmalloc(sizeof(struct diagchar_priv),
							GFP_KERNEL);
	if (diagpriv_data)
		diagpriv_data->pid = current->tgid;
	file->private_data = diagpriv_data;
	strlcpy(driver->client_map[i].name, current->comm, 20);
	driver->client_map[i].name[19] = '\0';
}

static int diagchar_open(struct inode *inode, struct file *file)
{
	int i = 0;
	void *temp;

	if (driver) {
		mutex_lock(&driver->diagchar_mutex);

		for (i = 0; i < driver->num_clients; i++)
			if (driver->client_map[i].pid == 0)
				break;

		if (i < driver->num_clients) {
			diag_add_client(i, file);
		} else {
			if (i < threshold_client_limit) {
				driver->num_clients++;
				temp = krealloc(driver->client_map
					, (driver->num_clients) * sizeof(struct
						 diag_client_map), GFP_KERNEL);
				if (!temp)
					goto fail;
				else
					driver->client_map = temp;
				temp = krealloc(driver->data_ready
					, (driver->num_clients) * sizeof(int),
							GFP_KERNEL);
				if (!temp)
					goto fail;
				else
					driver->data_ready = temp;
				diag_add_client(i, file);
			} else {
				mutex_unlock(&driver->diagchar_mutex);
				pr_alert("Max client limit for DIAG reached\n");
				pr_info("Cannot open handle %s"
					   " %d", current->comm, current->tgid);
				for (i = 0; i < driver->num_clients; i++)
					pr_debug("%d) %s PID=%d", i, driver->
						client_map[i].name,
						driver->client_map[i].pid);
				return -ENOMEM;
			}
		}
		driver->data_ready[i] = 0x0;
		driver->data_ready[i] |= MSG_MASKS_TYPE;
		driver->data_ready[i] |= EVENT_MASKS_TYPE;
		driver->data_ready[i] |= LOG_MASKS_TYPE;

		if (driver->ref_count == 0)
			diagmem_init(driver);
		driver->ref_count++;
		mutex_unlock(&driver->diagchar_mutex);
		return 0;
	}
	return -ENOMEM;

fail:
	mutex_unlock(&driver->diagchar_mutex);
	driver->num_clients--;
	pr_alert("diag: Insufficient memory for new client");
	return -ENOMEM;
}

static int diagchar_close(struct inode *inode, struct file *file)
{
	int i = 0;
	struct diagchar_priv *diagpriv_data = file->private_data;

	pr_debug("diag: process exit %s\n", current->comm);
	if (!(file->private_data)) {
		pr_alert("diag: Invalid file pointer");
		return -ENOMEM;
	}
	/* clean up any DCI registrations, if this is a DCI client
	* This will specially help in case of ungraceful exit of any DCI client
	* This call will remove any pending registrations of such client
	*/
	for (i = 0; i < MAX_DCI_CLIENTS; i++) {
		if (driver->dci_client_tbl[i].client &&
			driver->dci_client_tbl[i].client->tgid ==
							 current->tgid) {
			diagchar_ioctl(NULL, DIAG_IOCTL_DCI_DEINIT, 0);
			break;
		}
	}
	/* If the exiting process is the socket process */
	if (driver->socket_process &&
		(driver->socket_process->tgid == current->tgid)) {
		driver->socket_process = NULL;
	}

#ifdef CONFIG_DIAG_OVER_USB
	/* If the SD logging process exits, change logging to USB mode */
	if (driver->logging_process_id == current->tgid) {
		driver->logging_mode = USB_MODE;
		diagfwd_connect();
#ifdef CONFIG_DIAG_BRIDGE_CODE
		diag_clear_hsic_tbl();
		diagfwd_cancel_hsic();
		diagfwd_connect_bridge(0);
#endif
	}
#endif /* DIAG over USB */
	/* Delete the pkt response table entry for the exiting process */
	for (i = 0; i < diag_max_reg; i++)
			if (driver->table[i].process_id == current->tgid)
					driver->table[i].process_id = 0;

	if (driver) {
		mutex_lock(&driver->diagchar_mutex);
		driver->ref_count--;
		/* On Client exit, try to destroy all 3 pools */
		diagmem_exit(driver, POOL_TYPE_COPY);
		diagmem_exit(driver, POOL_TYPE_HDLC);
		diagmem_exit(driver, POOL_TYPE_WRITE_STRUCT);
		for (i = 0; i < driver->num_clients; i++) {
			if (NULL != diagpriv_data && diagpriv_data->pid ==
				 driver->client_map[i].pid) {
				driver->client_map[i].pid = 0;
				kfree(diagpriv_data);
				diagpriv_data = NULL;
				break;
			}
		}
		mutex_unlock(&driver->diagchar_mutex);
		return 0;
	}
	return -ENOMEM;
}

int diag_find_polling_reg(int i)
{
	uint16_t subsys_id, cmd_code_lo, cmd_code_hi;

	subsys_id = driver->table[i].subsys_id;
	cmd_code_lo = driver->table[i].cmd_code_lo;
	cmd_code_hi = driver->table[i].cmd_code_hi;
	if (driver->table[i].cmd_code == 0x0C)
		return 1;
	else if (driver->table[i].cmd_code == 0xFF) {
		if (subsys_id == 0x04 && cmd_code_hi == 0x0E &&
			 cmd_code_lo == 0x0E)
			return 1;
		else if (subsys_id == 0x08 && cmd_code_hi == 0x02 &&
			 cmd_code_lo == 0x02)
			return 1;
		else if (subsys_id == 0x32 && cmd_code_hi == 0x03  &&
			 cmd_code_lo == 0x03)
			return 1;
	}
	return 0;
}

void diag_clear_reg(int proc_num)
{
	int i;

	mutex_lock(&driver->diagchar_mutex);
	/* reset polling flag */
	driver->polling_reg_flag = 0;
	for (i = 0; i < diag_max_reg; i++) {
		if (driver->table[i].client_id == proc_num) {
			driver->table[i].process_id = 0;
		}
	}
	/* re-scan the registration table */
	for (i = 0; i < diag_max_reg; i++) {
		if (diag_find_polling_reg(i) == 1) {
			driver->polling_reg_flag = 1;
			break;
		}
	}
	mutex_unlock(&driver->diagchar_mutex);
}

void diag_add_reg(int j, struct bindpkt_params *params,
				  int *success, unsigned int *count_entries)
{
	*success = 1;
	driver->table[j].cmd_code = params->cmd_code;
	driver->table[j].subsys_id = params->subsys_id;
	driver->table[j].cmd_code_lo = params->cmd_code_lo;
	driver->table[j].cmd_code_hi = params->cmd_code_hi;

	/* check if incoming reg is polling & polling is yet not registered */
	if (driver->polling_reg_flag == 0)
		if (diag_find_polling_reg(j) == 1)
			driver->polling_reg_flag = 1;
	if (params->proc_id == APPS_PROC) {
		driver->table[j].process_id = current->tgid;
		driver->table[j].client_id = APPS_PROC;
	} else {
		driver->table[j].process_id = NON_APPS_PROC;
		driver->table[j].client_id = params->client_id;
	}
	(*count_entries)++;
}

long diagchar_ioctl(struct file *filp,
			   unsigned int iocmd, unsigned long ioarg)
{
	int i, j, temp, success = -1, status;
	unsigned int count_entries = 0, interim_count = 0;
	void *temp_buf;
	uint16_t support_list = 0;
	struct diag_dci_client_tbl *dci_params;
	struct diag_dci_health_stats stats;

	if (iocmd == DIAG_IOCTL_COMMAND_REG) {
		struct bindpkt_params_per_process pkt_params;
		struct bindpkt_params *params;
		struct bindpkt_params *head_params;
		if (copy_from_user(&pkt_params, (void *)ioarg,
			   sizeof(struct bindpkt_params_per_process))) {
			return -EFAULT;
		}
		if ((UINT32_MAX/sizeof(struct bindpkt_params)) <
							 pkt_params.count) {
			pr_warning("diag: integer overflow while multiply\n");
			return -EFAULT;
		}
		params = kzalloc(pkt_params.count*sizeof(
			struct bindpkt_params), GFP_KERNEL);
		if (!params) {
			pr_err("diag: unable to alloc memory\n");
			return -ENOMEM;
		} else
			head_params = params;

		if (copy_from_user(params, pkt_params.params,
			   pkt_params.count*sizeof(struct bindpkt_params))) {
			kfree(head_params);
			return -EFAULT;
		}
		mutex_lock(&driver->diagchar_mutex);
		for (i = 0; i < diag_max_reg; i++) {
			if (driver->table[i].process_id == 0) {
				diag_add_reg(i, params, &success,
							 &count_entries);
				if (pkt_params.count > count_entries) {
					params++;
				} else {
					mutex_unlock(&driver->diagchar_mutex);
					kfree(head_params);
					return success;
				}
			}
		}
		if (i < diag_threshold_reg) {
			/* Increase table size by amount required */
			if (pkt_params.count >= count_entries) {
				interim_count = pkt_params.count -
							 count_entries;
			} else {
				pr_warning("diag: error in params count\n");
				kfree(head_params);
				mutex_unlock(&driver->diagchar_mutex);
				return -EFAULT;
			}
			if (UINT32_MAX - diag_max_reg >=
							interim_count) {
				diag_max_reg += interim_count;
			} else {
				pr_warning("diag: Integer overflow\n");
				kfree(head_params);
				mutex_unlock(&driver->diagchar_mutex);
				return -EFAULT;
			}
			/* Make sure size doesnt go beyond threshold */
			if (diag_max_reg > diag_threshold_reg) {
				diag_max_reg = diag_threshold_reg;
				pr_info("diag: best case memory allocation\n");
			}
			if (UINT32_MAX/sizeof(struct diag_master_table) <
								 diag_max_reg) {
				pr_warning("diag: integer overflow\n");
				kfree(head_params);
				mutex_unlock(&driver->diagchar_mutex);
				return -EFAULT;
			}
			temp_buf = krealloc(driver->table,
					 diag_max_reg*sizeof(struct
					 diag_master_table), GFP_KERNEL);
			if (!temp_buf) {
				pr_alert("diag: Insufficient memory for reg.\n");
				mutex_unlock(&driver->diagchar_mutex);

				if (pkt_params.count >= count_entries) {
					interim_count = pkt_params.count -
								 count_entries;
				} else {
					pr_warning("diag: params count error\n");
					mutex_unlock(&driver->diagchar_mutex);
					kfree(head_params);
					return -EFAULT;
				}
				if (diag_max_reg >= interim_count) {
					diag_max_reg -= interim_count;
				} else {
					pr_warning("diag: Integer underflow\n");
					mutex_unlock(&driver->diagchar_mutex);
					kfree(head_params);
					return -EFAULT;
				}
				kfree(head_params);
				return 0;
			} else {
				driver->table = temp_buf;
			}
			for (j = i; j < diag_max_reg; j++) {
				diag_add_reg(j, params, &success,
							 &count_entries);
				if (pkt_params.count > count_entries) {
					params++;
				} else {
					mutex_unlock(&driver->diagchar_mutex);
					kfree(head_params);
					return success;
				}
			}
			kfree(head_params);
			mutex_unlock(&driver->diagchar_mutex);
		} else {
			mutex_unlock(&driver->diagchar_mutex);
			kfree(head_params);
			pr_err("Max size reached, Pkt Registration failed for"
						" Process %d", current->tgid);
		}
		success = 0;
	} else if (iocmd == DIAG_IOCTL_GET_DELAYED_RSP_ID) {
		struct diagpkt_delay_params delay_params;
		uint16_t interim_rsp_id;
		int interim_size;
		if (copy_from_user(&delay_params, (void *)ioarg,
					   sizeof(struct diagpkt_delay_params)))
			return -EFAULT;
		if ((delay_params.rsp_ptr) &&
		 (delay_params.size == sizeof(delayed_rsp_id)) &&
				 (delay_params.num_bytes_ptr)) {
			interim_rsp_id = DIAGPKT_NEXT_DELAYED_RSP_ID(
							delayed_rsp_id);
			if (copy_to_user((void *)delay_params.rsp_ptr,
					 &interim_rsp_id, sizeof(uint16_t)))
				return -EFAULT;
			interim_size = sizeof(delayed_rsp_id);
			if (copy_to_user((void *)delay_params.num_bytes_ptr,
						 &interim_size, sizeof(int)))
				return -EFAULT;
			success = 0;
		}
	} else if (iocmd == DIAG_IOCTL_DCI_REG) {
		if (driver->dci_state == DIAG_DCI_NO_REG)
			return DIAG_DCI_NO_REG;
		if (driver->num_dci_client >= MAX_DCI_CLIENTS)
			return DIAG_DCI_NO_REG;
		dci_params = kzalloc(sizeof(struct diag_dci_client_tbl),
								 GFP_KERNEL);
		if (dci_params == NULL) {
			pr_err("diag: unable to alloc memory\n");
			return -ENOMEM;
		}
		if (copy_from_user(dci_params, (void *)ioarg,
				 sizeof(struct diag_dci_client_tbl)))
			return -EFAULT;
		mutex_lock(&driver->dci_mutex);
		if (!(driver->num_dci_client))
			driver->in_busy_dci = 0;
		driver->num_dci_client++;
		pr_debug("diag: id = %d\n", driver->dci_client_id);
		driver->dci_client_id++;
		for (i = 0; i < MAX_DCI_CLIENTS; i++) {
			if (driver->dci_client_tbl[i].client == NULL) {
				driver->dci_client_tbl[i].client = current;
				driver->dci_client_tbl[i].list =
							 dci_params->list;
				driver->dci_client_tbl[i].signal_type =
					 dci_params->signal_type;
				create_dci_log_mask_tbl(driver->
					dci_client_tbl[i].dci_log_mask);
				create_dci_event_mask_tbl(driver->
					dci_client_tbl[i].dci_event_mask);
				driver->dci_client_tbl[i].data_len = 0;
				driver->dci_client_tbl[i].dci_data =
					 kzalloc(IN_BUF_SIZE, GFP_KERNEL);
				driver->dci_client_tbl[i].total_capacity =
								 IN_BUF_SIZE;
				driver->dci_client_tbl[i].dropped_logs = 0;
				driver->dci_client_tbl[i].dropped_events = 0;
				driver->dci_client_tbl[i].received_logs = 0;
				driver->dci_client_tbl[i].received_events = 0;
				break;
			}
		}
		mutex_unlock(&driver->dci_mutex);
		kfree(dci_params);
		return driver->dci_client_id;
	} else if (iocmd == DIAG_IOCTL_DCI_DEINIT) {
		success = -1;
		/* Delete this process from DCI table */
		mutex_lock(&driver->dci_mutex);
		for (i = 0; i < dci_max_reg; i++)
			if (driver->req_tracking_tbl[i].pid == current->tgid)
				driver->req_tracking_tbl[i].pid = 0;
		for (i = 0; i < MAX_DCI_CLIENTS; i++) {
			if (driver->dci_client_tbl[i].client &&
			driver->dci_client_tbl[i].client->tgid ==
							 current->tgid) {
				driver->dci_client_tbl[i].client = NULL;
				success = i;
				break;
			}
		}
		if (success >= 0)
			driver->num_dci_client--;
		mutex_unlock(&driver->dci_mutex);
		return success;
	} else if (iocmd == DIAG_IOCTL_DCI_SUPPORT) {
		if (driver->ch_dci)
			support_list = support_list | DIAG_CON_MPSS;
		if (copy_to_user((void *)ioarg, &support_list,
							 sizeof(uint16_t)))
			return -EFAULT;
		return DIAG_DCI_NO_ERROR;
	} else if (iocmd == DIAG_IOCTL_DCI_HEALTH_STATS) {
		if (copy_from_user(&stats, (void *)ioarg,
				 sizeof(struct diag_dci_health_stats)))
			return -EFAULT;
		for (i = 0; i < MAX_DCI_CLIENTS; i++) {
			dci_params = &(driver->dci_client_tbl[i]);
			if (dci_params->client &&
				dci_params->client->tgid == current->tgid) {
				stats.dropped_logs = dci_params->dropped_logs;
				stats.dropped_events =
						 dci_params->dropped_events;
				stats.received_logs = dci_params->received_logs;
				stats.received_events =
						 dci_params->received_events;
				if (stats.reset_status) {
					dci_params->dropped_logs = 0;
					dci_params->dropped_events = 0;
					dci_params->received_logs = 0;
					dci_params->received_events = 0;
				}
				break;
			}
		}
		if (copy_to_user((void *)ioarg, &stats,
				   sizeof(struct diag_dci_health_stats)))
			return -EFAULT;
		return DIAG_DCI_NO_ERROR;
	} else if (iocmd == DIAG_IOCTL_LSM_DEINIT) {
		for (i = 0; i < driver->num_clients; i++)
			if (driver->client_map[i].pid == current->tgid)
				break;
		if (i == driver->num_clients)
			return -EINVAL;
		driver->data_ready[i] |= DEINIT_TYPE;
		wake_up_interruptible(&driver->wait_q);
		success = 1;
	} else if (iocmd == DIAG_IOCTL_SWITCH_LOGGING) {
		mutex_lock(&driver->diagchar_mutex);
		temp = driver->logging_mode;
		driver->logging_mode = (int)ioarg;
		if (driver->logging_mode == MEMORY_DEVICE_MODE) {
			diag_clear_hsic_tbl();
			driver->mask_check = 1;
			if (driver->socket_process) {
				/*
				 * Notify the socket logging process that we
				 * are switching to MEMORY_DEVICE_MODE
				 */
				status = send_sig(SIGCONT,
					 driver->socket_process, 0);
				if (status) {
					pr_err("diag: %s, Error notifying ",
						__func__);
					pr_err("socket process, status: %d\n",
						status);
				}
			}
		}
		if (driver->logging_mode == UART_MODE) {
			diag_clear_hsic_tbl();
			driver->mask_check = 0;
			driver->logging_mode = MEMORY_DEVICE_MODE;
			driver->sub_logging_mode = UART_MODE;
		} else
			driver->sub_logging_mode = NO_LOGGING_MODE;
		if (driver->logging_mode == SOCKET_MODE) {
			diag_clear_hsic_tbl();
			driver->socket_process = current;
			driver->mask_check = 0;
			driver->logging_mode = MEMORY_DEVICE_MODE;
		}
		driver->logging_process_id = current->tgid;
		mutex_unlock(&driver->diagchar_mutex);
		if (temp == MEMORY_DEVICE_MODE && driver->logging_mode
							== NO_LOGGING_MODE) {
			driver->in_busy_1 = 1;
			driver->in_busy_2 = 1;
			driver->in_busy_lpass_1 = 1;
			driver->in_busy_lpass_2 = 1;
			driver->in_busy_wcnss_1 = 1;
			driver->in_busy_wcnss_2 = 1;
#ifdef CONFIG_DIAG_SDIO_PIPE
			driver->in_busy_sdio = 1;
#endif
#ifdef CONFIG_DIAG_BRIDGE_CODE
			diagfwd_disconnect_bridge(0);
			diag_clear_hsic_tbl();
#endif
		} else if (temp == NO_LOGGING_MODE && driver->logging_mode
							== MEMORY_DEVICE_MODE) {
			driver->in_busy_1 = 0;
			driver->in_busy_2 = 0;
			driver->in_busy_lpass_1 = 0;
			driver->in_busy_lpass_2 = 0;
			driver->in_busy_wcnss_1 = 0;
			driver->in_busy_wcnss_2 = 0;
			/* Poll SMD channels to check for data*/
			if (driver->ch)
				queue_work(driver->diag_wq,
					&(driver->diag_read_smd_work));
			if (driver->chlpass)
				queue_work(driver->diag_wq,
					&(driver->diag_read_smd_lpass_work));
			if (driver->ch_wcnss)
				queue_work(driver->diag_wq,
					&(driver->diag_read_smd_wcnss_work));
#ifdef CONFIG_DIAG_SDIO_PIPE
			driver->in_busy_sdio = 0;
			/* Poll SDIO channel to check for data */
			if (driver->sdio_ch)
				queue_work(driver->diag_sdio_wq,
					&(driver->diag_read_sdio_work));
#endif
#ifdef CONFIG_DIAG_BRIDGE_CODE
			diagfwd_connect_bridge(0);
#endif
		}
#ifdef CONFIG_DIAG_OVER_USB
		else if (temp == USB_MODE && driver->logging_mode
							 == NO_LOGGING_MODE) {
			diagfwd_disconnect();
#ifdef CONFIG_DIAG_BRIDGE_CODE
			diagfwd_disconnect_bridge(0);
#endif
		} else if (temp == NO_LOGGING_MODE && driver->logging_mode
								== USB_MODE) {
			diagfwd_connect();
#ifdef CONFIG_DIAG_BRIDGE_CODE
			diagfwd_connect_bridge(0);
#endif
		} else if (temp == USB_MODE && driver->logging_mode
							== MEMORY_DEVICE_MODE) {
			diagfwd_disconnect();
			driver->in_busy_1 = 0;
			driver->in_busy_2 = 0;
			driver->in_busy_lpass_1 = 0;
			driver->in_busy_lpass_2 = 0;
			driver->in_busy_wcnss_1 = 0;
			driver->in_busy_wcnss_2 = 0;

			/* Poll SMD channels to check for data*/
			if (driver->ch)
				queue_work(driver->diag_wq,
					 &(driver->diag_read_smd_work));
			if (driver->chlpass)
				queue_work(driver->diag_wq,
					&(driver->diag_read_smd_lpass_work));
			if (driver->ch_wcnss)
				queue_work(driver->diag_wq,
					&(driver->diag_read_smd_wcnss_work));
#ifdef CONFIG_DIAG_SDIO_PIPE
			driver->in_busy_sdio = 0;
			/* Poll SDIO channel to check for data */
			if (driver->sdio_ch)
				queue_work(driver->diag_sdio_wq,
					&(driver->diag_read_sdio_work));
#endif
#ifdef CONFIG_DIAG_BRIDGE_CODE
			diagfwd_cancel_hsic();
			diagfwd_connect_bridge(0);
#endif
		} else if (temp == MEMORY_DEVICE_MODE &&
				 driver->logging_mode == USB_MODE) {
			diagfwd_connect();
#ifdef CONFIG_DIAG_BRIDGE_CODE
			diag_clear_hsic_tbl();
			diagfwd_cancel_hsic();
			diagfwd_connect_bridge(0);
#endif
		}
#endif /* DIAG over USB */
		success = 1;
	}

	return success;
}

static int diagchar_read(struct file *file, char __user *buf, size_t count,
			  loff_t *ppos)
{
	struct diag_dci_client_tbl *entry;
	int index = -1, i = 0, ret = 0;
	int num_data = 0, data_type;
	for (i = 0; i < driver->num_clients; i++)
		if (driver->client_map[i].pid == current->tgid)
			index = i;

	if (index == -1) {
		pr_err("diag: Client PID not found in table");
		return -EINVAL;
	}

	wait_event_interruptible(driver->wait_q,
				  driver->data_ready[index]);
	mutex_lock(&driver->diagchar_mutex);

	if ((driver->data_ready[index] & USER_SPACE_LOG_TYPE) && (driver->
					logging_mode == MEMORY_DEVICE_MODE)) {
#ifdef CONFIG_DIAG_BRIDGE_CODE
		unsigned long spin_lock_flags;
		struct diag_write_device hsic_buf_tbl[NUM_HSIC_BUF_TBL_ENTRIES];
#endif

		pr_debug("diag: process woken up\n");
		/*Copy the type of data being passed*/
		data_type = driver->data_ready[index] & USER_SPACE_LOG_TYPE;
		COPY_USER_SPACE_OR_EXIT(buf, data_type, 4);
		/* place holder for number of data field */
		ret += 4;

		for (i = 0; i < driver->poolsize_write_struct; i++) {
			if (driver->buf_tbl[i].length > 0) {
#ifdef DIAG_DEBUG
				pr_debug("diag: WRITING the buf address "
				       "and length is %x , %d\n", (unsigned int)
					(driver->buf_tbl[i].buf),
					driver->buf_tbl[i].length);
#endif
				num_data++;
				/* Copy the length of data being passed */
				if (copy_to_user(buf+ret, (void *)&(driver->
						buf_tbl[i].length), 4)) {
						num_data--;
						goto drop;
				}
				ret += 4;

				/* Copy the actual data being passed */
				if (copy_to_user(buf+ret, (void *)driver->
				buf_tbl[i].buf, driver->buf_tbl[i].length)) {
					ret -= 4;
					num_data--;
					goto drop;
				}
				ret += driver->buf_tbl[i].length;
drop:
#ifdef DIAG_DEBUG
				pr_debug("diag: DEQUEUE buf address and"
				       " length is %x,%d\n", (unsigned int)
				       (driver->buf_tbl[i].buf), driver->
				       buf_tbl[i].length);
#endif
				diagmem_free(driver, (unsigned char *)
				(driver->buf_tbl[i].buf), POOL_TYPE_HDLC);
				driver->buf_tbl[i].length = 0;
				driver->buf_tbl[i].buf = 0;
			}
		}

		/* copy modem data */
		if (driver->in_busy_1 == 1) {
			num_data++;
			/*Copy the length of data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
					 (driver->write_ptr_1->length), 4);
			/*Copy the actual data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
					*(driver->buf_in_1),
					 driver->write_ptr_1->length);
			driver->in_busy_1 = 0;
		}
		if (driver->in_busy_2 == 1) {
			num_data++;
			/*Copy the length of data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
					 (driver->write_ptr_2->length), 4);
			/*Copy the actual data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
					 *(driver->buf_in_2),
					 driver->write_ptr_2->length);
			driver->in_busy_2 = 0;
		}
		/* copy lpass data */
		if (driver->in_busy_lpass_1 == 1) {
			num_data++;
			/*Copy the length of data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
				 (driver->write_ptr_lpass_1->length), 4);
			/*Copy the actual data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret, *(driver->
							buf_in_lpass_1),
					 driver->write_ptr_lpass_1->length);
			driver->in_busy_lpass_1 = 0;
		}
		if (driver->in_busy_lpass_2 == 1) {
			num_data++;
			/*Copy the length of data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
				 (driver->write_ptr_lpass_2->length), 4);
			/*Copy the actual data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret, *(driver->
				buf_in_lpass_2), driver->
					write_ptr_lpass_2->length);
			driver->in_busy_lpass_2 = 0;
		}
		/* copy wncss data */
		if (driver->in_busy_wcnss_1 == 1) {
			num_data++;
			/*Copy the length of data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
				 (driver->write_ptr_wcnss_1->length), 4);
			/*Copy the actual data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret, *(driver->
							buf_in_wcnss_1),
					 driver->write_ptr_wcnss_1->length);
			driver->in_busy_wcnss_1 = 0;
		}
		if (driver->in_busy_wcnss_2 == 1) {
			num_data++;
			/*Copy the length of data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
				 (driver->write_ptr_wcnss_2->length), 4);
			/*Copy the actual data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret, *(driver->
							buf_in_wcnss_2),
					 driver->write_ptr_wcnss_2->length);
			driver->in_busy_wcnss_2 = 0;
		}
#ifdef CONFIG_DIAG_SDIO_PIPE
		/* copy 9K data over SDIO */
		if (driver->in_busy_sdio == 1) {
			num_data++;
			/*Copy the length of data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
				 (driver->write_ptr_mdm->length), 4);
			/*Copy the actual data being passed*/
			COPY_USER_SPACE_OR_EXIT(buf+ret,
					*(driver->buf_in_sdio),
					 driver->write_ptr_mdm->length);
			driver->in_busy_sdio = 0;
		}
#endif
#ifdef CONFIG_DIAG_BRIDGE_CODE
		spin_lock_irqsave(&driver->hsic_spinlock, spin_lock_flags);
		for (i = 0; i < driver->poolsize_hsic_write; i++) {
			hsic_buf_tbl[i].buf = driver->hsic_buf_tbl[i].buf;
			driver->hsic_buf_tbl[i].buf = 0;
			hsic_buf_tbl[i].length =
					driver->hsic_buf_tbl[i].length;
			driver->hsic_buf_tbl[i].length = 0;
		}
		driver->num_hsic_buf_tbl_entries = 0;
		spin_unlock_irqrestore(&driver->hsic_spinlock,
					spin_lock_flags);

		for (i = 0; i < driver->poolsize_hsic_write; i++) {
			if (hsic_buf_tbl[i].length > 0) {
				pr_debug("diag: HSIC copy to user, i: %d, buf: %x, len: %d\n",
					 i, (unsigned int)hsic_buf_tbl[i].buf,
					hsic_buf_tbl[i].length);
				num_data++;
				/* Copy the length of data being passed */
				if (copy_to_user(buf+ret,
					(void *)&(hsic_buf_tbl[i].length),
					4)) {
					num_data--;
					goto drop_hsic;
				}
				ret += 4;

				/* Copy the actual data being passed */
				if (copy_to_user(buf+ret,
						(void *)hsic_buf_tbl[i].buf,
						hsic_buf_tbl[i].length)) {
					ret -= 4;
					num_data--;
					goto drop_hsic;
				}
				ret += hsic_buf_tbl[i].length;
drop_hsic:
				continue;

				#if 0	// Fixed a silent logging problem
				/* Return the buffer to the pool */
				diagmem_free(driver,
					(unsigned char *)(hsic_buf_tbl[i].buf),
					POOL_TYPE_HSIC);

				/* Call the write complete function */
				diagfwd_write_complete_hsic(NULL);
				#endif
			}
		}
#endif
		/* copy number of data fields */
		COPY_USER_SPACE_OR_EXIT(buf+4, num_data, 4);
		ret -= 4;
		driver->data_ready[index] ^= USER_SPACE_LOG_TYPE;

#if 1	// Fixed a silent logging problem
		for (i = 0; i < driver->poolsize_hsic_write; i++) {
			if (hsic_buf_tbl[i].length > 0) {
				diagmem_free(driver,
					(unsigned char *)(hsic_buf_tbl[i].buf),
					POOL_TYPE_HSIC);

				/* Call the write complete function */
				diagfwd_write_complete_hsic(NULL);	
			}
		}
#endif

		if (driver->ch)
			queue_work(driver->diag_wq,
					 &(driver->diag_read_smd_work));
		if (driver->chlpass)
			queue_work(driver->diag_wq,
					 &(driver->diag_read_smd_lpass_work));
		if (driver->ch_wcnss)
			queue_work(driver->diag_wq,
					 &(driver->diag_read_smd_wcnss_work));
#ifdef CONFIG_DIAG_SDIO_PIPE
		if (driver->sdio_ch)
			queue_work(driver->diag_sdio_wq,
					   &(driver->diag_read_sdio_work));
#endif
		APPEND_DEBUG('n');
		goto exit;
	} else if (driver->data_ready[index] & USER_SPACE_LOG_TYPE) {
		/* In case, the thread wakes up and the logging mode is
		not memory device any more, the condition needs to be cleared */
		driver->data_ready[index] ^= USER_SPACE_LOG_TYPE;
	}

	if (driver->data_ready[index] & DEINIT_TYPE) {
		/*Copy the type of data being passed*/
		data_type = driver->data_ready[index] & DEINIT_TYPE;
		COPY_USER_SPACE_OR_EXIT(buf, data_type, 4);
		driver->data_ready[index] ^= DEINIT_TYPE;
		goto exit;
	}

	if (driver->data_ready[index] & MSG_MASKS_TYPE) {
		/*Copy the type of data being passed*/
		data_type = driver->data_ready[index] & MSG_MASKS_TYPE;
		COPY_USER_SPACE_OR_EXIT(buf, data_type, 4);
		COPY_USER_SPACE_OR_EXIT(buf+4, *(driver->msg_masks),
							 MSG_MASK_SIZE);
		driver->data_ready[index] ^= MSG_MASKS_TYPE;
		goto exit;
	}

	if (driver->data_ready[index] & EVENT_MASKS_TYPE) {
		/*Copy the type of data being passed*/
		data_type = driver->data_ready[index] & EVENT_MASKS_TYPE;
		COPY_USER_SPACE_OR_EXIT(buf, data_type, 4);
		COPY_USER_SPACE_OR_EXIT(buf+4, *(driver->event_masks),
							 EVENT_MASK_SIZE);
		driver->data_ready[index] ^= EVENT_MASKS_TYPE;
		goto exit;
	}

	if (driver->data_ready[index] & LOG_MASKS_TYPE) {
		/*Copy the type of data being passed*/
		data_type = driver->data_ready[index] & LOG_MASKS_TYPE;
		COPY_USER_SPACE_OR_EXIT(buf, data_type, 4);
		COPY_USER_SPACE_OR_EXIT(buf+4, *(driver->log_masks),
							 LOG_MASK_SIZE);
		driver->data_ready[index] ^= LOG_MASKS_TYPE;
		goto exit;
	}

	if (driver->data_ready[index] & PKT_TYPE) {
		/*Copy the type of data being passed*/
		data_type = driver->data_ready[index] & PKT_TYPE;
		COPY_USER_SPACE_OR_EXIT(buf, data_type, 4);
		COPY_USER_SPACE_OR_EXIT(buf+4, *(driver->pkt_buf),
							 driver->pkt_length);
		driver->data_ready[index] ^= PKT_TYPE;
		goto exit;
	}

	if (driver->data_ready[index] & DCI_DATA_TYPE) {
		/* Copy the type of data being passed */
		data_type = driver->data_ready[index] & DCI_DATA_TYPE;
		COPY_USER_SPACE_OR_EXIT(buf, data_type, 4);
		/* check the current client and copy its data */
		for (i = 0; i < MAX_DCI_CLIENTS; i++) {
			if (driver->dci_client_tbl[i].client != NULL) {
				entry = &(driver->dci_client_tbl[i]);
				if (entry && (current->tgid ==
						entry->client->tgid)) {
					COPY_USER_SPACE_OR_EXIT(buf+4,
							entry->data_len, 4);
					COPY_USER_SPACE_OR_EXIT(buf+8,
					*(entry->dci_data), entry->data_len);
					entry->data_len = 0;
					break;
				}
			}
		}
		driver->data_ready[index] ^= DCI_DATA_TYPE;
		driver->in_busy_dci = 0;
		if (driver->ch_dci)
			queue_work(driver->diag_dci_wq,
				&(driver->diag_read_smd_dci_work));
		goto exit;
	}
exit:
	mutex_unlock(&driver->diagchar_mutex);
	return ret;
}

static int diagchar_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	int err, ret = 0, pkt_type;
#ifdef DIAG_DEBUG
	int length = 0, i;
#endif
	struct diag_send_desc_type send = { NULL, NULL, DIAG_STATE_START, 0 };
	struct diag_hdlc_dest_type enc = { NULL, NULL, 0 };
	void *buf_copy = NULL;
	unsigned int payload_size;
#ifdef CONFIG_DIAG_OVER_USB
	if (((driver->logging_mode == USB_MODE) && (!driver->usb_connected)) ||
				(driver->logging_mode == NO_LOGGING_MODE)) {
		/*Drop the diag payload */
		return -EIO;
	}
#endif /* DIAG over USB */
	/* Get the packet type F3/log/event/Pkt response */
	err = copy_from_user((&pkt_type), buf, 4);
	/* First 4 bytes indicate the type of payload - ignore these */
	if (count < 4) {
		pr_err("diag: Client sending short data\n");
		return -EBADMSG;
	}
	payload_size = count - 4;
	if (payload_size > USER_SPACE_DATA) {
		pr_err("diag: Dropping packet, packet payload size crosses 8KB limit. Current payload size %d\n",
				payload_size);
		driver->dropped_count++;
		return -EBADMSG;
	}
	if (pkt_type == DCI_DATA_TYPE) {
		err = copy_from_user(driver->user_space_data, buf + 4,
							 payload_size);
		if (err) {
			pr_alert("diag: copy failed for DCI data\n");
			return DIAG_DCI_SEND_DATA_FAIL;
		}
		err = diag_process_dci_transaction(driver->user_space_data,
							payload_size);
		return err;
	}
	if (pkt_type == USER_SPACE_LOG_TYPE) {
		err = copy_from_user(driver->user_space_data, buf + 4,
							 payload_size);
		/* Check masks for On-Device logging */
		if (driver->mask_check) {
			if (!mask_request_validate(driver->user_space_data)) {
				pr_alert("diag: mask request Invalid\n");
				return -EFAULT;
			}
		}
		buf = buf + 4;

		/* To removed "0x7E", when received only "0x7E" */
		if (0x7e == *(((unsigned char *)buf)))
			return 0;
#ifdef DIAG_DEBUG
		pr_debug("diag: user space data %d\n", payload_size);
		for (i = 0; i < payload_size; i++)
			pr_debug("\t %x", *((driver->user_space_data)+i));
#endif
#ifdef CONFIG_DIAG_SDIO_PIPE
		/* send masks to 9k too */
		if (driver->sdio_ch) {
			wait_event_interruptible(driver->wait_q,
				 (sdio_write_avail(driver->sdio_ch) >=
					 payload_size));
			if (driver->sdio_ch && (payload_size > 0)) {
				sdio_write(driver->sdio_ch, (void *)
				   (driver->user_space_data), payload_size);
			}
		}
#endif
#ifdef CONFIG_DIAG_BRIDGE_CODE
		/* send masks to 9k too */
		if (driver->hsic_ch && (payload_size > 0)) {
			/* wait sending mask updates if HSIC ch not ready */
			if (driver->in_busy_hsic_write)
				wait_event_interruptible(driver->wait_q,
					(driver->in_busy_hsic_write != 1));
			driver->in_busy_hsic_write = 1;
			driver->in_busy_hsic_read_on_device = 0;
			err = diag_bridge_write(driver->user_space_data,
							 payload_size);
			if (err) {
				pr_err("diag: err sending mask to MDM: %d\n",
									 err);
				/*
				* If the error is recoverable, then clear
				* the write flag, so we will resubmit a
				* write on the next frame.  Otherwise, don't
				* resubmit a write on the next frame.
				*/
				if ((-ESHUTDOWN) != err)
					driver->in_busy_hsic_write = 0;
			}
		}
#endif
#if 0
		/* send masks to 8k now */
		diag_process_hdlc((void *)(driver->user_space_data),
							 payload_size);
#endif
		return 0;
	}

	if (payload_size > itemsize) {
		pr_err("diag: Dropping packet, packet payload size crosses"
				"4KB limit. Current payload size %d\n",
				payload_size);
		driver->dropped_count++;
		return -EBADMSG;
	}

	buf_copy = diagmem_alloc(driver, payload_size, POOL_TYPE_COPY);
	if (!buf_copy) {
		driver->dropped_count++;
		return -ENOMEM;
	}

	err = copy_from_user(buf_copy, buf + 4, payload_size);
	if (err) {
		printk(KERN_INFO "diagchar : copy_from_user failed\n");
		ret = -EFAULT;
		goto fail_free_copy;
	}
#ifdef DIAG_DEBUG
	printk(KERN_DEBUG "data is -->\n");
	for (i = 0; i < payload_size; i++)
		printk(KERN_DEBUG "\t %x \t", *(((unsigned char *)buf_copy)+i));
#endif
	send.state = DIAG_STATE_START;
	send.pkt = buf_copy;
	send.last = (void *)(buf_copy + payload_size - 1);
	send.terminate = 1;
#ifdef DIAG_DEBUG
	pr_debug("diag: Already used bytes in buffer %d, and"
	" incoming payload size is %d\n", driver->used, payload_size);
	printk(KERN_DEBUG "hdlc encoded data is -->\n");
	for (i = 0; i < payload_size + 8; i++) {
		printk(KERN_DEBUG "\t %x \t", *(((unsigned char *)buf_hdlc)+i));
		if (*(((unsigned char *)buf_hdlc)+i) != 0x7e)
			length++;
	}
#endif
	mutex_lock(&driver->diagchar_mutex);
	if (!buf_hdlc)
		buf_hdlc = diagmem_alloc(driver, HDLC_OUT_BUF_SIZE,
						 POOL_TYPE_HDLC);
	if (!buf_hdlc) {
		ret = -ENOMEM;
		goto fail_free_hdlc;
	}
	if (HDLC_OUT_BUF_SIZE - driver->used <= (2*payload_size) + 3) {
		err = diag_device_write(buf_hdlc, APPS_DATA, NULL);
		if (err) {
			/*Free the buffer right away if write failed */
			diagmem_free(driver, buf_hdlc, POOL_TYPE_HDLC);
			diagmem_free(driver, (unsigned char *)driver->
				 write_ptr_svc, POOL_TYPE_WRITE_STRUCT);
			ret = -EIO;
			goto fail_free_hdlc;
		}
		buf_hdlc = NULL;
		driver->used = 0;
		buf_hdlc = diagmem_alloc(driver, HDLC_OUT_BUF_SIZE,
							 POOL_TYPE_HDLC);
		if (!buf_hdlc) {
			ret = -ENOMEM;
			goto fail_free_hdlc;
		}
	}

	enc.dest = buf_hdlc + driver->used;
	enc.dest_last = (void *)(buf_hdlc + driver->used + 2*payload_size + 3);
	diag_hdlc_encode(&send, &enc);

	/* This is to check if after HDLC encoding, we are still within the
	 limits of aggregation buffer. If not, we write out the current buffer
	and start aggregation in a newly allocated buffer */
	if ((unsigned int) enc.dest >=
		 (unsigned int)(buf_hdlc + HDLC_OUT_BUF_SIZE)) {
		err = diag_device_write(buf_hdlc, APPS_DATA, NULL);
		if (err) {
			/*Free the buffer right away if write failed */
			diagmem_free(driver, buf_hdlc, POOL_TYPE_HDLC);
			diagmem_free(driver, (unsigned char *)driver->
				 write_ptr_svc, POOL_TYPE_WRITE_STRUCT);
			ret = -EIO;
			goto fail_free_hdlc;
		}
		buf_hdlc = NULL;
		driver->used = 0;
		buf_hdlc = diagmem_alloc(driver, HDLC_OUT_BUF_SIZE,
							 POOL_TYPE_HDLC);
		if (!buf_hdlc) {
			ret = -ENOMEM;
			goto fail_free_hdlc;
		}
		enc.dest = buf_hdlc + driver->used;
		enc.dest_last = (void *)(buf_hdlc + driver->used +
							 (2*payload_size) + 3);
		diag_hdlc_encode(&send, &enc);
	}

	driver->used = (uint32_t) enc.dest - (uint32_t) buf_hdlc;
	if (pkt_type == DATA_TYPE_RESPONSE) {
		err = diag_device_write(buf_hdlc, APPS_DATA, NULL);
		if (err) {
			/*Free the buffer right away if write failed */
			diagmem_free(driver, buf_hdlc, POOL_TYPE_HDLC);
			diagmem_free(driver, (unsigned char *)driver->
				 write_ptr_svc, POOL_TYPE_WRITE_STRUCT);
			ret = -EIO;
			goto fail_free_hdlc;
		}
		buf_hdlc = NULL;
		driver->used = 0;
	}

	mutex_unlock(&driver->diagchar_mutex);
	diagmem_free(driver, buf_copy, POOL_TYPE_COPY);
	if (!timer_in_progress)	{
		timer_in_progress = 1;
		ret = mod_timer(&drain_timer, jiffies + msecs_to_jiffies(500));
	}
	return 0;

fail_free_hdlc:
	buf_hdlc = NULL;
	driver->used = 0;
	diagmem_free(driver, buf_copy, POOL_TYPE_COPY);
	mutex_unlock(&driver->diagchar_mutex);
	return ret;

fail_free_copy:
	diagmem_free(driver, buf_copy, POOL_TYPE_COPY);
	return ret;
}

int mask_request_validate(unsigned char mask_buf[])
{
	uint8_t packet_id;
	uint8_t subsys_id;
	uint16_t ss_cmd;

	packet_id = mask_buf[0];

	if (packet_id == 0x4B) {
		subsys_id = mask_buf[1];
		ss_cmd = *(uint16_t *)(mask_buf + 2);
		/* Packets with SSID which are allowed */
		switch (subsys_id) {
		case 0x04: /* DIAG_SUBSYS_WCDMA */
			if ((ss_cmd == 0) || (ss_cmd == 0xF))
				return 1;
			break;
		case 0x08: /* DIAG_SUBSYS_GSM */
			if ((ss_cmd == 0) || (ss_cmd == 0x1))
				return 1;
			break;
		case 0x09: /* DIAG_SUBSYS_UMTS */
		case 0x0F: /* DIAG_SUBSYS_CM */
			if (ss_cmd == 0)
				return 1;
			break;
		case 0x0C: /* DIAG_SUBSYS_OS */
			if ((ss_cmd == 2) || (ss_cmd == 0x100))
				return 1; /* MPU and APU */
			break;
		case 0x12: /* DIAG_SUBSYS_DIAG_SERV */
			if ((ss_cmd == 0) || (ss_cmd == 0x6) || (ss_cmd == 0x7))
				return 1;
			break;
		case 0x13: /* DIAG_SUBSYS_FS */
			if ((ss_cmd == 0) || (ss_cmd == 0x1))
				return 1;
			break;
		default:
			return 0;
			break;
		}
	} else {
		switch (packet_id) {
		case 0x00:    /* Version Number */
		case 0x0C:    /* CDMA status packet */
		case 0x1C:    /* Diag Version */
		case 0x1D:    /* Time Stamp */
		case 0x60:    /* Event Report Control */
		case 0x63:    /* Status snapshot */
		case 0x73:    /* Logging Configuration */
		case 0x7C:    /* Extended build ID */
		case 0x7D:    /* Extended Message configuration */
		case 0x81:    /* Event get mask */
		case 0x82:    /* Set the event mask */
			return 1;
			break;
		default:
			return 0;
			break;
		}
	}
	return 0;
}

static const struct file_operations diagcharfops = {
	.owner = THIS_MODULE,
	.read = diagchar_read,
	.write = diagchar_write,
	.unlocked_ioctl = diagchar_ioctl,
	.open = diagchar_open,
	.release = diagchar_close
};

static int diagchar_setup_cdev(dev_t devno)
{

	int err;

	cdev_init(driver->cdev, &diagcharfops);

	driver->cdev->owner = THIS_MODULE;
	driver->cdev->ops = &diagcharfops;

	err = cdev_add(driver->cdev, devno, 1);

	if (err) {
		printk(KERN_INFO "diagchar cdev registration failed !\n\n");
		return -1;
	}

	driver->diagchar_class = class_create(THIS_MODULE, "diag");

	if (IS_ERR(driver->diagchar_class)) {
		printk(KERN_ERR "Error creating diagchar class.\n");
		return -1;
	}

	device_create(driver->diagchar_class, NULL, devno,
				  (void *)driver, "diag");

	return 0;

}

static int diagchar_cleanup(void)
{
	if (driver) {
		if (driver->cdev) {
			/* TODO - Check if device exists before deleting */
			device_destroy(driver->diagchar_class,
				       MKDEV(driver->major,
					     driver->minor_start));
			cdev_del(driver->cdev);
		}
		if (!IS_ERR(driver->diagchar_class))
			class_destroy(driver->diagchar_class);
		kfree(driver);
	}
	return 0;
}

#ifdef CONFIG_DIAG_SDIO_PIPE
void diag_sdio_fn(int type)
{
	if (machine_is_msm8x60_fusion() || machine_is_msm8x60_fusn_ffa()) {
		if (type == INIT)
			diagfwd_sdio_init();
		else if (type == EXIT)
			diagfwd_sdio_exit();
	}
}
#else
inline void diag_sdio_fn(int type) {}
#endif

#ifdef CONFIG_DIAG_BRIDGE_CODE
void diag_bridge_fn(int type)
{
	if (type == INIT)
		diagfwd_bridge_init();
	else if (type == EXIT)
		diagfwd_bridge_exit();
}
#else
inline void diag_bridge_fn(int type) {}
#endif

static int __init diagchar_init(void)
{
	dev_t dev;
	int error;

	pr_debug("diagfwd initializing ..\n");
	driver = kzalloc(sizeof(struct diagchar_dev) + 5, GFP_KERNEL);

	if (driver) {
		driver->used = 0;
		timer_in_progress = 0;
		driver->debug_flag = 1;
		driver->dci_state = DIAG_DCI_NO_ERROR;
		setup_timer(&drain_timer, drain_timer_func, 1234);
		driver->itemsize = itemsize;
		driver->poolsize = poolsize;
		driver->itemsize_hdlc = itemsize_hdlc;
		driver->poolsize_hdlc = poolsize_hdlc;
		driver->itemsize_write_struct = itemsize_write_struct;
		driver->poolsize_write_struct = poolsize_write_struct;
		driver->num_clients = max_clients;
		driver->logging_mode = USB_MODE;
		driver->socket_process = NULL;
		driver->mask_check = 0;
		mutex_init(&driver->diagchar_mutex);
		init_waitqueue_head(&driver->wait_q);
		INIT_WORK(&(driver->diag_drain_work), diag_drain_work_fn);
		INIT_WORK(&(driver->diag_read_smd_work), diag_read_smd_work_fn);
		INIT_WORK(&(driver->diag_read_smd_cntl_work),
						 diag_read_smd_cntl_work_fn);
		INIT_WORK(&(driver->diag_read_smd_lpass_work),
			   diag_read_smd_lpass_work_fn);
		INIT_WORK(&(driver->diag_read_smd_lpass_cntl_work),
			   diag_read_smd_lpass_cntl_work_fn);
		INIT_WORK(&(driver->diag_read_smd_wcnss_work),
			diag_read_smd_wcnss_work_fn);
		INIT_WORK(&(driver->diag_read_smd_wcnss_cntl_work),
			diag_read_smd_wcnss_cntl_work_fn);
		INIT_WORK(&(driver->diag_read_smd_dci_work),
						diag_read_smd_dci_work_fn);
		INIT_WORK(&(driver->diag_update_smd_dci_work),
						diag_update_smd_dci_work_fn);
		INIT_WORK(&(driver->diag_clean_modem_reg_work),
						 diag_clean_modem_reg_fn);
		INIT_WORK(&(driver->diag_clean_lpass_reg_work),
						 diag_clean_lpass_reg_fn);
		INIT_WORK(&(driver->diag_clean_wcnss_reg_work),
						 diag_clean_wcnss_reg_fn);
		diag_debugfs_init();
		diagfwd_init();
		diagfwd_cntl_init();
		diag_masks_init();
		driver->dci_state = diag_dci_init();
		diag_sdio_fn(INIT);
		diag_bridge_fn(INIT);
		pr_debug("diagchar initializing ..\n");
		driver->num = 1;
		driver->name = ((void *)driver) + sizeof(struct diagchar_dev);
		strlcpy(driver->name, "diag", 4);

		/* Get major number from kernel and initialize */
		error = alloc_chrdev_region(&dev, driver->minor_start,
					    driver->num, driver->name);
		if (!error) {
			driver->major = MAJOR(dev);
			driver->minor_start = MINOR(dev);
		} else {
			printk(KERN_INFO "Major number not allocated\n");
			goto fail;
		}
		driver->cdev = cdev_alloc();
		error = diagchar_setup_cdev(dev);
		if (error)
			goto fail;
	} else {
		printk(KERN_INFO "kzalloc failed\n");
		goto fail;
	}

	pr_info("diagchar initialized now");
	return 0;

fail:
	diag_debugfs_cleanup();
	diagchar_cleanup();
	diagfwd_exit();
	diagfwd_cntl_exit();
	diag_masks_exit();
	diag_sdio_fn(EXIT);
	diag_bridge_fn(EXIT);
	return -1;
}

static void diagchar_exit(void)
{
	printk(KERN_INFO "diagchar exiting ..\n");
	/* On Driver exit, send special pool type to
	 ensure no memory leaks */
	diagmem_exit(driver, POOL_TYPE_ALL);
	diagfwd_exit();
	diagfwd_cntl_exit();
	diag_masks_exit();
	diag_sdio_fn(EXIT);
	diag_bridge_fn(EXIT);
	diag_debugfs_cleanup();
	diagchar_cleanup();
	printk(KERN_INFO "done diagchar exit\n");
}

module_init(diagchar_init);
module_exit(diagchar_exit);
