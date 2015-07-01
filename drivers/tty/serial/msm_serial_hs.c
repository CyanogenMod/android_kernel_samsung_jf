/* drivers/serial/msm_serial_hs.c
 *
 * MSM 7k High speed uart driver
 *
 * Copyright (c) 2008 Google Inc.
 * Copyright (c) 2007-2013, The Linux Foundation. All rights reserved.
 * Modified: Nick Pelly <npelly@google.com>
 *
 * All source code in this file is licensed under the following license
 * except where indicated.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Has optional support for uart power management independent of linux
 * suspend/resume:
 *
 * RX wakeup.
 * UART wakeup can be triggered by RX activity (using a wakeup GPIO on the
 * UART RX pin). This should only be used if there is not a wakeup
 * GPIO on the UART CTS, and the first RX byte is known (for example, with the
 * Bluetooth Texas Instruments HCILL protocol), since the first RX byte will
 * always be lost. RTS will be asserted even while the UART is off in this mode
 * of operation. See msm_serial_hs_platform_data.rx_wakeup_irq.
 */

#include <linux/module.h>

#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/tty_flip.h>
#include <linux/wait.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/device.h>
#include <linux/wakelock.h>
#include <linux/debugfs.h>
#include <linux/gpio.h>
#include <asm/atomic.h>
#include <asm/irq.h>

#include <mach/hardware.h>
#include <mach/dma.h>
#include <mach/msm_serial_hs.h>

#include "msm_serial_hs_hwreg.h"

static int hs_serial_debug_mask = 1;
module_param_named(debug_mask, hs_serial_debug_mask,
		   int, S_IRUGO | S_IWUSR | S_IWGRP);

enum flush_reason {
	FLUSH_NONE,
	FLUSH_DATA_READY,
	FLUSH_DATA_INVALID,  /* values after this indicate invalid data */
	FLUSH_IGNORE = FLUSH_DATA_INVALID,
	FLUSH_STOP,
	FLUSH_SHUTDOWN,
};

enum msm_hs_clk_states_e {
	MSM_HS_CLK_PORT_OFF,     /* port not in use */
	MSM_HS_CLK_OFF,          /* clock disabled */
	MSM_HS_CLK_REQUEST_OFF,  /* disable after TX and RX flushed */
	MSM_HS_CLK_ON,           /* clock enabled */
};

/* Track the forced RXSTALE flush during clock off sequence.
 * These states are only valid during MSM_HS_CLK_REQUEST_OFF */
enum msm_hs_clk_req_off_state_e {
	CLK_REQ_OFF_START,
	CLK_REQ_OFF_RXSTALE_ISSUED,
	CLK_REQ_OFF_FLUSH_ISSUED,
	CLK_REQ_OFF_RXSTALE_FLUSHED,
};

struct msm_hs_tx {
	unsigned int tx_ready_int_en;  /* ok to dma more tx */
	unsigned int dma_in_flight;    /* tx dma in progress */
	enum flush_reason flush;
	wait_queue_head_t wait;
	struct msm_dmov_cmd xfer;
	dmov_box *command_ptr;
	u32 *command_ptr_ptr;
	dma_addr_t mapped_cmd_ptr;
	dma_addr_t mapped_cmd_ptr_ptr;
	int tx_count;
	dma_addr_t dma_base;
	struct tasklet_struct tlet;
};

struct msm_hs_rx {
	enum flush_reason flush;
	struct msm_dmov_cmd xfer;
	dma_addr_t cmdptr_dmaaddr;
	dmov_box *command_ptr;
	u32 *command_ptr_ptr;
	dma_addr_t mapped_cmd_ptr;
	wait_queue_head_t wait;
	dma_addr_t rbuffer;
	unsigned char *buffer;
	unsigned int buffer_pending;
	struct dma_pool *pool;
	struct wake_lock wake_lock;
	struct delayed_work flip_insert_work;
	struct tasklet_struct tlet;
	bool dma_in_flight;
	bool cmd_exec;
};

enum buffer_states {
	NONE_PENDING = 0x0,
	FIFO_OVERRUN = 0x1,
	PARITY_ERROR = 0x2,
	CHARS_NORMAL = 0x4,
};

/* optional low power wakeup, typically on a GPIO RX irq */
struct msm_hs_wakeup {
	int irq;  /* < 0 indicates low power wakeup disabled */
	unsigned char ignore;  /* bool */

	/* bool: inject char into rx tty on wakeup */
	unsigned char inject_rx;
	char rx_to_inject;
};

/*
 * UART can be used in 2-wire or 4-wire mode.
 * Use uart_func_mode to set 2-wire or 4-wire mode.
 */
enum uart_func_mode {
	UART_TWO_WIRE, /* can't support HW Flow control. */
	UART_FOUR_WIRE,/* can support HW Flow control. */
};

struct msm_hs_port {
	struct uart_port uport;
	unsigned int imr_reg;  /* shadow value of UARTDM_IMR */
	struct clk *clk;
	struct clk *pclk;
	struct msm_hs_tx tx;
	struct msm_hs_rx rx;
	/* gsbi uarts have to do additional writes to gsbi memory */
	/* block and top control status block. The following pointers */
	/* keep a handle to these blocks. */
	unsigned char __iomem	*mapped_gsbi;
	int dma_tx_channel;
	int dma_rx_channel;
	int dma_tx_crci;
	int dma_rx_crci;
	struct hrtimer clk_off_timer;  /* to poll TXEMT before clock off */
	ktime_t clk_off_delay;
	enum msm_hs_clk_states_e clk_state;
	enum msm_hs_clk_req_off_state_e clk_req_off_state;
	atomic_t clk_count;
	struct msm_hs_wakeup wakeup;
	struct wake_lock dma_wake_lock;  /* held while any DMA active */

	struct dentry *loopback_dir;
	struct work_struct clock_off_w; /* work for actual clock off */
	struct workqueue_struct *hsuart_wq; /* hsuart workqueue */
	struct mutex clk_mutex; /* mutex to guard against clock off/clock on */
	bool tty_flush_receive;
	bool rx_discard_flush_issued;
	enum uart_func_mode func_mode;
	bool is_shutdown;
	bool termios_in_progress;
	int rx_buf_size;
};

#define MSM_UARTDM_BURST_SIZE 16   /* DM burst size (in bytes) */
#define UARTDM_TX_BUF_SIZE UART_XMIT_SIZE
#define UARTDM_RX_BUF_SIZE 512
#define RETRY_TIMEOUT 5
#define UARTDM_NR 256
#define RX_FLUSH_COMPLETE_TIMEOUT 300 /* In jiffies */

static struct dentry *debug_base;
static struct msm_hs_port q_uart_port[UARTDM_NR];
static struct platform_driver msm_serial_hs_platform_driver;
static struct uart_driver msm_hs_driver;
static struct uart_ops msm_hs_ops;
static void msm_hs_start_rx_locked(struct uart_port *uport);

#define UARTDM_TO_MSM(uart_port) \
	container_of((uart_port), struct msm_hs_port, uport)

struct uart_port * msm_hs_get_port_by_id(int num)
{
	struct uart_port *uport;
	struct msm_hs_port *msm_uport;

	if (num < 0 || num >= UARTDM_NR)
		return NULL;

	msm_uport = &q_uart_port[num];

	uport = &(msm_uport->uport);

	return uport;
}

static int msm_hs_clock_vote(struct msm_hs_port *msm_uport)
{
	int ret = 0;
	if (atomic_inc_return(&msm_uport->clk_count) == 1) {
		ret = clk_prepare_enable(msm_uport->clk);
		if (ret) {
			dev_err(msm_uport->uport.dev,
			"%s: could not turn on clk %d \n", __func__, ret);
			return ret;
		}
		if (msm_uport->pclk) {
			ret = clk_prepare_enable(msm_uport->pclk);
			if (ret) {
				clk_disable_unprepare(msm_uport->clk);
				dev_err(msm_uport->uport.dev,
					"%s: could not turn on pclk %d \n",
								__func__, ret);
				return ret;
			}
		}

		msm_uport->clk_state = MSM_HS_CLK_ON;
	}
	return ret;
}

static void msm_hs_clock_unvote(struct msm_hs_port *msm_uport)
{
	int ret = 0;

	ret = atomic_read(&msm_uport->clk_count);
	if (ret <= 0) {
		WARN(ret, "msm_uport->clk_count < 0");
		dev_err(msm_uport->uport.dev,
			"%s: clock count invalid %d \n", __func__,
				atomic_read(&msm_uport->clk_count));
		return;
	}

	ret = atomic_dec_return(&msm_uport->clk_count);
	if (ret == 0) {
		clk_disable_unprepare(msm_uport->clk);
		if (msm_uport->pclk)
			clk_disable_unprepare(msm_uport->pclk);
		msm_uport->clk_state = MSM_HS_CLK_OFF;
	}
}

static ssize_t show_clock(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	int state = 1;
	enum msm_hs_clk_states_e clk_state;
	unsigned long flags;

	struct platform_device *pdev = container_of(dev, struct
						    platform_device, dev);
	struct msm_hs_port *msm_uport = &q_uart_port[pdev->id];

	spin_lock_irqsave(&msm_uport->uport.lock, flags);
	clk_state = msm_uport->clk_state;
	spin_unlock_irqrestore(&msm_uport->uport.lock, flags);

	if (clk_state <= MSM_HS_CLK_OFF)
		state = 0;

	return snprintf(buf, PAGE_SIZE, "%d\n", state);
}

static ssize_t set_clock(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	int state;
	struct platform_device *pdev = container_of(dev, struct
						    platform_device, dev);
	struct msm_hs_port *msm_uport = &q_uart_port[pdev->id];

	state = buf[0] - '0';
	switch (state) {
	case 0: {
		msm_hs_request_clock_off(&msm_uport->uport);
		break;
	}
	case 1: {
		msm_hs_request_clock_on(&msm_uport->uport);
		break;
	}
	default: {
		return -EINVAL;
	}
	}
	return count;
}

static DEVICE_ATTR(clock, S_IWUSR | S_IRUGO, show_clock, set_clock);

static inline unsigned int use_low_power_wakeup(struct msm_hs_port *msm_uport)
{
	return (msm_uport->wakeup.irq > 0);
}

static inline int is_gsbi_uart(struct msm_hs_port *msm_uport)
{
	/* assume gsbi uart if gsbi resource found in pdata */
	return ((msm_uport->mapped_gsbi != NULL));
}

static inline unsigned int msm_hs_read(struct uart_port *uport,
				       unsigned int offset)
{
	return readl_relaxed(uport->membase + offset);
}

static inline void msm_hs_write(struct uart_port *uport, unsigned int offset,
				 unsigned int value)
{
	writel_relaxed(value, uport->membase + offset);
}

/*
 * This API read and provides UART Core registers information.
 */
static void print_uart_registers(struct msm_hs_port *msm_uport)
{
	struct uart_port *uport = &(msm_uport->uport);

	pr_info("============= UART Registers ================\n");

	pr_info("UART_DM_MR1:%x\n", msm_hs_read(uport, UARTDM_MR1_ADDR));
	pr_info("UART_DM_MR2:%x\n", msm_hs_read(uport, UARTDM_MR2_ADDR));
	pr_info("UART_DM_IPR:%x\n", msm_hs_read(uport, UARTDM_IPR_ADDR));
	pr_info("UART_DM_TFWR:%x\n", msm_hs_read(uport, UARTDM_TFWR_ADDR));
	pr_info("UART_DM_RFWR:%x\n", msm_hs_read(uport, UARTDM_RFWR_ADDR));
	pr_info("UART_DM_DMEN:%x\n", msm_hs_read(uport, UARTDM_DMEN_ADDR));
	pr_info("UART_DM_SR:%x\n", msm_hs_read(uport, UARTDM_SR_ADDR));
	pr_info("UART_DM_ISR:%x\n", msm_hs_read(uport, UARTDM_ISR_ADDR));
	pr_info("UART_DM_TXFS:%x\n", msm_hs_read(uport, UARTDM_TXFS_ADDR));
	pr_info("UART_DM_RXFS:%x\n", msm_hs_read(uport, UARTDM_RXFS_ADDR));
	pr_info("UART_DM_IMR:%x\n", msm_uport->imr_reg);

	pr_info("=============================================\n");

}

