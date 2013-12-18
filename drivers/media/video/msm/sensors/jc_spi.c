/*
 * driver for S5C73M3 SPI
 *
 * Copyright (c) 2011, Samsung Electronics. All rights reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>

#define cam_err(fmt, arg...)	\
	do {					\
		printk(KERN_ERR "[%s:%d] " fmt ,	\
			__func__, __LINE__, ##arg);		\
	}							\
	while (0)
#define SWAP32(l) \
	(((((l) & 0xff000000) >> 24) | \
	(((l) & 0x00ff0000) >> 8)  | \
	(((l) & 0x0000ff00) << 8)  | \
	(((l) & 0x000000ff) << 24)))

static struct spi_device *g_spi;

/* If AP can't change the endian to BIG */
/* for s5c73m ISP, this option must is required.*/
/* This option depends on SPI_DMA_MODE */
/* in camera driver file*/
/*#define CHANGE_ENDIAN	*/

#if 0
static inline
int spi_xmit(const u8 *addr, const int len)
{
	int ret;
#if defined(CHANGE_ENDIAN)
	u8 buf[8] = {0};
#endif

	struct spi_message msg;

	struct spi_transfer xfer = {
		.len = len,
#if !defined(CHANGE_ENDIAN)
		.tx_buf = addr,
		/*QCTK ALRAN QUP_CONFIG 0-4 bits BIG ENDIAN*/
		.bits_per_word = 32,
#else
		.tx_buf = buf,
#endif
	};

#if defined(CHANGE_ENDIAN)
	buf[0] = addr[3];
	buf[1] = addr[2];
	buf[2] = addr[1];
	buf[3] = addr[0];

	buf[4] = addr[7];
	buf[5] = addr[6];
	buf[6] = addr[5];
	buf[7] = addr[4];
#endif

	spi_message_init(&msg);
	spi_message_add_tail(&xfer, &msg);

	ret = spi_sync(g_spi, &msg);

	if (ret < 0)
		cam_err("error %d\n", ret);

	return ret;
}
#else // for DMA write

#define BUF_SIZE_FOR_SPI 2048
/*static inline*/
int spi_xmit(const u8 *addr,const int len)
{
	struct spi_message msg;
	int ret =-1;
	void *buf_for_tx = NULL;
	int duplicate_len = len;
	int check_len = 0;
	struct spi_transfer *xfer;
	u64 mask;
	struct device *dev = &(g_spi->master->dev);
	dma_addr_t dma_handle1;
	int change_endian_count = 0;
	unsigned long *buf_for_tx_trans;
	unsigned long *addr_trans;
	mask = DMA_BIT_MASK(32);
	dev->dma_mask = &mask;
	dev->coherent_dma_mask = DMA_BIT_MASK(32);
	buf_for_tx = dma_alloc_coherent(dev, BUF_SIZE_FOR_SPI, &dma_handle1, GFP_KERNEL);
	xfer = kmalloc(sizeof(struct spi_transfer),GFP_KERNEL);
	xfer->speed_hz = 11000000;//1100000;
	xfer->bits_per_word = 32;
	xfer->cs_change = 0;
	xfer->delay_usecs = 0;
	xfer->len = BUF_SIZE_FOR_SPI;
	xfer->tx_dma = dma_handle1;
	xfer->rx_dma = 0;
	xfer->tx_buf = buf_for_tx;
	xfer->rx_buf = 0;
	g_spi->mode |= 0x20;
	spi_setup(g_spi);
	buf_for_tx_trans = (unsigned long *)buf_for_tx;
	addr_trans = (unsigned long *)addr;
	while(duplicate_len !=0 ) {
		if(duplicate_len > BUF_SIZE_FOR_SPI) {

			if (!buf_for_tx) {
				printk("Error allocating DMA memory\n");
				kfree(xfer);
				return 0;
			}
			while(change_endian_count != (BUF_SIZE_FOR_SPI/4)) {
				*(buf_for_tx_trans+change_endian_count) =
					SWAP32(*(addr_trans+(check_len*BUF_SIZE_FOR_SPI/4)+change_endian_count));
				change_endian_count = change_endian_count+1;
			}
			change_endian_count = 0;
			spi_message_init(&msg);
			msg.is_dma_mapped=1;
			spi_message_add_tail(xfer, &msg);
			ret = spi_sync(g_spi, &msg);
			if (ret < 0)
				printk("spi_sync ret: %d\n",ret);
			duplicate_len = duplicate_len - BUF_SIZE_FOR_SPI;
			check_len = check_len + 1;
		} else {

			if (!buf_for_tx) {
				printk("Error allocating DMA memory\n");
				kfree(xfer);
				return 0;
			}

			change_endian_count = 0;
			xfer->speed_hz = 11000000;//1100000;
			xfer->bits_per_word = 32;
			xfer->cs_change = 0;
			xfer->delay_usecs = 0;
			xfer->len = duplicate_len;
			xfer->tx_dma = dma_handle1;
			xfer->rx_dma = 0;
			xfer->tx_buf = buf_for_tx;
			xfer->rx_buf = 0;
			g_spi->mode |= 0x20;

			while(change_endian_count != (duplicate_len/4)) {
				*(buf_for_tx_trans+change_endian_count) =
					SWAP32(*(addr_trans+(check_len*BUF_SIZE_FOR_SPI/4)+change_endian_count));
				change_endian_count = change_endian_count+1;
			}
			change_endian_count = 0;
			spi_setup(g_spi);

			spi_message_init(&msg);
			msg.is_dma_mapped=1;
			spi_message_add_tail(xfer, &msg);
			ret = spi_sync(g_spi, &msg);
			if (ret < 0)
				printk("spi_sync ret: %d\n",ret);
			duplicate_len = 0;
		}
	}
	dma_free_coherent(dev,BUF_SIZE_FOR_SPI,buf_for_tx,dma_handle1);
	buf_for_tx = 0;
	kfree(xfer);
	xfer = 0;
	return ret;

}
#endif
static inline
int spi_xmit_rx(u8 *in_buf, size_t len)
{
	int ret;
	u8 read_out_buf[2];

	struct spi_message msg;
	struct spi_transfer xfer = {
		.tx_buf = read_out_buf,
		.rx_buf = in_buf,
		.len	= len,
		.cs_change = 0,
	};

	spi_message_init(&msg);

	spi_message_add_tail(&xfer, &msg);

	ret = spi_sync(g_spi, &msg);

	if (ret < 0)
		cam_err("%s - error %d\n",
				__func__, ret);

	return ret;
}

