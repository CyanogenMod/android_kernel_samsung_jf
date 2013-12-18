/*
 * mdp_sec_debug.c
 *
 * This is supporting debug functions for MDP module
 */

#include "sec_debug_mdp.h"

struct sec_debug_mdp sec_debug_mdp;

/*
* CONFIG_SEC_DEBUG_MDP_SAVE_PREV_DATA is not enabled yet.
*/
#ifdef CONFIG_SEC_DEBUG_MDP_SAVE_PREV_STATE
static void set_value(struct sec_debug_entity *entity , unsigned int cmd)
{
	switch (cmd)
	{
		case SEC_DEBUG_IN:
			entity->state |= cmd;
			break;
		case SEC_DEBUG_OUT:
			entity->state |= cmd;
			entity->prev_state |= entity->state;
			entity->state = 0;
			entity->prev_state <<= 4;
			break;
		case SEC_DEBUG_LOCKED:
			entity->state |= cmd;
			break;
		default :
			SEC_DEBUG_MDP("%s : Wrong command\n", __func__);
			break;
	}
}

void sec_debug_mdp_set_value(unsigned int cmd, unsigned char value)
{
	switch (cmd) {
		case SEC_DEBUG_OVERLAY_SET:
			set_value(&sec_debug_mdp.mdp_ioctl.ioctl_overlay_set, value);
			break;
		case SEC_DEBUG_OVERLAY_PLAY:
			set_value(&sec_debug_mdp.mdp_ioctl.ioctl_overlay_play, value);
			break;
		case SEC_DEBUG_OVERLAY_VSYNC_CTRL:
			set_value(&sec_debug_mdp.mdp_ioctl.ioctl_overlay_vsync_ctrl, value);
			break;
		case SEC_DEBUG_BUFFER_SYNC:
			set_value(&sec_debug_mdp.mdp_ioctl.ioctl_buffer_sync, value);
			break;
		case SEC_DEBUG_VSYNC_SYSFS_EVENT:
			set_value(&sec_debug_mdp.mdp_sysfs_event.vsync_sysfs_event, value);
			break;
		case SEC_DEBUG_VSYNC_IRQ:
			set_value(&sec_debug_mdp.mdp_isr.vsync_isr, value);
			break;
		case SEC_DEBUG_DISPLAY_COMMIT:
			set_value(&sec_debug_mdp.mdp_ioctl.ioctl_display_commit, value);
			break;
		default :
			SEC_DEBUG_MDP("%s : Wrong command\n", __func__);
			break;
	}
}
#else
void sec_debug_mdp_set_value(unsigned int cmd, unsigned char value)
{
	switch (cmd) {
		case SEC_DEBUG_OVERLAY_SET:
			sec_debug_mdp.mdp_ioctl.ioctl_overlay_set.state |= value;
			break;
		case SEC_DEBUG_OVERLAY_PLAY:
			sec_debug_mdp.mdp_ioctl.ioctl_overlay_play.state |= value;
			break;
		case SEC_DEBUG_OVERLAY_VSYNC_CTRL:
			sec_debug_mdp.mdp_ioctl.ioctl_overlay_vsync_ctrl.state |= value;
			break;
		case SEC_DEBUG_BUFFER_SYNC:
			sec_debug_mdp.mdp_ioctl.ioctl_buffer_sync.state |= value;
			break;
		case SEC_DEBUG_VSYNC_SYSFS_EVENT:
			sec_debug_mdp.mdp_sysfs_event.vsync_sysfs_event.state |= value;
			break;
		case SEC_DEBUG_VSYNC_IRQ:
			sec_debug_mdp.mdp_isr.vsync_isr.state |= value;
			break;
		default :
			SEC_DEBUG_MDP("Wrong command\n");
			break;
	}
}
#endif /*CONFIG_SEC_DEBUG_MDP_SAVE_PREV_DATA*/

void sec_debug_mdp_reset_value(void)
{
	sec_debug_mdp_print_values();
	memset(&sec_debug_mdp, 0, sizeof(sec_debug_mdp));
}

#ifdef CONFIG_SEC_DEBUG_MDP_SAVE_PREV_STATE
void sec_debug_mdp_print_values(void)
{
	SEC_DEBUG_MDP("###########################\n"
					"IOCTL : %d, %d, %d, %d\n"
					"IOCTL(PREV) : %d, %d, %d, %d\n"
					"SYSFS_EVENT : %d\n"
					"SYSFS_EVENT(PREV) : %d\n"
					"ISR : %d\n"
					"ISR(PREV) : %d\n"
					"[SEC_DEBUG_MDP] "
					"###########################\n",
					sec_debug_mdp.mdp_ioctl.ioctl_overlay_vsync_ctrl.state,
					sec_debug_mdp.mdp_ioctl.ioctl_overlay_set.state,
					sec_debug_mdp.mdp_ioctl.ioctl_buffer_sync.state,
					sec_debug_mdp.mdp_ioctl.ioctl_overlay_play.state,
					sec_debug_mdp.mdp_ioctl.ioctl_overlay_vsync_ctrl.prev_state,
					sec_debug_mdp.mdp_ioctl.ioctl_overlay_set.prev_state,
					sec_debug_mdp.mdp_ioctl.ioctl_buffer_sync.prev_state,
					sec_debug_mdp.mdp_ioctl.ioctl_overlay_play.prev_state,
					sec_debug_mdp.mdp_sysfs_event.vsync_sysfs_event.state,
					sec_debug_mdp.mdp_sysfs_event.vsync_sysfs_event.prev_state,
					sec_debug_mdp.mdp_isr.vsync_isr.state,
					sec_debug_mdp.mdp_isr.vsync_isr.prev_state
					);
}
#else
void sec_debug_mdp_print_values(void)
{
	SEC_DEBUG_MDP("###########################\n"
					"IOCTL : %d, %d, %d, %d\n"
					"SYSFS_EVENT : %d\n"
					"ISR : %d\n"
					"[SEC_DEBUG_MDP] "
					"###########################\n",
					sec_debug_mdp.mdp_ioctl.ioctl_overlay_vsync_ctrl.state,
					sec_debug_mdp.mdp_ioctl.ioctl_overlay_set.state,
					sec_debug_mdp.mdp_ioctl.ioctl_buffer_sync.state,
					sec_debug_mdp.mdp_ioctl.ioctl_overlay_play.state,
					sec_debug_mdp.mdp_sysfs_event.vsync_sysfs_event.state,
					sec_debug_mdp.mdp_isr.vsync_isr.state
					);
}
#endif

void sec_debug_mdp_init(void)
{
	SEC_DEBUG_MDP("SEC MDP debug init");
	memset(&sec_debug_mdp, 0, sizeof(sec_debug_mdp));
	sec_debug_mdp_print_values();
}