static void msm_hs_release_port(struct uart_port *port)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(port);
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *gsbi_resource;
	resource_size_t size;

	if (is_gsbi_uart(msm_uport)) {
		iowrite32(GSBI_PROTOCOL_IDLE, msm_uport->mapped_gsbi +
			  GSBI_CONTROL_ADDR);
		gsbi_resource = platform_get_resource_byname(pdev,
							     IORESOURCE_MEM,
							     "gsbi_resource");
		if (unlikely(!gsbi_resource))
			return;

		size = resource_size(gsbi_resource);
		release_mem_region(gsbi_resource->start, size);
		iounmap(msm_uport->mapped_gsbi);
		msm_uport->mapped_gsbi = NULL;
	}
}

static int msm_hs_request_port(struct uart_port *port)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(port);
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *gsbi_resource;
	resource_size_t size;

	gsbi_resource = platform_get_resource_byname(pdev,
						     IORESOURCE_MEM,
						     "gsbi_resource");
	if (gsbi_resource) {
		size = resource_size(gsbi_resource);
		if (unlikely(!request_mem_region(gsbi_resource->start, size,
						 "msm_serial_hs")))
			return -EBUSY;
		msm_uport->mapped_gsbi = ioremap(gsbi_resource->start,
						 size);
		if (!msm_uport->mapped_gsbi) {
			release_mem_region(gsbi_resource->start, size);
			return -EBUSY;
		}
	}
	/* no gsbi uart */
	return 0;
}

static int msm_serial_loopback_enable_set(void *data, u64 val)
{
	struct msm_hs_port *msm_uport = data;
	struct uart_port *uport = &(msm_uport->uport);
	unsigned long flags;
	int ret = 0;

	msm_hs_clock_vote(msm_uport);

	if (val) {
		spin_lock_irqsave(&uport->lock, flags);
		ret = msm_hs_read(uport, UARTDM_MR2_ADDR);
		ret |= UARTDM_MR2_LOOP_MODE_BMSK;
		msm_hs_write(uport, UARTDM_MR2_ADDR, ret);
		spin_unlock_irqrestore(&uport->lock, flags);
	} else {
		spin_lock_irqsave(&uport->lock, flags);
		ret = msm_hs_read(uport, UARTDM_MR2_ADDR);
		ret &= ~UARTDM_MR2_LOOP_MODE_BMSK;
		msm_hs_write(uport, UARTDM_MR2_ADDR, ret);
		spin_unlock_irqrestore(&uport->lock, flags);
	}
	/* Calling CLOCK API. Hence mb() requires here. */
	mb();

	msm_hs_clock_unvote(msm_uport);

	return 0;
}

static int msm_serial_loopback_enable_get(void *data, u64 *val)
{
	struct msm_hs_port *msm_uport = data;
	struct uart_port *uport = &(msm_uport->uport);
	unsigned long flags;
	int ret = 0;

	msm_hs_clock_vote(msm_uport);

	spin_lock_irqsave(&uport->lock, flags);
	ret = msm_hs_read(&msm_uport->uport, UARTDM_MR2_ADDR);
	spin_unlock_irqrestore(&uport->lock, flags);


	msm_hs_clock_unvote(msm_uport);

	*val = (ret & UARTDM_MR2_LOOP_MODE_BMSK) ? 1 : 0;
	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(loopback_enable_fops, msm_serial_loopback_enable_get,
			msm_serial_loopback_enable_set, "%llu\n");

/*
 * msm_serial_hs debugfs node: <debugfs_root>/msm_serial_hs/loopback.<id>
 * writing 1 turns on internal loopback mode in HW. Useful for automation
 * test scripts.
 * writing 0 disables the internal loopback mode. Default is disabled.
 */
static void __devinit msm_serial_debugfs_init(struct msm_hs_port *msm_uport,
					   int id)
{
	char node_name[15];
	snprintf(node_name, sizeof(node_name), "loopback.%d", id);
	msm_uport->loopback_dir = debugfs_create_file(node_name,
						S_IRUGO | S_IWUSR,
						debug_base,
						msm_uport,
						&loopback_enable_fops);

	if (IS_ERR_OR_NULL(msm_uport->loopback_dir))
		pr_err("%s(): Cannot create loopback.%d debug entry",
							__func__, id);
}

static int __devexit msm_hs_remove(struct platform_device *pdev)
{

	struct msm_hs_port *msm_uport;
	struct device *dev;

	if (pdev->id < 0 || pdev->id >= UARTDM_NR) {
		printk(KERN_ERR "Invalid plaform device ID = %d\n", pdev->id);
		return -EINVAL;
	}

	msm_uport = &q_uart_port[pdev->id];
	dev = msm_uport->uport.dev;

	sysfs_remove_file(&pdev->dev.kobj, &dev_attr_clock.attr);
	debugfs_remove(msm_uport->loopback_dir);

	dma_unmap_single(dev, msm_uport->rx.mapped_cmd_ptr, sizeof(dmov_box),
			 DMA_TO_DEVICE);
	dma_pool_free(msm_uport->rx.pool, msm_uport->rx.buffer,
		      msm_uport->rx.rbuffer);
	dma_pool_destroy(msm_uport->rx.pool);

	dma_unmap_single(dev, msm_uport->rx.cmdptr_dmaaddr, sizeof(u32),
			 DMA_TO_DEVICE);
	dma_unmap_single(dev, msm_uport->tx.mapped_cmd_ptr_ptr, sizeof(u32),
			 DMA_TO_DEVICE);
	dma_unmap_single(dev, msm_uport->tx.mapped_cmd_ptr, sizeof(dmov_box),
			 DMA_TO_DEVICE);

	wake_lock_destroy(&msm_uport->rx.wake_lock);
	wake_lock_destroy(&msm_uport->dma_wake_lock);
	destroy_workqueue(msm_uport->hsuart_wq);
	mutex_destroy(&msm_uport->clk_mutex);

	uart_remove_one_port(&msm_hs_driver, &msm_uport->uport);
	clk_put(msm_uport->clk);
	if (msm_uport->pclk)
		clk_put(msm_uport->pclk);

	/* Free the tx resources */
	kfree(msm_uport->tx.command_ptr);
	kfree(msm_uport->tx.command_ptr_ptr);

	/* Free the rx resources */
	kfree(msm_uport->rx.command_ptr);
	kfree(msm_uport->rx.command_ptr_ptr);

	iounmap(msm_uport->uport.membase);

	return 0;
}

/**
 * msm_hs_config_uart_tx_rx_gpios - Configures UART Tx and RX GPIOs
 * @port: uart port
 */
static int msm_hs_config_uart_tx_rx_gpios(struct uart_port *uport)
{
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
					pdev->dev.platform_data;
	int ret = -EINVAL;


	ret = gpio_request(pdata->uart_tx_gpio, "UART_TX_GPIO");
	if (unlikely(ret)) {
		pr_err("gpio request failed for:%d\n",
				pdata->uart_tx_gpio);
		goto exit_uart_config;
	}

	ret = gpio_request(pdata->uart_rx_gpio, "UART_RX_GPIO");
	if (unlikely(ret)) {
		pr_err("gpio request failed for:%d\n",
				pdata->uart_rx_gpio);
		gpio_free(pdata->uart_tx_gpio);
		goto exit_uart_config;
	}

exit_uart_config:
	return ret;
}

/**
 * msm_hs_unconfig_uart_tx_rx_gpios: Unconfigures UART Tx and RX GPIOs
 * @port: uart port
 */
static void msm_hs_unconfig_uart_tx_rx_gpios(struct uart_port *uport)
{
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
						pdev->dev.platform_data;


	gpio_free(pdata->uart_tx_gpio);
	gpio_free(pdata->uart_rx_gpio);

}

/**
 * msm_hs_config_uart_hwflow_gpios: Configures UART HWFlow GPIOs
 * @port: uart port
 */
static int msm_hs_config_uart_hwflow_gpios(struct uart_port *uport)
{
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
						pdev->dev.platform_data;
	int ret = -EINVAL;

	ret = gpio_request(pdata->uart_cts_gpio, "UART_CTS_GPIO");
	if (unlikely(ret)) {
		pr_err("gpio request failed for:%d\n", pdata->uart_cts_gpio);
		goto exit_config_uart;
	}

	ret = gpio_request(pdata->uart_rfr_gpio, "UART_RFR_GPIO");
	if (unlikely(ret)) {
		pr_err("gpio request failed for:%d\n", pdata->uart_rfr_gpio);
		gpio_free(pdata->uart_cts_gpio);
		goto exit_config_uart;
	}

exit_config_uart:
	return ret;
}

/**
 * msm_hs_unconfig_uart_hwflow_gpios: Unonfigures UART HWFlow GPIOs
 * @port: uart port
 */
static void msm_hs_unconfig_uart_hwflow_gpios(struct uart_port *uport)
{
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
						pdev->dev.platform_data;

	gpio_free(pdata->uart_cts_gpio);
	gpio_free(pdata->uart_rfr_gpio);

}

/**
 * msm_hs_config_uart_gpios: Configures UART GPIOs and returns success or
 * Failure
 * @port: uart port
 */
static int msm_hs_config_uart_gpios(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	int ret;

	/* Configure UART Tx and Rx GPIOs */
	ret = msm_hs_config_uart_tx_rx_gpios(uport);
	if (!ret) {
		if (msm_uport->func_mode == UART_FOUR_WIRE) {
			/*if 4-wire uart, configure CTS and RFR GPIOs */
			ret = msm_hs_config_uart_hwflow_gpios(uport);
			if (ret)
				msm_hs_unconfig_uart_tx_rx_gpios(uport);
		}
	}

	return ret;
}

/**
 * msm_hs_unconfig_uart_gpios: Unconfigures UART GPIOs
 * @port: uart port
 */
static void msm_hs_unconfig_uart_gpios(struct uart_port *port)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(port);

	msm_hs_unconfig_uart_tx_rx_gpios(port);
	if (msm_uport->func_mode == UART_FOUR_WIRE)
		msm_hs_unconfig_uart_hwflow_gpios(port);
}

static int msm_hs_init_clk(struct uart_port *uport)
{
	int ret;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	/* Set up the MREG/NREG/DREG/MNDREG */
	ret = clk_set_rate(msm_uport->clk, uport->uartclk);
	if (ret) {
		printk(KERN_WARNING "Error setting clock rate on UART\n");
		return ret;
	}

	ret = msm_hs_clock_vote(msm_uport);
	if (ret) {
		printk(KERN_ERR "Error could not turn on UART clk\n");
		return ret;
	}
	return 0;
}

/*
 * programs the UARTDM_CSR register with correct bit rates
 *
 * Interrupts should be disabled before we are called, as
 * we modify Set Baud rate
 * Set receive stale interrupt level, dependant on Bit Rate
 * Goal is to have around 8 ms before indicate stale.
 * roundup (((Bit Rate * .008) / 10) + 1
 */
