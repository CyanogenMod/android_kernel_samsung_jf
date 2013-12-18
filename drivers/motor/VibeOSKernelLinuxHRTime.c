/*
** =========================================================================
** File:
**     VibeOSKernelLinuxHRTime.c
**
** Description:
**     High Resolution Time helper functions for Linux.
**
** Portions Copyright (c) 2010 Immersion Corporation. All Rights Reserved.
**
** This file contains Original Code and/or Modifications of Original Code
** as defined in and that are subject to the GNU Public License v2 -
** (the 'License'). You may not use this file except in compliance with the
** License. You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or contact
** TouchSenseSales@immersion.com.
**
** The Original Code and all software distributed under the License are
** distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
** EXPRESS OR IMPLIED, AND IMMERSION HEREBY DISCLAIMS ALL SUCH WARRANTIES,
** INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
** FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. Please see
** the License for the specific language governing rights and limitations
** under the License.
** =========================================================================
*/

/*
** Kernel high-resolution software timer is used as an example but another type
** of timer (such as HW timer or standard software timer)
**  might be used to achieve the 5ms required rate.
*/

#ifndef CONFIG_HIGH_RES_TIMERS
#warning "The Kernel does not have high resolution timers enabled."\
"Either provide a non hr-timer implementation of VibeOSKernelLinuxTime."\
"c or re-compile your kernel with CONFIG_HIGH_RES_TIMERS=y"
#endif

#include <linux/hrtimer.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#define WATCHDOG_TIMEOUT    10  /* 10 timer cycles = 50ms */

/* Global variables */
static bool g_btimerstarted;
static struct hrtimer g_tsptimer;
static ktime_t g_ktfivems;
static int g_nwatchdog_counter;

static struct semaphore g_mutex;

/* Forward declarations */
static void VibeOSKernelLinuxStartTimer(void);
static void VibeOSKernelLinuxStopTimer(void);
static int VibeOSKernelProcessData(void *data);
#define VIBEOSKERNELPROCESSDATA

static inline int VibeSemIsLocked(struct semaphore *lock)
{
	return (lock->count) != 1;
}

static enum hrtimer_restart tsp_timer_interrupt(struct hrtimer *timer)
{
	/* Scheduling next timeout value right away */
	hrtimer_forward_now(timer, g_ktfivems);

	if (g_btimerstarted)
		if (VibeSemIsLocked(&g_mutex))
			up(&g_mutex);

	return HRTIMER_RESTART;
}

static int VibeOSKernelProcessData(void *data)
{
	int i;
	int nactuator_not_playing = 0;

	for (i = 0; i < NUM_ACTUATORS; i++) {
		actuator_samples_buffer *pcurrent_actuator_sample =
							&(g_samples_buffer[i]);

		if (-1 == pcurrent_actuator_sample->nindex_playing_buffer) {
			nactuator_not_playing++;
			if ((NUM_ACTUATORS == nactuator_not_playing) &&
				((++g_nwatchdog_counter) > WATCHDOG_TIMEOUT)) {
				int8_t czero[1] = {0};

				/*
				** Nothing to play for all actuators,
				** turn off the timer
				** when we reach the watchdog tick count limit
				*/
				ImmVibeSPI_ForceOut_SetSamples(i, 8, 1, czero);
				ImmVibeSPI_ForceOut_AmpDisable(i);
				VibeOSKernelLinuxStopTimer();

				/* Reset watchdog counter */
				g_nwatchdog_counter = 0;
			}
		} else {
			/* Play the current buffer */
			if (VIBE_E_FAIL == ImmVibeSPI_ForceOut_SetSamples(
			 pcurrent_actuator_sample->actuator_samples
			  [(int)pcurrent_actuator_sample->nindex_playing_buffer]
			  .nactuator_index,
			 pcurrent_actuator_sample->actuator_samples
			  [(int)pcurrent_actuator_sample->nindex_playing_buffer]
			  .nbit_depth,
			 pcurrent_actuator_sample->actuator_samples
			  [(int)pcurrent_actuator_sample->nindex_playing_buffer]
			  .nbuffer_size,
			 pcurrent_actuator_sample->actuator_samples
			  [(int)pcurrent_actuator_sample->nindex_playing_buffer]
			  .data_buffer)) {
				/* VIBE_E_FAIL means NAK has been handled.
				Schedule timer to restart 5 ms from now */
				hrtimer_forward_now(&g_tsptimer, g_ktfivems);
			}

			pcurrent_actuator_sample->nindex_output_value +=
			 pcurrent_actuator_sample->actuator_samples
			 [(int)pcurrent_actuator_sample->nindex_playing_buffer]
			 .nbuffer_size;

			if (pcurrent_actuator_sample->nindex_output_value >=
			    pcurrent_actuator_sample->actuator_samples
			    [(int)pcurrent_actuator_sample
			    ->nindex_playing_buffer]
			    .nbuffer_size) {
				/* Reach the end of the current buffer */
			    pcurrent_actuator_sample->actuator_samples
			    [(int)pcurrent_actuator_sample
				->nindex_playing_buffer]
			    .nbuffer_size = 0;

				/* Switch buffer */
				(pcurrent_actuator_sample
					->nindex_playing_buffer) ^= 1;
				pcurrent_actuator_sample
					->nindex_output_value = 0;

				/* Finished playing,
				   disable amp for actuator (i) */
				if (g_bstoprequested) {
					pcurrent_actuator_sample
						->nindex_playing_buffer = -1;

					ImmVibeSPI_ForceOut_AmpDisable(i);
				}
			}
		}
	}

    /* If finished playing, stop timer */
	if (g_bstoprequested) {
		VibeOSKernelLinuxStopTimer();

		/* Reset watchdog counter */
		g_nwatchdog_counter = 0;

		if (VibeSemIsLocked(&g_mutex))
			up(&g_mutex);
		return 1;   /* tell the caller this is the last iteration */
	}

	return 0;
}