int jc_spi_read(u8 *buf, size_t len, const int rxSize)
{
	int k;
	int ret = 0;
	u8 temp_buf[4] = {0};
	u32 count = len/rxSize;
	u32 extra = len%rxSize;

	for (k = 0; k < count; k++) {
		ret = spi_xmit_rx(&buf[rxSize*k], rxSize);
		if (ret < 0) {
			cam_err("%s - error %d\n",
				__func__, ret);
			return -EINVAL;
		}
	}

	if (extra != 0) {
		ret = spi_xmit_rx(&buf[rxSize*k], extra);
		if (ret < 0) {
			cam_err("%s - error %d\n",
				__func__, ret);
			return -EINVAL;
		}
	}

	for (k = 0; k < len-3; k += 4) {
		memcpy(temp_buf, (char *)&buf[k], sizeof(temp_buf));
		buf[k] = temp_buf[3];
		buf[k+1] = temp_buf[2];
		buf[k+2] = temp_buf[1];
		buf[k+3] = temp_buf[0];
	}

	return 0;
}

int jc_spi_write(const u8 *addr, const int len, const int txSize)
{
	int i, j = 0;
	int ret = 0;
	u8 paddingData[8];
	u32 count = len/txSize;
	u32 extra = len%txSize;
	cam_err("Entered\n");
	cam_err("count = %d extra = %d\n", count, extra);

	memset(paddingData, 0, sizeof(paddingData));

	for (i = 0 ; i < count ; i++) {
		ret = spi_xmit(&addr[j], txSize);
		j += txSize;
		if (ret < 0) {
			cam_err("failed to write spi_xmit\n");
			goto exit_err;
		}
		cam_err("Delay!!!\n");
		usleep(50*1000);
	}

	if (extra) {
		ret = spi_xmit(&addr[j], extra);
		if (ret < 0) {
			cam_err("failed to write spi_xmit\n");
			goto exit_err;
		}
	}

	for (i = 0; i < 4; i++) {
		memset(paddingData, 0, sizeof(paddingData));
		ret = spi_xmit(paddingData, 8);
		if (ret < 0) {
			cam_err("failed to write spi_xmit\n");
			goto exit_err;
		}
	}
	cam_err("Finish!!\n");
exit_err:
	return ret;
}

static int jc_spi_probe(struct spi_device *spi)
{
	int ret;

	cam_err("Entered\n");

	if (spi_setup(spi)) {
		cam_err("failed to setup spi for jc_spi\n");
		ret = -EINVAL;
		goto err_setup;
	}

	g_spi = spi;

	cam_err("jc_spi successfully probed\n");

	return 0;

err_setup:
	return ret;
}

static int __devexit jc_spi_remove(struct spi_device *spi)
{
	return 0;
}

static struct spi_driver jc_spi_driver = {
	.driver = {
		.name	= "jc_spi",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
	},
	.probe	= jc_spi_probe,
	.remove = __devexit_p(jc_spi_remove),
};

int jc_spi_init(void)
{
	cam_err("Entered\n");

	return spi_register_driver(&jc_spi_driver);
}

void jc_spi_exit(void)
{
	spi_unregister_driver(&jc_spi_driver);
}