static unsigned long msm_hs_set_bps_locked(struct uart_port *uport,
			       unsigned int bps,
				unsigned long flags)
{
	unsigned long rxstale;
	unsigned long data;
	unsigned int curr_uartclk;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	switch (bps) {
	case 300:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x00);
		rxstale = 1;
		break;
	case 600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x11);
		rxstale = 1;
		break;
	case 1200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x22);
		rxstale = 1;
		break;
	case 2400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x33);
		rxstale = 1;
		break;
	case 4800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x44);
		rxstale = 1;
		break;
	case 9600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x55);
		rxstale = 2;
		break;
	case 14400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x66);
		rxstale = 3;
		break;
	case 19200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x77);
		rxstale = 4;
		break;
	case 28800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x88);
		rxstale = 6;
		break;
	case 38400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x99);
		rxstale = 8;
		break;
	case 57600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xaa);
		rxstale = 16;
		break;
	case 76800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xbb);
		rxstale = 16;
		break;
	case 115200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xcc);
		rxstale = 31;
		break;
	case 230400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xee);
		rxstale = 31;
		break;
	case 460800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
		rxstale = 31;
		break;
	case 4000000:
	case 3686400:
	case 3200000:
	case 3500000:
	case 3000000:
	case 2500000:
	case 1500000:
	case 1152000:
	case 1000000:
	case 921600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
		rxstale = 31;
		break;
	default:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
		/* default to 9600 */
		bps = 9600;
		rxstale = 2;
		break;
	}
	/*
	 * uart baud rate depends on CSR and MND Values
	 * we are updating CSR before and then calling
	 * clk_set_rate which updates MND Values. Hence
	 * dsb requires here.
	 */
	mb();
	curr_uartclk = uport->uartclk;
	if (bps > 460800) {
		uport->uartclk = bps * 16;
	} else {
		uport->uartclk = 7372800;
	}

	spin_unlock_irqrestore(&uport->lock, flags);

	if (curr_uartclk != uport->uartclk) {
	if (clk_set_rate(msm_uport->clk, uport->uartclk)) {
			pr_err("%s(): Error setting clock rate on UART\n",
								__func__);
		WARN_ON(1);
		spin_lock_irqsave(&uport->lock, flags);
		return flags;
	}
	}

	spin_lock_irqsave(&uport->lock, flags);
	data = rxstale & UARTDM_IPR_STALE_LSB_BMSK;
	data |= UARTDM_IPR_STALE_TIMEOUT_MSB_BMSK & (rxstale << 2);

	msm_hs_write(uport, UARTDM_IPR_ADDR, data);
	return flags;
}


static void msm_hs_set_std_bps_locked(struct uart_port *uport,
			       unsigned int bps)
{
	unsigned long rxstale;
	unsigned long data;

	switch (bps) {
	case 9600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x99);
		rxstale = 2;
		break;
	case 14400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xaa);
		rxstale = 3;
		break;
	case 19200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xbb);
		rxstale = 4;
		break;
	case 28800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xcc);
		rxstale = 6;
		break;
	case 38400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xdd);
		rxstale = 8;
		break;
	case 57600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xee);
		rxstale = 16;
		break;
	case 115200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
		rxstale = 31;
		break;
	default:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x99);
		/* default to 9600 */
		bps = 9600;
		rxstale = 2;
		break;
	}

	data = rxstale & UARTDM_IPR_STALE_LSB_BMSK;
	data |= UARTDM_IPR_STALE_TIMEOUT_MSB_BMSK & (rxstale << 2);

	msm_hs_write(uport, UARTDM_IPR_ADDR, data);
}

/*
 * termios :  new ktermios
 * oldtermios:  old ktermios previous setting
 *
 * Configure the serial port
 */
static void msm_hs_set_termios(struct uart_port *uport,
				   struct ktermios *termios,
				   struct ktermios *oldtermios)
{
	int ret;
	unsigned int bps;
	unsigned long data;
	unsigned long flags;
	unsigned int c_cflag = termios->c_cflag;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	bool error_case = false;

	mutex_lock(&msm_uport->clk_mutex);
	spin_lock_irqsave(&uport->lock, flags);

	msm_uport->termios_in_progress = true;

	/* Disable RxStale Event Mechanism */
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

	/* Disable all UART interrupts */
	msm_hs_write(uport, UARTDM_IMR_ADDR, 0);

	/* Enable RFR so remote UART doesn't send any data. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_HIGH);

	/*
	 * It is quite possible that previous graceful flush is not
	 * completed and set_termios() request has been received.
	 * Hence wait here to make sure that it is completed and
	 * queued one more UART RX CMD with ADM.
	 */
	if (msm_uport->rx.dma_in_flight &&
			msm_uport->rx.flush == FLUSH_DATA_READY) {
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx.flush == FLUSH_NONE,
			RX_FLUSH_COMPLETE_TIMEOUT);
		if (!ret) {
			pr_err("%s(): timeout for Rx cmd completion\n",
							__func__);
			spin_lock_irqsave(&uport->lock, flags);
			print_uart_registers(msm_uport);
			spin_unlock_irqrestore(&uport->lock, flags);
			BUG_ON(1);
		}

		spin_lock_irqsave(&uport->lock, flags);
	}

	/*
	 * Wait for queued Rx CMD to ADM driver to be programmed
	 * with ADM hardware before going and changing UART baud rate.
	 * Below udelay(500) is required as exec_cmd callback is called
	 * before actually programming ADM hardware with cmd.
	 */
	if (msm_uport->rx.dma_in_flight) {
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx.cmd_exec == true,
			RX_FLUSH_COMPLETE_TIMEOUT);
		if (!ret)
			pr_err("%s(): timeout for rx cmd to be program\n",
								__func__);
		else
			udelay(500);

		spin_lock_irqsave(&uport->lock, flags);
	}

	/* 300 is the minimum baud support by the driver  */
	bps = uart_get_baud_rate(uport, termios, oldtermios, 200, 4000000);

	/* Temporary remapping  200 BAUD to 3.2 mbps */
	if (bps == 200)
		bps = 3200000;

	uport->uartclk = clk_get_rate(msm_uport->clk);
	if (!uport->uartclk)
		msm_hs_set_std_bps_locked(uport, bps);
	else
		flags = msm_hs_set_bps_locked(uport, bps, flags);

	data = msm_hs_read(uport, UARTDM_MR2_ADDR);
	data &= ~UARTDM_MR2_PARITY_MODE_BMSK;
	/* set parity */
	if (PARENB == (c_cflag & PARENB)) {
		if (PARODD == (c_cflag & PARODD)) {
			data |= ODD_PARITY;
		} else if (CMSPAR == (c_cflag & CMSPAR)) {
			data |= SPACE_PARITY;
		} else {
			data |= EVEN_PARITY;
		}
	}

	/* Set bits per char */
	data &= ~UARTDM_MR2_BITS_PER_CHAR_BMSK;

	switch (c_cflag & CSIZE) {
	case CS5:
		data |= FIVE_BPC;
		break;
	case CS6:
		data |= SIX_BPC;
		break;
	case CS7:
		data |= SEVEN_BPC;
		break;
	default:
		data |= EIGHT_BPC;
		break;
	}
	/* stop bits */
	if (c_cflag & CSTOPB) {
		data |= STOP_BIT_TWO;
	} else {
		/* otherwise 1 stop bit */
		data |= STOP_BIT_ONE;
	}
	data |= UARTDM_MR2_ERROR_MODE_BMSK;
	/* write parity/bits per char/stop bit configuration */
	msm_hs_write(uport, UARTDM_MR2_ADDR, data);

	uport->ignore_status_mask = termios->c_iflag & INPCK;
	uport->ignore_status_mask |= termios->c_iflag & IGNPAR;
	uport->ignore_status_mask |= termios->c_iflag & IGNBRK;

	uport->read_status_mask = (termios->c_cflag & CREAD);

	/* Wait for baud rate and UART protocol parameters to set. */
	udelay(200);

	/* Set Transmit software time out */
	uart_update_timeout(uport, c_cflag, bps);

	/*
	 * Configure HW flow control
	 * UART Core would see status of CTS line when it is sending data
	 * to remote uart to confirm that it can receive or not.
	 * UART Core would trigger RFR if it is not having any space with
	 * RX FIFO.
	 */
	data = msm_hs_read(uport, UARTDM_MR1_ADDR);
	data &= ~(UARTDM_MR1_CTS_CTL_BMSK | UARTDM_MR1_RX_RDY_CTL_BMSK);
	if (c_cflag & CRTSCTS) {
		data |= UARTDM_MR1_CTS_CTL_BMSK;
		data |= UARTDM_MR1_RX_RDY_CTL_BMSK;
	}

	msm_hs_write(uport, UARTDM_MR1_ADDR, data);

	/* Enable previously enabled all UART interrupts. */
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);

	/*
	 * Invoke Force RxStale Interrupt
	 * On receiving this interrupt, send discard flush request
	 * to ADM driver and ignore all received data.
	 */
	wake_lock(&msm_uport->rx.wake_lock);
	msm_hs_write(uport, UARTDM_CR_ADDR, FORCE_STALE_EVENT);
	mb();

	msm_uport->rx_discard_flush_issued = true;

		/*
	 * Wait for above discard flush request for UART RX CMD to be
	 * completed. completion would be signal from rx_tlet without
	 * queueing any next UART RX CMD.
		 */
	if (msm_uport->rx.dma_in_flight) {
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx_discard_flush_issued == false,
			msecs_to_jiffies(300));
			if (!ret) {
				pr_err("%s(): timeout for rx discard flush\n",
								__func__);
				spin_lock_irqsave(&uport->lock, flags);
				print_uart_registers(msm_uport);
				error_case = true;
			}
		if (!error_case)
			spin_lock_irqsave(&uport->lock, flags);
	} else {
		spin_unlock_irqrestore(&uport->lock, flags);
		pr_err("%s(): called with rx.dma_in_flight:%d\n",
				__func__, msm_uport->rx.dma_in_flight);
		print_uart_registers(msm_uport);
		spin_lock_irqsave(&uport->lock, flags);
	}

	/* Start Rx Transfer */
	msm_hs_start_rx_locked(&msm_uport->uport);

	/* Disable RFR so remote UART can send data. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_LOW);
	mb();

	msm_uport->termios_in_progress = false;

	spin_unlock_irqrestore(&uport->lock, flags);
	mutex_unlock(&msm_uport->clk_mutex);
}

/*
 *  Standard API, Transmitter
 *  Any character in the transmit shift register is sent
 */
unsigned int msm_hs_tx_empty(struct uart_port *uport)
{
	unsigned int data;
	unsigned int ret = 0;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	if (msm_uport->clk_state == MSM_HS_CLK_OFF) {
		pr_err("%s: Failing GSBI clocks are OFF\n", __func__);
		return 0;
	}
	data = msm_hs_read(uport, UARTDM_SR_ADDR);

	if (data & UARTDM_SR_TXEMT_BMSK)
		ret = TIOCSER_TEMT;

	return ret;
}
EXPORT_SYMBOL(msm_hs_tx_empty);

/*
 *  Standard API, Stop transmitter.
 *  Any character in the transmit shift register is sent as
 *  well as the current data mover transfer .
 */
static void msm_hs_stop_tx_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	msm_uport->tx.tx_ready_int_en = 0;
}

/*
 *  Standard API, Stop receiver as soon as possible.
 *
 *  Function immediately terminates the operation of the
 *  channel receiver and any incoming characters are lost. None
 *  of the receiver status bits are affected by this command and
 *  characters that are already in the receive FIFO there.
 */
static void msm_hs_stop_rx_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	if (msm_uport->clk_state == MSM_HS_CLK_OFF)
		return;

	/* Disable RxStale Event Mechanism */
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

	/* Enable RFR so remote UART doesn't send any data. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_HIGH);

	/* Allow to receive all pending data from UART RX FIFO */
	udelay(100);

	if (msm_uport->rx.flush == FLUSH_NONE) {
		wake_lock(&msm_uport->rx.wake_lock);

		msm_uport->rx_discard_flush_issued = true;

		/*
		 * Invoke Force RxStale Interrupt
		 * On receiving this interrupt, send discard flush request
		 * to ADM driver and ignore all received data.
		 */
		msm_hs_write(uport, UARTDM_CR_ADDR, FORCE_STALE_EVENT);
		mb();
	}

	if (msm_uport->rx.flush != FLUSH_SHUTDOWN)
		msm_uport->rx.flush = FLUSH_STOP;
}

