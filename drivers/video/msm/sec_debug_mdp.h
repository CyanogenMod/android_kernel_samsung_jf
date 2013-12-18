/*
 * sec_debug.h
 *
 * header file supporting debug functions for Samsung device
 */
#ifndef __SEC_DEBUG_MDP_H
#define __SEC_DEBUG_MDP_H

#include <linux/kernel.h>
#include "mdp.h"

/*
* SEC Debug Command
*/
#define SEC_DEBUG_OVERLAY_SET           BIT(0)
#define SEC_DEBUG_OVERLAY_PLAY          BIT(1)
#define SEC_DEBUG_OVERLAY_VSYNC_CTRL    BIT(2)
#define SEC_DEBUG_BUFFER_SYNC           BIT(3)
#define SEC_DEBUG_VSYNC_SYSFS_EVENT     BIT(4)
#define SEC_DEBUG_VSYNC_IRQ             BIT(5)
#define SEC_DEBUG_DISPLAY_COMMIT        BIT(6)

/*
* SEC Debug settting value
*/
#define SEC_DEBUG_IN	BIT(0)
#define SEC_DEBUG_OUT	BIT(1)
#define SEC_DEBUG_LOCKED	BIT(2)

#define CONFIG_SEC_DEBUG_MDP_SAVE_PREV_STATE

#ifdef CONFIG_SEC_DEBUG_MDP
#ifdef CONFIG_SEC_DEBUG_MDP_LOG
#define SEC_DEBUG_MDP(x...)	printk(KERN_INFO "[SEC_DEBUG_MDP] " x)
#else
#define SEC_DEBUG_MDP(x...)
#endif /*CONFIG_SEC_DEBUG_MDP_LOG*/

struct sec_debug_entity {
	unsigned char state;
#ifdef CONFIG_SEC_DEBUG_MDP_SAVE_PREV_STATE
	unsigned int prev_state;
#endif
};

struct sec_debug_mdp_ioctl {
	struct sec_debug_entity ioctl_overlay_vsync_ctrl;
	struct sec_debug_entity ioctl_overlay_set;
	struct sec_debug_entity ioctl_buffer_sync;
	struct sec_debug_entity ioctl_overlay_play;
	struct sec_debug_entity ioctl_display_commit;
};

struct sec_debug_mdp_sysfs_event {
	struct sec_debug_entity vsync_sysfs_event;
};

struct sec_debug_mdp_isr {
	struct sec_debug_entity vsync_isr;
};

struct sec_debug_dsi_err {
	unsigned int fps_chage_time_out_err_cnt;
	unsigned int mipi_tx_time_out_err_cnt;
	unsigned int mipi_dsi_ack_err_status;
	unsigned int mipi_dsi_timeout_status;
	unsigned int mipi_dsi_dln0_phy_err;
	unsigned int mipi_dsi_fifo_status;
	unsigned int mipi_dsi_status;
};

struct sec_debug_mdp {
	struct sec_debug_mdp_ioctl mdp_ioctl;
	struct sec_debug_mdp_sysfs_event mdp_sysfs_event;
	struct sec_debug_mdp_isr mdp_isr;
	struct sec_debug_dsi_err dsi_err;
	void *data;
};

void sec_debug_mdp_init(void);
void sec_debug_mdp_set_value(unsigned int cmd, unsigned char value);
void sec_debug_mdp_reset_value(void);
void sec_debug_mdp_print_values(void);
#else
#define	sec_debug_mdp_init() do {} while(0)
#define sec_debug_mdp_set_value(cmd, value) do {} while(0)
#define sec_debug_mdp_reset_value() do {} while(0)
#define sec_debug_mdp_print_values() do {} while(0)
#endif /*CONFIG_SEC_DEBUG_MDP*/
#endif /*__SEC_DEBUG_MDP_H*/