static void VibeOSKernelLinuxInitTimer(void)
{
	/* Get a 5,000,000ns = 5ms time value */
	g_ktfivems = ktime_set(0, 5000000);
	hrtimer_init(&g_tsptimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);

    /* Initialize a 5ms-timer with tsp_timer_interrupt
	as timer callback (interrupt driven)*/
	g_tsptimer.function = tsp_timer_interrupt;
	sema_init(&g_mutex, 1); /*  initialize simaphore */
}

static void VibeOSKernelLinuxStartTimer(void)
{
	int i;
	int res;

	/* Reset watchdog counter */
	g_nwatchdog_counter = 0;

	if (!g_btimerstarted) {
		if (!VibeSemIsLocked(&g_mutex))
			res = down_interruptible(&g_mutex); /* start locked */

		g_btimerstarted = true;

		/* Start the timer */
		hrtimer_start(&g_tsptimer, g_ktfivems, HRTIMER_MODE_REL);

		/* Don't block the write() function after the first sample
		to allow the host sending the next samples with no delay */
		for (i = 0; i < NUM_ACTUATORS; i++) {
			if ((g_samples_buffer[i]
				.actuator_samples[0].nbuffer_size) ||
				(g_samples_buffer[i]
				.actuator_samples[1].nbuffer_size)) {
				g_samples_buffer[i].nindex_output_value = 0;
				return;
			}
		}
	}

	if (0 != VibeOSKernelProcessData(NULL))
		return;

	/*
	** Use interruptible version of down to be safe
	** (try to not being stuck here if the mutex is
	** not freed for any reason)
	*/
	/* wait for the mutex to be freed by the timer */
	res = down_interruptible(&g_mutex);
	if (res != 0)
		DbgOut((KERN_INFO
		 "tspdrv: down_interruptible interrupted by a signal.\n"));
}

static void VibeOSKernelLinuxStopTimer(void)
{
	int i;

	if (g_btimerstarted) {
		g_btimerstarted = false;
		hrtimer_cancel(&g_tsptimer);
	}

    /* Reset samples buffers */
	for (i = 0; i < NUM_ACTUATORS; i++) {
		g_samples_buffer[i].nindex_playing_buffer = -1;
		g_samples_buffer[i].actuator_samples[0].nbuffer_size = 0;
		g_samples_buffer[i].actuator_samples[1].nbuffer_size = 0;
	}
	g_bstoprequested = false;
	g_bisplaying = false;
}

static void VibeOSKernelLinuxTerminateTimer(void)
{
	VibeOSKernelLinuxStopTimer();
	if (VibeSemIsLocked(&g_mutex))
		up(&g_mutex);
}