/*  Transmit the next chunk of data */
static void msm_hs_submit_tx_locked(struct uart_port *uport)
{
	int left;
	int tx_count;
	int aligned_tx_count;
	dma_addr_t src_addr;
	dma_addr_t aligned_src_addr;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct msm_hs_tx *tx = &msm_uport->tx;
	struct circ_buf *tx_buf = &msm_uport->uport.state->xmit;

	if (tx->dma_in_flight || msm_uport->is_shutdown)
		return;

	if (uart_circ_empty(tx_buf) || uport->state->port.tty->stopped) {
		msm_hs_stop_tx_locked(uport);
		return;
	}

	tx->dma_in_flight = 1;

	tx_count = uart_circ_chars_pending(tx_buf);

	if (UARTDM_TX_BUF_SIZE < tx_count)
		tx_count = UARTDM_TX_BUF_SIZE;

	left = UART_XMIT_SIZE - tx_buf->tail;

	if (tx_count > left)
		tx_count = left;

	src_addr = tx->dma_base + tx_buf->tail;
	/* Mask the src_addr to align on a cache
	 * and add those bytes to tx_count */
	aligned_src_addr = src_addr & ~(dma_get_cache_alignment() - 1);
	aligned_tx_count = tx_count + src_addr - aligned_src_addr;

	dma_sync_single_for_device(uport->dev, aligned_src_addr,
			aligned_tx_count, DMA_TO_DEVICE);

	tx->command_ptr->num_rows = (((tx_count + 15) >> 4) << 16) |
				     ((tx_count + 15) >> 4);
	tx->command_ptr->src_row_addr = src_addr;

	dma_sync_single_for_device(uport->dev, tx->mapped_cmd_ptr,
				   sizeof(dmov_box), DMA_TO_DEVICE);

	*tx->command_ptr_ptr = CMD_PTR_LP | DMOV_CMD_ADDR(tx->mapped_cmd_ptr);

	/* Save tx_count to use in Callback */
	tx->tx_count = tx_count;
	msm_hs_write(uport, UARTDM_NCF_TX_ADDR, tx_count);

	/* Disable the tx_ready interrupt */
	msm_uport->imr_reg &= ~UARTDM_ISR_TX_READY_BMSK;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
	/* Calling next DMOV API. Hence mb() here. */
	mb();

	dma_sync_single_for_device(uport->dev, tx->mapped_cmd_ptr_ptr,
				   sizeof(u32), DMA_TO_DEVICE);
	msm_uport->tx.flush = FLUSH_NONE;
	msm_dmov_enqueue_cmd(msm_uport->dma_tx_channel, &tx->xfer);
}

/* Start to receive the next chunk of data */
static void msm_hs_start_rx_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	unsigned int buffer_pending = msm_uport->rx.buffer_pending;
	unsigned int data;

	if (msm_uport->rx.dma_in_flight) {
		pr_err("%s(): RX CMD is already queued.\n", __func__);
		print_uart_registers(msm_uport);
		BUG_ON(1);
	}

	msm_uport->rx.buffer_pending = 0;
	if (buffer_pending && hs_serial_debug_mask)
		printk(KERN_ERR "Error: rx started in buffer state = %x",
		       buffer_pending);
	/*
	 * Zeroed out UART RX software buffer which would help to
	 * check how much data is copied if there is any RX stall.
	 */
	memset(msm_uport->rx.buffer, 0x00, msm_uport->rx_buf_size);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_STALE_INT);
	msm_hs_write(uport, UARTDM_DMRX_ADDR, msm_uport->rx_buf_size);
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_ENABLE);
	msm_uport->imr_reg |= UARTDM_ISR_RXLEV_BMSK;

	/*
	 * Enable UARTDM Rx Interface as previously it has been
	 * disable in set_termios before configuring baud rate.
	 */
	data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
	data |= UARTDM_RX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
	/* Calling next DMOV API. Hence mb() here. */
	mb();

	msm_uport->rx.flush = FLUSH_NONE;
	msm_uport->rx.dma_in_flight = true;
	msm_uport->rx.cmd_exec = false;
	msm_dmov_enqueue_cmd_ext(msm_uport->dma_rx_channel,
					&msm_uport->rx.xfer);
	/*
	 * wake up set_termios() process if it is waiting for
	 * completion of previous UART RX CMD and queueing of
	 * new UART RX CMD before going ahead with baud rate
	 * change process.
	 */
	if (msm_uport->termios_in_progress)
		wake_up(&msm_uport->rx.wait);
}

static void flip_insert_work(struct work_struct *work)
{
	unsigned long flags;
	int retval;
	struct msm_hs_port *msm_uport =
		container_of(work, struct msm_hs_port,
			     rx.flip_insert_work.work);
	struct tty_struct *tty = msm_uport->uport.state->port.tty;

	spin_lock_irqsave(&msm_uport->uport.lock, flags);
	if (msm_uport->rx.buffer_pending == NONE_PENDING) {
		if (hs_serial_debug_mask)
			printk(KERN_ERR "Error: No buffer pending in %s",
			       __func__);
		return;
	}
	if (msm_uport->rx.buffer_pending & FIFO_OVERRUN) {
		retval = tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		if (retval)
			msm_uport->rx.buffer_pending &= ~FIFO_OVERRUN;
	}
	if (msm_uport->rx.buffer_pending & PARITY_ERROR) {
		retval = tty_insert_flip_char(tty, 0, TTY_PARITY);
		if (retval)
			msm_uport->rx.buffer_pending &= ~PARITY_ERROR;
	}
	if (msm_uport->rx.buffer_pending & CHARS_NORMAL) {
		int rx_count, rx_offset;
		rx_count = (msm_uport->rx.buffer_pending & 0xFFFF0000) >> 16;
		rx_offset = (msm_uport->rx.buffer_pending & 0xFFD0) >> 5;
		retval = tty_insert_flip_string(tty, msm_uport->rx.buffer +
						rx_offset, rx_count);
		msm_uport->rx.buffer_pending &= (FIFO_OVERRUN |
						 PARITY_ERROR);
		if (retval != rx_count)
			msm_uport->rx.buffer_pending |= CHARS_NORMAL |
				retval << 8 | (rx_count - retval) << 16;
	}
	if (msm_uport->rx.buffer_pending)
		schedule_delayed_work(&msm_uport->rx.flip_insert_work,
				      msecs_to_jiffies(RETRY_TIMEOUT));
	else
		if ((msm_uport->clk_state == MSM_HS_CLK_ON) &&
		    (msm_uport->rx.flush <= FLUSH_IGNORE)) {
			if (hs_serial_debug_mask)
				printk(KERN_WARNING
				       "msm_serial_hs: "
				       "Pending buffers cleared. "
				       "Restarting\n");
			msm_hs_start_rx_locked(&msm_uport->uport);
		}
	spin_unlock_irqrestore(&msm_uport->uport.lock, flags);
	tty_flip_buffer_push(tty);
}

static void msm_serial_hs_rx_tlet(unsigned long tlet_ptr)
{
	int retval;
	int rx_count;
	unsigned long status;
	unsigned long flags;
	unsigned int error_f = 0;
	struct uart_port *uport;
	struct msm_hs_port *msm_uport;
	unsigned int flush;
	struct tty_struct *tty;

	msm_uport = container_of((struct tasklet_struct *)tlet_ptr,
				 struct msm_hs_port, rx.tlet);
	uport = &msm_uport->uport;
	tty = uport->state->port.tty;

	status = msm_hs_read(uport, UARTDM_SR_ADDR);

	spin_lock_irqsave(&uport->lock, flags);

	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

	/* Rx DMA cmd is completed here. */
	msm_uport->rx.dma_in_flight = false;

	/* overflow is not connect to data in a FIFO */
	if (unlikely((status & UARTDM_SR_OVERRUN_BMSK) &&
		     (uport->read_status_mask & CREAD))) {
		retval = tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		if (!retval)
			msm_uport->rx.buffer_pending |= TTY_OVERRUN;
		uport->icount.buf_overrun++;
		error_f = 1;
	}

	if (!(uport->ignore_status_mask & INPCK))
		status = status & ~(UARTDM_SR_PAR_FRAME_BMSK);

	if (unlikely(status & UARTDM_SR_PAR_FRAME_BMSK)) {
		/* Can not tell difference between parity & frame error */
		if (hs_serial_debug_mask)
			printk(KERN_WARNING "msm_serial_hs: parity error\n");
		uport->icount.parity++;
		error_f = 1;
		if (!(uport->ignore_status_mask & IGNPAR)) {
			retval = tty_insert_flip_char(tty, 0, TTY_PARITY);
			if (!retval)
				msm_uport->rx.buffer_pending |= TTY_PARITY;
		}
	}

	if (unlikely(status & UARTDM_SR_RX_BREAK_BMSK)) {
		if (hs_serial_debug_mask)
			printk(KERN_WARNING "msm_serial_hs: Rx break\n");
		uport->icount.brk++;
		error_f = 1;
		msm_hs_write(uport, UARTDM_CR_ADDR, RESET_BREAK_INT);
		if (!(uport->ignore_status_mask & IGNBRK)) {
			retval = tty_insert_flip_char(tty, 0, TTY_BREAK);
			if (!retval)
				msm_uport->rx.buffer_pending |= TTY_BREAK;
		}
	}

	if (error_f)
		msm_hs_write(uport, UARTDM_CR_ADDR, RESET_ERROR_STATUS);

	if (msm_uport->clk_req_off_state == CLK_REQ_OFF_FLUSH_ISSUED)
		msm_uport->clk_req_off_state = CLK_REQ_OFF_RXSTALE_FLUSHED;
	flush = msm_uport->rx.flush;

	/*
	 * Part hs_isr() sets the flush as FLUSH_IGNORE if it is explictily
	 * trigger from set_termios().
	 */
	if (flush == FLUSH_IGNORE) {
		if (!msm_uport->rx_discard_flush_issued &&
					!msm_uport->rx.buffer_pending) {
			msm_hs_start_rx_locked(uport);
		} else {
			msm_uport->rx_discard_flush_issued = false;
			wake_up(&msm_uport->rx.wait);
			goto out;
		}
	}

	/* Part of stop_rx sets the flush as FLUSH_STOP */
	if (flush == FLUSH_STOP) {
		if (msm_uport->rx_discard_flush_issued)
			msm_uport->rx_discard_flush_issued = false;
		msm_uport->rx.flush = FLUSH_SHUTDOWN;
		wake_up(&msm_uport->rx.wait);
	}

	if (flush >= FLUSH_DATA_INVALID)
		goto out;

	rx_count = msm_hs_read(uport, UARTDM_RX_TOTAL_SNAP_ADDR);

	/* order the read of rx.buffer */
	rmb();

	if (0 != (uport->read_status_mask & CREAD)) {
		retval = tty_insert_flip_string(tty, msm_uport->rx.buffer,
						rx_count);
		if (retval != rx_count) {
			msm_uport->rx.buffer_pending |= CHARS_NORMAL |
				retval << 5 | (rx_count - retval) << 16;
		}
	}

	/* order the read of rx.buffer and the start of next rx xfer */
	wmb();

	if (!msm_uport->rx.buffer_pending)
		msm_hs_start_rx_locked(uport);

out:
	if (msm_uport->rx.buffer_pending) {
		if (hs_serial_debug_mask)
			printk(KERN_WARNING
			       "msm_serial_hs: "
			       "tty buffer exhausted. "
			       "Stalling\n");
		schedule_delayed_work(&msm_uport->rx.flip_insert_work
				      , msecs_to_jiffies(RETRY_TIMEOUT));
	}
	/* release wakelock in 500ms, not immediately, because higher layers
	 * don't always take wakelocks when they should */
	wake_lock_timeout(&msm_uport->rx.wake_lock, HZ / 2);
	/* tty_flip_buffer_push() might call msm_hs_start(), so unlock */
	spin_unlock_irqrestore(&uport->lock, flags);
	if (flush < FLUSH_DATA_INVALID)
		tty_flip_buffer_push(tty);
}

/* Enable the transmitter Interrupt */
static void msm_hs_start_tx_locked(struct uart_port *uport )
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	if (msm_uport->is_shutdown)
		return;

	if (msm_uport->clk_state == MSM_HS_CLK_OFF) {
		pr_err("%s:Failing as GSBI clocks are OFF\n", __func__);
		return;
	}

	if (msm_uport->tx.tx_ready_int_en == 0) {
		msm_uport->tx.tx_ready_int_en = 1;
		if (msm_uport->tx.dma_in_flight == 0)
			msm_hs_submit_tx_locked(uport);
	}
}

/*
 *  This routine is called when we are done with a DMA transfer
 *
 *  This routine is registered with Data mover when we set
 *  up a Data Mover transfer. It is called from Data mover ISR
 *  when the DMA transfer is done.
 */
static void msm_hs_dmov_tx_callback(struct msm_dmov_cmd *cmd_ptr,
					unsigned int result,
					struct msm_dmov_errdata *err)
{
	struct msm_hs_port *msm_uport;

	msm_uport = container_of(cmd_ptr, struct msm_hs_port, tx.xfer);
	if (msm_uport->tx.flush == FLUSH_STOP)
		/* DMA FLUSH unsuccesfful */
		WARN_ON(!(result & DMOV_RSLT_FLUSH));
	else
		/* DMA did not finish properly */
		WARN_ON(!(result & DMOV_RSLT_DONE));

	tasklet_schedule(&msm_uport->tx.tlet);
}

static void msm_serial_hs_tx_tlet(unsigned long tlet_ptr)
{
	unsigned long flags;
	struct msm_hs_port *msm_uport = container_of((struct tasklet_struct *)
				tlet_ptr, struct msm_hs_port, tx.tlet);
	struct msm_hs_tx *tx = &msm_uport->tx;

	spin_lock_irqsave(&(msm_uport->uport.lock), flags);

	tx->dma_in_flight = 0;
	if (msm_uport->tx.flush == FLUSH_STOP) {
		msm_uport->tx.flush = FLUSH_SHUTDOWN;
		wake_up(&msm_uport->tx.wait);
		spin_unlock_irqrestore(&(msm_uport->uport.lock), flags);
		return;
	}

	msm_uport->imr_reg |= UARTDM_ISR_TX_READY_BMSK;
	msm_hs_write(&(msm_uport->uport), UARTDM_IMR_ADDR, msm_uport->imr_reg);
	/* Calling clk API. Hence mb() requires. */
	mb();

	spin_unlock_irqrestore(&(msm_uport->uport.lock), flags);
}

/*
 * This routine is called when ADM driver is about to program queued UART
 * RX CMD with ADM hardware.
 */
static void msm_hs_dmov_rx_exec_callback(struct msm_dmov_cmd *cmd_ptr)
{
	struct msm_hs_port *msm_uport;
	struct msm_hs_rx *rx;

	msm_uport = container_of(cmd_ptr, struct msm_hs_port, rx.xfer);
	rx = &msm_uport->rx;

	rx->cmd_exec = true;
	/*
	 * wakeup set_termios() as  it waits for UART RX CMD to be programmed
	 * with ADM hardware.
	 */
	wake_up(&msm_uport->rx.wait);
}

/*
 * This routine is called when we are done with a DMA transfer or the
 * a flush has been sent to the data mover driver.
 *
 * This routine is registered with Data mover when we set up a Data Mover
 *  transfer. It is called from Data mover ISR when the DMA transfer is done.
 */
static void msm_hs_dmov_rx_callback(struct msm_dmov_cmd *cmd_ptr,
					unsigned int result,
					struct msm_dmov_errdata *err)
{
	struct msm_hs_port *msm_uport;

	if (result & DMOV_RSLT_ERROR)
		pr_err("%s(): DMOV_RSLT_ERROR\n", __func__);

	msm_uport = container_of(cmd_ptr, struct msm_hs_port, rx.xfer);

	tasklet_schedule(&msm_uport->rx.tlet);
}

/*
 *  Standard API, Current states of modem control inputs
 *
 * Since CTS can be handled entirely by HARDWARE we always
 * indicate clear to send and count on the TX FIFO to block when
 * it fills up.
 *
 * - TIOCM_DCD
 * - TIOCM_CTS
 * - TIOCM_DSR
 * - TIOCM_RI
 *  (Unsupported) DCD and DSR will return them high. RI will return low.
 */
static unsigned int msm_hs_get_mctrl_locked(struct uart_port *uport)
{
	return TIOCM_DSR | TIOCM_CAR | TIOCM_CTS;
}

/*
 *  Standard API, Set or clear RFR_signal
 *
 * Set RFR high, (Indicate we are not ready for data), we disable auto
 * ready for receiving and then set RFR_N high. To set RFR to low we just turn
 * back auto ready for receiving and it should lower RFR signal
 * when hardware is ready
 */
void msm_hs_set_mctrl_locked(struct uart_port *uport,
				    unsigned int mctrl)
{
	unsigned int set_rts;
	unsigned int data;

	/* RTS is active low */
	set_rts = TIOCM_RTS & mctrl ? 0 : 1;

	data = msm_hs_read(uport, UARTDM_MR1_ADDR);
	if (set_rts) {
		/*disable auto ready-for-receiving */
		data &= ~UARTDM_MR1_RX_RDY_CTL_BMSK;
		msm_hs_write(uport, UARTDM_MR1_ADDR, data);
		/* set RFR_N to high */
		msm_hs_write(uport, UARTDM_CR_ADDR, RFR_HIGH);
	} else {
		/* Enable auto ready-for-receiving */
		data |= UARTDM_MR1_RX_RDY_CTL_BMSK;
		msm_hs_write(uport, UARTDM_MR1_ADDR, data);
	}
	mb();
}

void msm_hs_set_mctrl(struct uart_port *uport,
				    unsigned int mctrl)
{
	unsigned long flags;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	if (msm_uport->clk_state == MSM_HS_CLK_OFF) {
		pr_err("%s:Failing as GSBI clocks are OFF\n", __func__);
		return;
	}
	spin_lock_irqsave(&uport->lock, flags);
	msm_hs_set_mctrl_locked(uport, mctrl);
	spin_unlock_irqrestore(&uport->lock, flags);
}
EXPORT_SYMBOL(msm_hs_set_mctrl);

/* Standard API, Enable modem status (CTS) interrupt  */
static void msm_hs_enable_ms_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	/* Enable DELTA_CTS Interrupt */
	msm_uport->imr_reg |= UARTDM_ISR_DELTA_CTS_BMSK;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
	mb();

}

static void msm_hs_flush_buffer_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	if (msm_uport->tx.dma_in_flight)
		msm_uport->tty_flush_receive = true;
}

static void msm_hs_power(struct uart_port *port, unsigned int state,
			  unsigned int oldstate)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(port);

	switch (state) {
	case 1:
		msm_hs_request_clock_on(&msm_uport->uport);
		break;
	default:
		pr_err("Unknown PM state %d\n", state);
	}
}

/*
 *  Standard API, Break Signal
 *
 * Control the transmission of a break signal. ctl eq 0 => break
 * signal terminate ctl ne 0 => start break signal
 */
static void msm_hs_break_ctl(struct uart_port *uport, int ctl)
{
	unsigned long flags;

	spin_lock_irqsave(&uport->lock, flags);
	msm_hs_write(uport, UARTDM_CR_ADDR, ctl ? START_BREAK : STOP_BREAK);
	mb();
	spin_unlock_irqrestore(&uport->lock, flags);
}

static void msm_hs_config_port(struct uart_port *uport, int cfg_flags)
{
	unsigned long flags;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	if (cfg_flags & UART_CONFIG_TYPE) {
		uport->type = PORT_MSM;
		msm_hs_request_port(uport);
	}

	if (is_gsbi_uart(msm_uport)) {
		if (msm_uport->pclk)
			clk_prepare_enable(msm_uport->pclk);
		spin_lock_irqsave(&uport->lock, flags);
		iowrite32(GSBI_PROTOCOL_UART, msm_uport->mapped_gsbi +
			  GSBI_CONTROL_ADDR);
		spin_unlock_irqrestore(&uport->lock, flags);
		if (msm_uport->pclk)
			clk_disable_unprepare(msm_uport->pclk);
	}
}

/*  Handle CTS changes (Called from interrupt handler) */
static void msm_hs_handle_delta_cts_locked(struct uart_port *uport)
{
	/* clear interrupt */
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_CTS);
	/* Calling CLOCK API. Hence mb() requires here. */
	mb();
	uport->icount.cts++;

	/* clear the IOCTL TIOCMIWAIT if called */
	wake_up_interruptible(&uport->state->port.delta_msr_wait);
}

/* check if the TX path is flushed, and if so clock off
 * returns 0 did not clock off, need to retry (still sending final byte)
 *        -1 did not clock off, do not retry
 *         1 if we clocked off
 */
static int msm_hs_check_clock_off(struct uart_port *uport)
{
	unsigned long sr_status;
	unsigned long flags;
	int ret;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct circ_buf *tx_buf = &uport->state->xmit;

	mutex_lock(&msm_uport->clk_mutex);
	spin_lock_irqsave(&uport->lock, flags);

	/* Cancel if tx tty buffer is not empty, dma is in flight,
	 * or tx fifo is not empty */
	if (msm_uport->clk_state != MSM_HS_CLK_REQUEST_OFF ||
	    !uart_circ_empty(tx_buf) || msm_uport->tx.dma_in_flight ||
	    msm_uport->imr_reg & UARTDM_ISR_TXLEV_BMSK) {
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
		return -1;
	}

	/* Make sure the uart is finished with the last byte */
	sr_status = msm_hs_read(uport, UARTDM_SR_ADDR);
	if (!(sr_status & UARTDM_SR_TXEMT_BMSK)) {
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* retry */
	}

	/* Make sure forced RXSTALE flush complete */
	switch (msm_uport->clk_req_off_state) {
	case CLK_REQ_OFF_START:
		msm_uport->clk_req_off_state = CLK_REQ_OFF_RXSTALE_ISSUED;
		wake_lock(&msm_uport->rx.wake_lock);
		msm_hs_write(uport, UARTDM_CR_ADDR, FORCE_STALE_EVENT);
		/*
		 * Before returning make sure that device writel completed.
		 * Hence mb() requires here.
		 */
		mb();
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* RXSTALE flush not complete - retry */
	case CLK_REQ_OFF_RXSTALE_ISSUED:
	case CLK_REQ_OFF_FLUSH_ISSUED:
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* RXSTALE flush not complete - retry */
	case CLK_REQ_OFF_RXSTALE_FLUSHED:
		break;  /* continue */
	}

	if (msm_uport->rx.flush != FLUSH_SHUTDOWN) {
		if (msm_uport->rx.flush == FLUSH_NONE) {
			msm_hs_stop_rx_locked(uport);
			msm_uport->rx_discard_flush_issued = true;
		}

		spin_unlock_irqrestore(&uport->lock, flags);
		if (msm_uport->rx_discard_flush_issued) {
			pr_debug("%s(): wainting for flush completion.\n",
								__func__);
			ret = wait_event_timeout(msm_uport->rx.wait,
				msm_uport->rx_discard_flush_issued == false,
				RX_FLUSH_COMPLETE_TIMEOUT);
			if (!ret)
				pr_err("%s(): Flush complete pending.\n",
								__func__);
		}

		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* come back later to really clock off */
	}

	spin_unlock_irqrestore(&uport->lock, flags);

	/* we really want to clock off */
	msm_hs_clock_unvote(msm_uport);

	spin_lock_irqsave(&uport->lock, flags);
	if (use_low_power_wakeup(msm_uport)) {
		msm_uport->wakeup.ignore = 1;
		enable_irq(msm_uport->wakeup.irq);
	}
	printk(KERN_INFO "(msm_serial_hs) msm_hs_check_clock_off - dma wake unlock\n");
	wake_unlock(&msm_uport->dma_wake_lock);

	spin_unlock_irqrestore(&uport->lock, flags);
	mutex_unlock(&msm_uport->clk_mutex);
	return 1;
}

static void hsuart_clock_off_work(struct work_struct *w)
{
	struct msm_hs_port *msm_uport = container_of(w, struct msm_hs_port,
							clock_off_w);
	struct uart_port *uport = &msm_uport->uport;

	int check_clk_off = msm_hs_check_clock_off(uport);

	if (!check_clk_off) {
		hrtimer_start(&msm_uport->clk_off_timer,
				msm_uport->clk_off_delay,
				HRTIMER_MODE_REL);
	} else if (check_clk_off == -1) {
		printk(KERN_INFO "(msm_serial_hs) hsuart_clock_off_work WORKQUEUE - FIFO is not empty or in flight...\n");
	} else {
		printk(KERN_INFO "(msm_serial_hs) hsuart_clock_off_work WORKQUEUE - Maybe, clock is off-ed.\n");
	}
}

static enum hrtimer_restart msm_hs_clk_off_retry(struct hrtimer *timer)
{
	struct msm_hs_port *msm_uport = container_of(timer, struct msm_hs_port,
							clk_off_timer);

	queue_work(msm_uport->hsuart_wq, &msm_uport->clock_off_w);
	return HRTIMER_NORESTART;
}

static irqreturn_t msm_hs_isr(int irq, void *dev)
{
	unsigned long flags;
	unsigned long isr_status;
	struct msm_hs_port *msm_uport = (struct msm_hs_port *)dev;
	struct uart_port *uport = &msm_uport->uport;
	struct circ_buf *tx_buf = &uport->state->xmit;
	struct msm_hs_tx *tx = &msm_uport->tx;
	struct msm_hs_rx *rx = &msm_uport->rx;

	spin_lock_irqsave(&uport->lock, flags);

	if (msm_uport->is_shutdown) {
		pr_err("%s(): Received UART interrupt after shutdown.\n",
								__func__);
		spin_unlock_irqrestore(&uport->lock, flags);
		return IRQ_HANDLED;
	}

	isr_status = msm_hs_read(uport, UARTDM_MISR_ADDR);

	/* Uart RX starting */
	if (isr_status & UARTDM_ISR_RXLEV_BMSK) {
		wake_lock(&rx->wake_lock);  /* hold wakelock while rx dma */
		msm_uport->imr_reg &= ~UARTDM_ISR_RXLEV_BMSK;
		msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
		/* Complete device write for IMR. Hence mb() requires. */
		mb();
	}
	/* Stale rx interrupt */
	if (isr_status & UARTDM_ISR_RXSTALE_BMSK) {
		msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);
		msm_hs_write(uport, UARTDM_CR_ADDR, RESET_STALE_INT);
		/*
		 * Complete device write before calling DMOV API. Hence
		 * mb() requires here.
		 */
		mb();

		if (msm_uport->clk_req_off_state == CLK_REQ_OFF_RXSTALE_ISSUED)
			msm_uport->clk_req_off_state =
				CLK_REQ_OFF_FLUSH_ISSUED;

		if (rx->flush == FLUSH_NONE) {
			if (!msm_uport->termios_in_progress) {
			rx->flush = FLUSH_DATA_READY;
				/* Graceful Flush */
			msm_dmov_flush(msm_uport->dma_rx_channel, 1);
			} else {
				rx->flush = FLUSH_IGNORE;
				/* Discard Flush */
				msm_dmov_flush(msm_uport->dma_rx_channel, 0);
			}
		}

		/*
		 * Force RxStale is performed from msm_hs_stop_rx_locked() and
		 * change rx.flush to FLUSH_STOP.
		 */
		if (rx->flush == FLUSH_STOP) {
			if (msm_uport->rx_discard_flush_issued)
				/* Discard Flush */
				msm_dmov_flush(msm_uport->dma_rx_channel, 0);
		}
	}

	/* tx ready interrupt */
	if (isr_status & UARTDM_ISR_TX_READY_BMSK) {
		/* Clear  TX Ready */
		msm_hs_write(uport, UARTDM_CR_ADDR, CLEAR_TX_READY);

		if (msm_uport->clk_state == MSM_HS_CLK_REQUEST_OFF) {
			msm_uport->imr_reg |= UARTDM_ISR_TXLEV_BMSK;
			msm_hs_write(uport, UARTDM_IMR_ADDR,
				     msm_uport->imr_reg);
		}
		/*
		 * Complete both writes before starting new TX.
		 * Hence mb() requires here.
		 */
		mb();
		/* Complete DMA TX transactions and submit new transactions */

		/* Do not update tx_buf.tail if uart_flush_buffer already
						called in serial core */
		if (!msm_uport->tty_flush_receive)
			tx_buf->tail = (tx_buf->tail +
					tx->tx_count) & ~UART_XMIT_SIZE;
		else
			msm_uport->tty_flush_receive = false;

		uport->icount.tx += tx->tx_count;
		if (tx->tx_ready_int_en)
			msm_hs_submit_tx_locked(uport);

		if (uart_circ_chars_pending(tx_buf) < WAKEUP_CHARS)
			uart_write_wakeup(uport);
	}
	if (isr_status & UARTDM_ISR_TXLEV_BMSK) {
		/* TX FIFO is empty */
		msm_uport->imr_reg &= ~UARTDM_ISR_TXLEV_BMSK;
		msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
		/*
		 * Complete device write before starting clock_off request.
		 * Hence mb() requires here.
		 */
		mb();
		queue_work(msm_uport->hsuart_wq, &msm_uport->clock_off_w);
	}

	/* Change in CTS interrupt */
	if (isr_status & UARTDM_ISR_DELTA_CTS_BMSK)
		msm_hs_handle_delta_cts_locked(uport);

	spin_unlock_irqrestore(&uport->lock, flags);

	return IRQ_HANDLED;
}

/* request to turn off uart clock once pending TX is flushed */
void msm_hs_request_clock_off(struct uart_port *uport) {
	unsigned long flags;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	spin_lock_irqsave(&uport->lock, flags);
	if (msm_uport->clk_state == MSM_HS_CLK_ON) {
		msm_uport->clk_state = MSM_HS_CLK_REQUEST_OFF;
		msm_uport->clk_req_off_state = CLK_REQ_OFF_START;
		msm_uport->imr_reg |= UARTDM_ISR_TXLEV_BMSK;
		msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
		/*
		 * Complete device write before retuning back.
		 * Hence mb() requires here.
		 */
		mb();
	}
	spin_unlock_irqrestore(&uport->lock, flags);
}
EXPORT_SYMBOL(msm_hs_request_clock_off);

void msm_hs_request_clock_on(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	unsigned long flags;
	unsigned int data;
	int ret = 0;

	if(msm_uport->is_shutdown) {
		printk(KERN_INFO "(msm_serial_hs) msm_hs_request_clock_on - uart shutdown, so return..\n");
		return;
	}

	mutex_lock(&msm_uport->clk_mutex);
	spin_lock_irqsave(&uport->lock, flags);

	switch (msm_uport->clk_state) {
	case MSM_HS_CLK_OFF:
		printk(KERN_INFO "(msm_serial_hs) msm_hs_check_clock_on - dma wake lock\n");
		wake_lock(&msm_uport->dma_wake_lock);
		disable_irq_nosync(msm_uport->wakeup.irq);
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = msm_hs_clock_vote(msm_uport);
		if (ret) {
			dev_err(uport->dev, "Clock ON Failure"
			"For UART CLK Stalling HSUART\n");
			wake_unlock(&msm_uport->dma_wake_lock);
			break;
		}

		spin_lock_irqsave(&uport->lock, flags);
		/* else fall-through */
	case MSM_HS_CLK_REQUEST_OFF:
		if (msm_uport->rx.flush == FLUSH_STOP ||
		    msm_uport->rx.flush == FLUSH_SHUTDOWN) {
			msm_hs_write(uport, UARTDM_CR_ADDR, RESET_RX);
			data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
			data |= UARTDM_RX_DM_EN_BMSK;
			msm_hs_write(uport, UARTDM_DMEN_ADDR, data);
			/* Complete above device write. Hence mb() here. */
			mb();
		}
		hrtimer_try_to_cancel(&msm_uport->clk_off_timer);
		if (msm_uport->rx.flush == FLUSH_SHUTDOWN)
			msm_hs_start_rx_locked(uport);
		if (msm_uport->rx.flush == FLUSH_STOP)
			msm_uport->rx.flush = FLUSH_IGNORE;
		msm_uport->clk_state = MSM_HS_CLK_ON;
		break;
	case MSM_HS_CLK_ON:
		break;
	case MSM_HS_CLK_PORT_OFF:
		break;
	}

	if (!ret)
		spin_unlock_irqrestore(&uport->lock, flags);
	mutex_unlock(&msm_uport->clk_mutex);
}
EXPORT_SYMBOL(msm_hs_request_clock_on);

static irqreturn_t msm_hs_wakeup_isr(int irq, void *dev)
{
	unsigned int wakeup = 0;
	unsigned long flags;
	struct msm_hs_port *msm_uport = (struct msm_hs_port *)dev;
	struct uart_port *uport = &msm_uport->uport;
	struct tty_struct *tty = NULL;

	spin_lock_irqsave(&uport->lock, flags);
	if (msm_uport->clk_state == MSM_HS_CLK_OFF)  {
		/* ignore the first irq - it is a pending irq that occured
		 * before enable_irq() */
		if (msm_uport->wakeup.ignore)
			msm_uport->wakeup.ignore = 0;
		else
			wakeup = 1;
	}

	if (wakeup) {
		/* the uart was clocked off during an rx, wake up and
		 * optionally inject char into tty rx */
		spin_unlock_irqrestore(&uport->lock, flags);
		msm_hs_request_clock_on(uport);
		spin_lock_irqsave(&uport->lock, flags);
		if (msm_uport->wakeup.inject_rx) {
			tty = uport->state->port.tty;
			tty_insert_flip_char(tty,
					     msm_uport->wakeup.rx_to_inject,
					     TTY_NORMAL);
		}
	}

	spin_unlock_irqrestore(&uport->lock, flags);

	if (wakeup && msm_uport->wakeup.inject_rx)
		tty_flip_buffer_push(tty);
	return IRQ_HANDLED;
}

static const char *msm_hs_type(struct uart_port *port)
{
	return ("MSM HS UART");
}

/* Called when port is opened */
static int msm_hs_startup(struct uart_port *uport)
{
	int ret;
	int rfr_level;
	unsigned long flags;
	unsigned int data;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
					pdev->dev.platform_data;
	struct circ_buf *tx_buf = &uport->state->xmit;
	struct msm_hs_tx *tx = &msm_uport->tx;
	struct msm_hs_rx *rx = &msm_uport->rx;

	msm_uport->is_shutdown = false;
	msm_uport->termios_in_progress = false;

	rfr_level = uport->fifosize;
	if (rfr_level > 16)
		rfr_level -= 16;

	tx->dma_base = dma_map_single(uport->dev, tx_buf->buf, UART_XMIT_SIZE,
				      DMA_TO_DEVICE);

	printk(KERN_INFO "(msm_serial_hs) msm_hs_startup - dma wake lock\n");
	wake_lock(&msm_uport->dma_wake_lock);
	/* turn on uart clk */
	ret = msm_hs_init_clk(uport);
	if (unlikely(ret)) {
		pr_err("Turning ON uartclk error\n");
		wake_unlock(&msm_uport->dma_wake_lock);
		return ret;
	}

	/* Stop remote UART to send data by setting RFR GPIO to LOW. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_HIGH);


	if (pdata && pdata->config_gpio) {
		ret = msm_hs_config_uart_gpios(uport);
		if (ret)
			goto deinit_uart_clk;
	} else {
		pr_debug("%s(): UART GPIOs not specified.\n", __func__);
	}

	/* Set auto RFR Level */
	data = msm_hs_read(uport, UARTDM_MR1_ADDR);
	data &= ~UARTDM_MR1_AUTO_RFR_LEVEL1_BMSK;
	data &= ~UARTDM_MR1_AUTO_RFR_LEVEL0_BMSK;
	data |= (UARTDM_MR1_AUTO_RFR_LEVEL1_BMSK & (rfr_level << 2));
	data |= (UARTDM_MR1_AUTO_RFR_LEVEL0_BMSK & rfr_level);
	msm_hs_write(uport, UARTDM_MR1_ADDR, data);

	/* Make sure RXSTALE count is non-zero */
	data = msm_hs_read(uport, UARTDM_IPR_ADDR);
	if (!data) {
		data |= 0x1f & UARTDM_IPR_STALE_LSB_BMSK;
		msm_hs_write(uport, UARTDM_IPR_ADDR, data);
	}

	/* Enable Data Mover Mode */
	data = UARTDM_TX_DM_EN_BMSK | UARTDM_RX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);


	/* Reset both RX and TX HW state machine */
	msm_hs_write(uport, UARTDM_CR_ADDR, (RESET_RX | RESET_TX));
	/*
	 * Rx and Tx reset operation takes few clock cycles, hence as
	 * safe side adding 10us delay.
	 */
	udelay(10);

	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_ERROR_STATUS);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_BREAK_INT);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_STALE_INT);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_CTS);
	/* Turn on Uart Receiver */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_RX_EN_BMSK);

	/* Turn on Uart Transmitter */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_TX_EN_BMSK);

	/* Initialize the tx */
	tx->tx_ready_int_en = 0;
	tx->dma_in_flight = 0;
	rx->dma_in_flight = false;
	rx->cmd_exec = false;

	tx->xfer.complete_func = msm_hs_dmov_tx_callback;

	tx->command_ptr->cmd = CMD_LC |
	    CMD_DST_CRCI(msm_uport->dma_tx_crci) | CMD_MODE_BOX;

	tx->command_ptr->src_dst_len = (MSM_UARTDM_BURST_SIZE << 16)
					   | (MSM_UARTDM_BURST_SIZE);

	tx->command_ptr->row_offset = (MSM_UARTDM_BURST_SIZE << 16);

	tx->command_ptr->dst_row_addr =
	    msm_uport->uport.mapbase + UARTDM_TF_ADDR;

	msm_uport->imr_reg |= UARTDM_ISR_RXSTALE_BMSK;
	/* Enable reading the current CTS, no harm even if CTS is ignored */
	msm_uport->imr_reg |= UARTDM_ISR_CURRENT_CTS_BMSK;

	msm_hs_write(uport, UARTDM_TFWR_ADDR, 0);  /* TXLEV on empty TX fifo */
	/*
	 * Complete all device write related configuration before
	 * queuing RX request. Hence mb() requires here.
	 */
	mb();

	if (use_low_power_wakeup(msm_uport)) {
		ret = irq_set_irq_wake(msm_uport->wakeup.irq, 1);
		if (unlikely(ret)) {
			pr_err("%s():Err setting wakeup irq\n", __func__);
			goto unconfigure_uart_gpio;
		}
	}

	ret = request_irq(uport->irq, msm_hs_isr, IRQF_TRIGGER_HIGH,
			  "msm_hs_uart", msm_uport);
	if (unlikely(ret)) {
		pr_err("%s():Error getting uart irq\n", __func__);
		goto free_wake_irq;
	}
	if (use_low_power_wakeup(msm_uport)) {

		ret = request_threaded_irq(msm_uport->wakeup.irq, NULL,
					msm_hs_wakeup_isr,
					IRQF_TRIGGER_FALLING,
					"msm_hs_wakeup", msm_uport);

		if (unlikely(ret)) {
			pr_err("%s():Err getting uart wakeup_irq\n", __func__);
			goto free_uart_irq;
		}
		disable_irq(msm_uport->wakeup.irq);
	}

	spin_lock_irqsave(&uport->lock, flags);

	msm_hs_start_rx_locked(uport);

	/* Allow remote UART to send data by setting RFR GPIO to HIGH. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_LOW);

	spin_unlock_irqrestore(&uport->lock, flags);

	pm_runtime_enable(uport->dev);

	return 0;

free_uart_irq:
	free_irq(uport->irq, msm_uport);
free_wake_irq:
	irq_set_irq_wake(msm_uport->wakeup.irq, 0);
unconfigure_uart_gpio:
	if (pdata && pdata->config_gpio)
		msm_hs_unconfig_uart_gpios(uport);
deinit_uart_clk:
	msm_hs_clock_unvote(msm_uport);

	printk(KERN_INFO "(msm_serial_hs) msm_hs_startup deinit clk - dma wake unlock\n");

	wake_unlock(&msm_uport->dma_wake_lock);

	return ret;
}

/* Initialize tx and rx data structures */
static int uartdm_init_port(struct uart_port *uport)
{
	int ret = 0;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct msm_hs_tx *tx = &msm_uport->tx;
	struct msm_hs_rx *rx = &msm_uport->rx;

	/* Allocate the command pointer. Needs to be 64 bit aligned */
	tx->command_ptr = kmalloc(sizeof(dmov_box), GFP_KERNEL | __GFP_DMA);
	if (!tx->command_ptr)
		return -ENOMEM;

	tx->command_ptr_ptr = kmalloc(sizeof(u32), GFP_KERNEL | __GFP_DMA);
	if (!tx->command_ptr_ptr) {
		ret = -ENOMEM;
		goto free_tx_command_ptr;
	}

	tx->mapped_cmd_ptr = dma_map_single(uport->dev, tx->command_ptr,
					    sizeof(dmov_box), DMA_TO_DEVICE);
	tx->mapped_cmd_ptr_ptr = dma_map_single(uport->dev,
						tx->command_ptr_ptr,
						sizeof(u32), DMA_TO_DEVICE);
	tx->xfer.cmdptr = DMOV_CMD_ADDR(tx->mapped_cmd_ptr_ptr);

	init_waitqueue_head(&rx->wait);
	init_waitqueue_head(&tx->wait);
	wake_lock_init(&rx->wake_lock, WAKE_LOCK_SUSPEND, "msm_serial_hs_rx");
	wake_lock_init(&msm_uport->dma_wake_lock, WAKE_LOCK_SUSPEND,
		       "msm_serial_hs_dma");

	tasklet_init(&rx->tlet, msm_serial_hs_rx_tlet,
			(unsigned long) &rx->tlet);
	tasklet_init(&tx->tlet, msm_serial_hs_tx_tlet,
			(unsigned long) &tx->tlet);

	rx->pool = dma_pool_create("rx_buffer_pool", uport->dev,
				   msm_uport->rx_buf_size, 16, 0);
	if (!rx->pool) {
		pr_err("%s(): cannot allocate rx_buffer_pool", __func__);
		ret = -ENOMEM;
		goto exit_tasket_init;
	}

	rx->buffer = dma_pool_alloc(rx->pool, GFP_KERNEL, &rx->rbuffer);
	if (!rx->buffer) {
		pr_err("%s(): cannot allocate rx->buffer", __func__);
		ret = -ENOMEM;
		goto free_pool;
	}

	/* Allocate the command pointer. Needs to be 64 bit aligned */
	rx->command_ptr = kmalloc(sizeof(dmov_box), GFP_KERNEL | __GFP_DMA);
	if (!rx->command_ptr) {
		pr_err("%s(): cannot allocate rx->command_ptr", __func__);
		ret = -ENOMEM;
		goto free_rx_buffer;
	}

	rx->command_ptr_ptr = kmalloc(sizeof(u32), GFP_KERNEL | __GFP_DMA);
	if (!rx->command_ptr_ptr) {
		pr_err("%s(): cannot allocate rx->command_ptr_ptr", __func__);
		ret = -ENOMEM;
		goto free_rx_command_ptr;
	}

	rx->command_ptr->num_rows = ((msm_uport->rx_buf_size >> 4) << 16) |
					 (msm_uport->rx_buf_size >> 4);

	rx->command_ptr->dst_row_addr = rx->rbuffer;

	/* Set up Uart Receive */
	msm_hs_write(uport, UARTDM_RFWR_ADDR, 0);

	rx->xfer.complete_func = msm_hs_dmov_rx_callback;
	rx->xfer.exec_func = msm_hs_dmov_rx_exec_callback;

	rx->command_ptr->cmd = CMD_LC |
	    CMD_SRC_CRCI(msm_uport->dma_rx_crci) | CMD_MODE_BOX;

	rx->command_ptr->src_dst_len = (MSM_UARTDM_BURST_SIZE << 16)
					   | (MSM_UARTDM_BURST_SIZE);
	rx->command_ptr->row_offset =  MSM_UARTDM_BURST_SIZE;
	rx->command_ptr->src_row_addr = uport->mapbase + UARTDM_RF_ADDR;

	rx->mapped_cmd_ptr = dma_map_single(uport->dev, rx->command_ptr,
					    sizeof(dmov_box), DMA_TO_DEVICE);

	*rx->command_ptr_ptr = CMD_PTR_LP | DMOV_CMD_ADDR(rx->mapped_cmd_ptr);

	rx->cmdptr_dmaaddr = dma_map_single(uport->dev, rx->command_ptr_ptr,
					    sizeof(u32), DMA_TO_DEVICE);
	rx->xfer.cmdptr = DMOV_CMD_ADDR(rx->cmdptr_dmaaddr);

	INIT_DELAYED_WORK(&rx->flip_insert_work, flip_insert_work);

	return ret;

free_rx_command_ptr:
	kfree(rx->command_ptr);

free_rx_buffer:
	dma_pool_free(msm_uport->rx.pool, msm_uport->rx.buffer,
			msm_uport->rx.rbuffer);

free_pool:
	dma_pool_destroy(msm_uport->rx.pool);

exit_tasket_init:
	wake_lock_destroy(&msm_uport->rx.wake_lock);
	wake_lock_destroy(&msm_uport->dma_wake_lock);
	tasklet_kill(&msm_uport->tx.tlet);
	tasklet_kill(&msm_uport->rx.tlet);
	dma_unmap_single(uport->dev, msm_uport->tx.mapped_cmd_ptr_ptr,
			sizeof(u32), DMA_TO_DEVICE);
	dma_unmap_single(uport->dev, msm_uport->tx.mapped_cmd_ptr,
			sizeof(dmov_box), DMA_TO_DEVICE);
	kfree(msm_uport->tx.command_ptr_ptr);

free_tx_command_ptr:
	kfree(msm_uport->tx.command_ptr);
	return ret;
}

static int __devinit msm_hs_probe(struct platform_device *pdev)
{
	int ret;
	struct uart_port *uport;
	struct msm_hs_port *msm_uport;
	unsigned int data;
	struct resource *resource;
	struct msm_serial_hs_platform_data *pdata = pdev->dev.platform_data;

	if (pdev->id < 0 || pdev->id >= UARTDM_NR) {
		printk(KERN_ERR "Invalid plaform device ID = %d\n", pdev->id);
		return -EINVAL;
	}

	msm_uport = &q_uart_port[pdev->id];
	uport = &msm_uport->uport;

	uport->dev = &pdev->dev;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	uport->mapbase = resource->start;  /* virtual address */

	uport->membase = ioremap(uport->mapbase, PAGE_SIZE);
	if (unlikely(!uport->membase))
		return -ENOMEM;

	uport->irq = platform_get_irq(pdev, 0);
	if (unlikely((int)uport->irq < 0))
		return -ENXIO;

	if (pdata == NULL)
		msm_uport->wakeup.irq = -1;
	else {
		msm_uport->wakeup.irq = pdata->wakeup_irq;
		msm_uport->wakeup.ignore = 1;
		msm_uport->wakeup.inject_rx = pdata->inject_rx_on_wakeup;
		msm_uport->wakeup.rx_to_inject = pdata->rx_to_inject;

		if (unlikely(msm_uport->wakeup.irq < 0))
			return -ENXIO;

	}

	/* Identify UART functional mode as 2-wire or 4-wire. */
	if (pdata && pdata->config_gpio) {
		switch (pdata->config_gpio) {
		case 4:
			if (gpio_is_valid(pdata->uart_tx_gpio)
				&& gpio_is_valid(pdata->uart_rx_gpio)
				&& gpio_is_valid(pdata->uart_cts_gpio)
				&& gpio_is_valid(pdata->uart_rfr_gpio)) {
					msm_uport->func_mode = UART_FOUR_WIRE;
			} else {
				pr_err("%s(): Wrong GPIO Number for 4-Wire.\n",
								__func__);
				return -EINVAL;
			}
			break;
		case 2:
			if (gpio_is_valid(pdata->uart_tx_gpio)
				&& gpio_is_valid(pdata->uart_rx_gpio)) {
					msm_uport->func_mode = UART_TWO_WIRE;
			} else {
				pr_err("%s(): Wrong GPIO Number for 2-Wire.\n",
								__func__);
				return -EINVAL;
			}
			break;
		default:
			pr_err("%s(): Invalid number of GPIOs.\n", __func__);
			pdata->config_gpio = 0;
			return -EINVAL;
		}
	}

	if (pdata && pdata->uartdm_rx_buf_size)
		msm_uport->rx_buf_size = pdata->uartdm_rx_buf_size;
	else
		msm_uport->rx_buf_size = UARTDM_RX_BUF_SIZE;

	resource = platform_get_resource_byname(pdev, IORESOURCE_DMA,
						"uartdm_channels");
	if (unlikely(!resource))
		return -ENXIO;
	msm_uport->dma_tx_channel = resource->start;
	msm_uport->dma_rx_channel = resource->end;

	resource = platform_get_resource_byname(pdev, IORESOURCE_DMA,
						"uartdm_crci");
	if (unlikely(!resource))
		return -ENXIO;
	msm_uport->dma_tx_crci = resource->start;
	msm_uport->dma_rx_crci = resource->end;

	uport->iotype = UPIO_MEM;
	uport->fifosize = 64;
	uport->ops = &msm_hs_ops;
	uport->flags = UPF_BOOT_AUTOCONF;
	uport->uartclk = 7372800;
	msm_uport->imr_reg = 0x0;

	msm_uport->clk = clk_get(&pdev->dev, "core_clk");
	if (IS_ERR(msm_uport->clk))
		return PTR_ERR(msm_uport->clk);

	msm_uport->pclk = clk_get(&pdev->dev, "iface_clk");
	/*
	 * Some configurations do not require explicit pclk control so
	 * do not flag error on pclk get failure.
	 */
	if (IS_ERR(msm_uport->pclk))
		msm_uport->pclk = NULL;

	ret = clk_set_rate(msm_uport->clk, uport->uartclk);
	if (ret) {
		printk(KERN_WARNING "Error setting clock rate on UART\n");
		return ret;
	}

	msm_uport->hsuart_wq = alloc_workqueue("k_hsuart",
					WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
	if (!msm_uport->hsuart_wq) {
		pr_err("%s(): Unable to create workqueue hsuart_wq\n",
								__func__);
		return -ENOMEM;
	}

	INIT_WORK(&msm_uport->clock_off_w, hsuart_clock_off_work);
	mutex_init(&msm_uport->clk_mutex);
	atomic_set(&msm_uport->clk_count, 0);

	msm_hs_clock_vote(msm_uport);

	ret = uartdm_init_port(uport);
	if (unlikely(ret)) {
		msm_hs_clock_unvote(msm_uport);
		return ret;
	}

	/* configure the CR Protection to Enable */
	msm_hs_write(uport, UARTDM_CR_ADDR, CR_PROTECTION_EN);
	/*
	 * Enable Command register protection before going ahead as this hw
	 * configuration makes sure that issued cmd to CR register gets complete
	 * before next issued cmd start. Hence mb() requires here.
	 */
	mb();

	/*
	 * Set RX_BREAK_ZERO_CHAR_OFF and RX_ERROR_CHAR_OFF
	 * so any rx_break and character having parity of framing
	 * error don't enter inside UART RX FIFO.
	 */
	data = msm_hs_read(uport, UARTDM_MR2_ADDR);
	data |= (UARTDM_MR2_RX_BREAK_ZERO_CHAR_OFF |
			UARTDM_MR2_RX_ERROR_CHAR_OFF);
	msm_hs_write(uport, UARTDM_MR2_ADDR, data);
	mb();
	msm_hs_clock_unvote(msm_uport);

	msm_uport->clk_state = MSM_HS_CLK_PORT_OFF;
	hrtimer_init(&msm_uport->clk_off_timer, CLOCK_MONOTONIC,
		     HRTIMER_MODE_REL);
	msm_uport->clk_off_timer.function = msm_hs_clk_off_retry;
	msm_uport->clk_off_delay = ktime_set(0, 1000000);  /* 1ms */

	ret = sysfs_create_file(&pdev->dev.kobj, &dev_attr_clock.attr);
	if (unlikely(ret))
		return ret;

	msm_serial_debugfs_init(msm_uport, pdev->id);

	uport->line = pdev->id;
	if (pdata != NULL && pdata->userid && pdata->userid <= UARTDM_NR)
		uport->line = pdata->userid;
	return uart_add_one_port(&msm_hs_driver, uport);
}

static int __init msm_serial_hs_init(void)
{
	int ret;
	int i;

	/* Init all UARTS as non-configured */
	for (i = 0; i < UARTDM_NR; i++)
		q_uart_port[i].uport.type = PORT_UNKNOWN;

	ret = uart_register_driver(&msm_hs_driver);
	if (unlikely(ret)) {
		printk(KERN_ERR "%s failed to load\n", __FUNCTION__);
		return ret;
	}
	debug_base = debugfs_create_dir("msm_serial_hs", NULL);
	if (IS_ERR_OR_NULL(debug_base))
		pr_info("msm_serial_hs: Cannot create debugfs dir\n");

	ret = platform_driver_register(&msm_serial_hs_platform_driver);
	if (ret) {
		printk(KERN_ERR "%s failed to load\n", __FUNCTION__);
		debugfs_remove_recursive(debug_base);
		uart_unregister_driver(&msm_hs_driver);
		return ret;
	}

	printk(KERN_INFO "msm_serial_hs module loaded\n");
	return ret;
}

/*
 *  Called by the upper layer when port is closed.
 *     - Disables the port
 *     - Unhook the ISR
 */
static void msm_hs_shutdown(struct uart_port *uport)
{
	int ret;
	unsigned int data;
	unsigned long flags;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
				pdev->dev.platform_data;


	/* deactivate if any clock off hrtimer is active. */
	hrtimer_try_to_cancel(&msm_uport->clk_off_timer);

	if (msm_uport->clk_state == MSM_HS_CLK_OFF)
		msm_hs_clock_vote(msm_uport);

	spin_lock_irqsave(&uport->lock, flags);
	/* disable UART TX interface to DM */
	data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
	data &= ~UARTDM_TX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);
	mb();

	if (msm_uport->tx.dma_in_flight) {
		msm_uport->tx.flush = FLUSH_STOP;
		/* discard flush */
		msm_dmov_flush(msm_uport->dma_tx_channel, 0);
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->tx.wait,
			msm_uport->tx.flush == FLUSH_SHUTDOWN, 100);
		if (!ret) {
			pr_err("%s():HSUART TX Stalls.\n", __func__);
			print_uart_registers(msm_uport);
	}
	} else {
		spin_unlock_irqrestore(&uport->lock, flags);
	}

	tasklet_kill(&msm_uport->tx.tlet);

	if (msm_uport->rx.dma_in_flight) {

		if (msm_uport->rx.flush < FLUSH_STOP) {
			pr_err("%s(): rx.flush is not correct.\n",
							__func__);
			print_uart_registers(msm_uport);
			BUG_ON(1);
		}

		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx.flush == FLUSH_SHUTDOWN,
			RX_FLUSH_COMPLETE_TIMEOUT);
		if (!ret) {
			pr_err("%s(): Rx completion failed.\n", __func__);
			print_uart_registers(msm_uport);
		}
	}

	tasklet_kill(&msm_uport->rx.tlet);

	spin_lock_irqsave(&uport->lock, flags);
	/* Disable all UART interrupts */
	msm_uport->imr_reg = 0;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);

	/* Free the UART IRQ line */
	free_irq(uport->irq, msm_uport);

	msm_uport->is_shutdown = true;
	spin_unlock_irqrestore(&uport->lock, flags);

	/* disable UART RX interface to DM */
	data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
	data &= ~UARTDM_RX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);

	cancel_delayed_work_sync(&msm_uport->rx.flip_insert_work);
	flush_workqueue(msm_uport->hsuart_wq);
	pm_runtime_disable(uport->dev);

	/* Disable the transmitter */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_TX_DISABLE_BMSK);
	/* Disable the receiver */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_RX_DISABLE_BMSK);

	/*
	 * Complete all device write before actually disabling uartclk.
	 * Hence mb() requires here.
	 */
	mb();

	if (msm_uport->clk_state != MSM_HS_CLK_OFF) {
		msm_hs_clock_unvote(msm_uport);
		printk(KERN_INFO "(msm_serial_hs) msm_hs_shutdown - dma wake unlock\n");
		wake_unlock(&msm_uport->dma_wake_lock);
	}
	dma_unmap_single(uport->dev, msm_uport->tx.dma_base,
			 UART_XMIT_SIZE, DMA_TO_DEVICE);

	if (use_low_power_wakeup(msm_uport))
		irq_set_irq_wake(msm_uport->wakeup.irq, 0);

	/* Free the interrupt */
	if (use_low_power_wakeup(msm_uport))
		free_irq(msm_uport->wakeup.irq, msm_uport);

	if (pdata && pdata->config_gpio)
			msm_hs_unconfig_uart_gpios(uport);
}

static void __exit msm_serial_hs_exit(void)
{
	printk(KERN_INFO "msm_serial_hs module removed\n");
	debugfs_remove_recursive(debug_base);
	platform_driver_unregister(&msm_serial_hs_platform_driver);
	uart_unregister_driver(&msm_hs_driver);
}

#if 0 /*Bluesleep manages uart clk control*/
static int msm_hs_runtime_idle(struct device *dev)
{
	/*
	 * returning success from idle results in runtime suspend to be
	 * called
	 */
	return 0;
}

static int msm_hs_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = container_of(dev, struct
						    platform_device, dev);
	struct msm_hs_port *msm_uport = &q_uart_port[pdev->id];
	msm_hs_request_clock_on(&msm_uport->uport);
	return 0;
}

static int msm_hs_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = container_of(dev, struct
						    platform_device, dev);
	struct msm_hs_port *msm_uport = &q_uart_port[pdev->id];

	printk(KERN_INFO "(msm_serial_hs) msm_hs_runtime_suspend.\n");

	msm_hs_request_clock_off(&msm_uport->uport);
	return 0;
}

static const struct dev_pm_ops msm_hs_dev_pm_ops = {
	.runtime_suspend = msm_hs_runtime_suspend,
	.runtime_resume  = msm_hs_runtime_resume,
	.runtime_idle    = msm_hs_runtime_idle,
};
#endif

static struct platform_driver msm_serial_hs_platform_driver = {
	.probe	= msm_hs_probe,
	.remove = __devexit_p(msm_hs_remove),
	.driver = {
		.name = "msm_serial_hs",
		/*Bluesleep manages uart clk control*/
		/*.pm   = &msm_hs_dev_pm_ops,*/
	},
};

static struct uart_driver msm_hs_driver = {
	.owner = THIS_MODULE,
	.driver_name = "msm_serial_hs",
	.dev_name = "ttyHS",
	.nr = UARTDM_NR,
	.cons = 0,
};

static struct uart_ops msm_hs_ops = {
	.tx_empty = msm_hs_tx_empty,
	.set_mctrl = msm_hs_set_mctrl_locked,
	.get_mctrl = msm_hs_get_mctrl_locked,
	.stop_tx = msm_hs_stop_tx_locked,
	.start_tx = msm_hs_start_tx_locked,
	.stop_rx = msm_hs_stop_rx_locked,
	.enable_ms = msm_hs_enable_ms_locked,
	.break_ctl = msm_hs_break_ctl,
	.startup = msm_hs_startup,
	.shutdown = msm_hs_shutdown,
	.set_termios = msm_hs_set_termios,
	.type = msm_hs_type,
	.config_port = msm_hs_config_port,
	.release_port = msm_hs_release_port,
	.request_port = msm_hs_request_port,
	.flush_buffer = msm_hs_flush_buffer_locked,
	.pm = msm_hs_power,
};

module_init(msm_serial_hs_init);
module_exit(msm_serial_hs_exit);
MODULE_DESCRIPTION("High Speed UART Driver for the MSM chipset");
MODULE_VERSION("1.2");
MODULE_LICENSE("GPL v2");
