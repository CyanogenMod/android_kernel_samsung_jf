/*
 * sec_debug.c
 *
 * driver supporting debug functions for Samsung device
 *
 * COPYRIGHT(C) Samsung Electronics Co., Ltd. 2006-2011 All Right Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/ctype.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/sysrq.h>
#include <asm/cacheflush.h>
#include <linux/io.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/sec_param.h>
#include <mach/system.h>
#include <mach/sec_debug.h>
#include <mach/msm_iomap.h>
#include <mach/msm_smsm.h>
#ifdef CONFIG_SEC_DEBUG_LOW_LOG
#include <linux/seq_file.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#endif
#include <asm/system_info.h>

/* onlyjazz.ed26 : make the restart_reason global to enable it early
   in sec_debug_init and share with restart functions */


#ifdef CONFIG_SEC_DEBUG_DOUBLE_FREE
#include <linux/circ_buf.h>
#endif

enum sec_debug_upload_cause_t {
	UPLOAD_CAUSE_INIT = 0xCAFEBABE,
	UPLOAD_CAUSE_KERNEL_PANIC = 0x000000C8,
	UPLOAD_CAUSE_FORCED_UPLOAD = 0x00000022,
	UPLOAD_CAUSE_CP_ERROR_FATAL = 0x000000CC,
	UPLOAD_CAUSE_MDM_ERROR_FATAL = 0x000000EE,
	UPLOAD_CAUSE_USER_FAULT = 0x0000002F,
	UPLOAD_CAUSE_HSIC_DISCONNECTED = 0x000000DD,
	UPLOAD_CAUSE_MODEM_RST_ERR = 0x000000FC,
	UPLOAD_CAUSE_RIVA_RST_ERR = 0x000000FB,
	UPLOAD_CAUSE_LPASS_RST_ERR = 0x000000FA,
	UPLOAD_CAUSE_DSPS_RST_ERR = 0x000000FD,
	UPLOAD_CAUSE_PERIPHERAL_ERR = 0x000000FF,
};

struct sec_debug_mmu_reg_t {
	int SCTLR;
	int TTBR0;
	int TTBR1;
	int TTBCR;
	int DACR;
	int DFSR;
	int DFAR;
	int IFSR;
	int IFAR;
	int DAFSR;
	int IAFSR;
	int PMRRR;
	int NMRRR;
	int FCSEPID;
	int CONTEXT;
	int URWTPID;
	int UROTPID;
	int POTPIDR;
};

/* ARM CORE regs mapping structure */
struct sec_debug_core_t {
	/* COMMON */
	unsigned int r0;
	unsigned int r1;
	unsigned int r2;
	unsigned int r3;
	unsigned int r4;
	unsigned int r5;
	unsigned int r6;
	unsigned int r7;
	unsigned int r8;
	unsigned int r9;
	unsigned int r10;
	unsigned int r11;
	unsigned int r12;

	/* SVC */
	unsigned int r13_svc;
	unsigned int r14_svc;
	unsigned int spsr_svc;

	/* PC & CPSR */
	unsigned int pc;
	unsigned int cpsr;

	/* USR/SYS */
	unsigned int r13_usr;
	unsigned int r14_usr;

	/* FIQ */
	unsigned int r8_fiq;
	unsigned int r9_fiq;
	unsigned int r10_fiq;
	unsigned int r11_fiq;
	unsigned int r12_fiq;
	unsigned int r13_fiq;
	unsigned int r14_fiq;
	unsigned int spsr_fiq;

	/* IRQ */
	unsigned int r13_irq;
	unsigned int r14_irq;
	unsigned int spsr_irq;

	/* MON */
	unsigned int r13_mon;
	unsigned int r14_mon;
	unsigned int spsr_mon;

	/* ABT */
	unsigned int r13_abt;
	unsigned int r14_abt;
	unsigned int spsr_abt;

	/* UNDEF */
	unsigned int r13_und;
	unsigned int r14_und;
	unsigned int spsr_und;
};

/* enable sec_debug feature */
static unsigned enable = 1;
static unsigned enable_user = 1;
static unsigned reset_reason = 0xFFEEFFEE;
static char sec_build_info[100];
static unsigned int secdbg_paddr;
static unsigned int secdbg_size;

unsigned int sec_dbg_level;

uint runtime_debug_val;

module_param_named(enable, enable, uint, 0644);
module_param_named(enable_user, enable_user, uint, 0644);
module_param_named(reset_reason, reset_reason, uint, 0644);
module_param_named(runtime_debug_val, runtime_debug_val, uint, 0644);

static int force_error(const char *val, struct kernel_param *kp);
module_param_call(force_error, force_error, NULL, NULL, 0644);

static int dbg_set_cpu_affinity(const char *val, struct kernel_param *kp);
module_param_call(setcpuaff, dbg_set_cpu_affinity, NULL, NULL, 0644);
static char *sec_build_time[] = {
	__DATE__,
	__TIME__
};
static char build_root[] = __FILE__;


/* klaatu - schedule log */
#ifdef CONFIG_SEC_DEBUG_SCHED_LOG
struct sec_debug_log {
	atomic_t idx_sched[CONFIG_NR_CPUS];
	struct sched_log sched[CONFIG_NR_CPUS][SCHED_LOG_MAX];

	atomic_t idx_irq[CONFIG_NR_CPUS];
	struct irq_log irq[CONFIG_NR_CPUS][SCHED_LOG_MAX];

	atomic_t idx_irq_exit[CONFIG_NR_CPUS];
	struct irq_exit_log irq_exit[CONFIG_NR_CPUS][SCHED_LOG_MAX];

	atomic_t idx_timer[CONFIG_NR_CPUS];
	struct timer_log timer_log[CONFIG_NR_CPUS][SCHED_LOG_MAX];

#ifdef CONFIG_SEC_DEBUG_MSG_LOG
	atomic_t idx_secmsg[CONFIG_NR_CPUS];
	struct secmsg_log secmsg[CONFIG_NR_CPUS][MSG_LOG_MAX];
#endif
#ifdef CONFIG_SEC_DEBUG_AVC_LOG
	atomic_t idx_secavc[CONFIG_NR_CPUS];
	struct secavc_log secavc[CONFIG_NR_CPUS][AVC_LOG_MAX];
#endif
#ifdef CONFIG_SEC_DEBUG_DCVS_LOG
	atomic_t dcvs_log_idx[CONFIG_NR_CPUS] ;
	struct dcvs_debug dcvs_log[CONFIG_NR_CPUS][DCVS_LOG_MAX] ;
#endif
#ifdef CONFIG_SEC_DEBUG_FUELGAUGE_LOG
	atomic_t fg_log_idx;
	struct fuelgauge_debug fg_log[FG_LOG_MAX] ;
#endif
};

struct sec_debug_log *secdbg_log;
struct sec_debug_subsys *secdbg_subsys;
struct sec_debug_subsys_data_krait *secdbg_krait;

#endif	/* CONFIG_SEC_DEBUG_SCHED_LOG */

/* klaatu - semaphore log */
#ifdef CONFIG_SEC_DEBUG_SEMAPHORE_LOG
static struct sem_debug sem_debug_free_head;
static struct sem_debug sem_debug_done_head;
static int sem_debug_free_head_cnt;
static int sem_debug_done_head_cnt;
static int sem_debug_init;
static spinlock_t sem_debug_lock;

/* rwsemaphore logging */
static struct rwsem_debug rwsem_debug_free_head;
static struct rwsem_debug rwsem_debug_done_head;
static int rwsem_debug_free_head_cnt;
static int rwsem_debug_done_head_cnt;
static int rwsem_debug_init;
static spinlock_t rwsem_debug_lock;
#endif	/* CONFIG_SEC_DEBUG_SEMAPHORE_LOG */

void __hexdump(void *mem, unsigned long size)
{
	#define WORDS_PER_LINE 4
	#define WORD_SIZE 4
	#define LINE_SIZE (WORDS_PER_LINE * WORD_SIZE)
	#define LINE_BUF_SIZE (WORDS_PER_LINE * WORD_SIZE * 3 \
		+ WORDS_PER_LINE + 4)
	unsigned long addr;
	char linebuf[LINE_BUF_SIZE];
	int numline = size / LINE_SIZE;
	int i;
	for (i = 0; i < numline; i++) {
		addr = (unsigned long)mem + i * LINE_SIZE;
		hex_dump_to_buffer((const void *)addr,
			LINE_SIZE, LINE_SIZE,
			WORD_SIZE, linebuf, sizeof(linebuf), 1);
		pr_info(" %lx : %s\n", addr, linebuf);
	}

}

#ifdef CONFIG_SEC_DEBUG_DOUBLE_FREE

/* Double free detector(dfd) can use lots of memory because
 * it needs to hold on the freed slabs, otherwise
 * the the freeing node will be put into the kmem cache
 * of it's size and the slab allocator will soon re-allocate
 * this slab when the slab of that size is requested.
 * So to alleviate the pressure of the other shrinkers when
 * there is a memory pressure, enable DFD_HAS_SHRINKER below.
 */
#define DFD_HAS_SHRINKER

#ifdef DFD_HAS_SHRINKER
/* Using DFD shrinker will keep the DFD buffer entries low
 * but at a cost. The page allocator will often go into
 * the slowpath and try to reclaim pages and eventually call
 * the shrinker.
 * If you want to  avoid this overhead, enable below feature
 * to completely flush out the circular buffer and disable the
 * DFD when there is a memory pressure */
#define DFD_SHRINKER_DISABLE_DFD_ON_MEM_PRESSURE
#endif

#define KFREE_HOOK_BYPASS_MASK 0x1
/* The average size of a slab object is about 256 bytes.
 * 1<<5 number of slab objects take about 8MB to 10MB
 * (This average was mesaured with a min slab size of 64) */
#define KFREE_CIRC_BUF_SIZE (1<<15)
#define KFREE_FREE_MAGIC 0xf4eef4ee

static int dfd_disabled;

static DEFINE_SPINLOCK(circ_buf_lock);
struct kfree_info_entry {
	void *addr;
	void *caller;
};

struct kfree_circ_buf {
	int head;
	int tail;
	struct kfree_info_entry entry[KFREE_CIRC_BUF_SIZE];
};

struct kfree_circ_buf kfree_circ_buf;

/* the caller must hold the circ_buf_lock */
static void *circ_buf_lookup(struct kfree_circ_buf *circ_buf, void *addr)
{
	int i;
	for (i = circ_buf->tail; i != circ_buf->head ;
		i = (i + 1) & (KFREE_CIRC_BUF_SIZE - 1)) {
		if (circ_buf->entry[i].addr == addr)
			return &circ_buf->entry[i];
	}

	return NULL;
}

/* the caller must hold the circ_buf_lock and must check
 * for the buffer status before calling */
static void *circ_buf_get(struct kfree_circ_buf *circ_buf)
{
	void *entry;
	entry = &circ_buf->entry[circ_buf->tail];
	smp_rmb();
	circ_buf->tail = (circ_buf->tail + 1) &
		(KFREE_CIRC_BUF_SIZE - 1);
	return entry;
}

/* the caller must hold the circ_buf_lock and must check
 * for the buffer status before calling */
static void *circ_buf_put(struct kfree_circ_buf *circ_buf,
				struct kfree_info_entry *entry)
{
	memcpy(&circ_buf->entry[circ_buf->head], entry, sizeof(*entry));
	smp_wmb();
	circ_buf->head = (circ_buf->head + 1) &
		(KFREE_CIRC_BUF_SIZE - 1);
	return entry;
}

static int dfd_flush(void)
{
	struct kfree_info_entry *pentry;
	unsigned long cnt;
	unsigned long flags;

	spin_lock_irqsave(&circ_buf_lock, flags);
	cnt = CIRC_CNT(kfree_circ_buf.head, kfree_circ_buf.tail,
		KFREE_CIRC_BUF_SIZE);
	spin_unlock_irqrestore(&circ_buf_lock, flags);
	pr_debug("%s: cnt=%lu\n", __func__, cnt);

do_flush:
	while (cnt) {
		pentry = NULL;
		/* we want to keep the lock region as short as possible
		 * so we will re-read the buf count every loop */
		spin_lock_irqsave(&circ_buf_lock, flags);
		cnt = CIRC_CNT(kfree_circ_buf.head, kfree_circ_buf.tail,
			KFREE_CIRC_BUF_SIZE);
		if (cnt == 0) {
			spin_unlock_irqrestore(&circ_buf_lock, flags);
			break;
		}
		pentry = circ_buf_get(&kfree_circ_buf);
		spin_unlock_irqrestore(&circ_buf_lock, flags);
		if (pentry)
			kfree((void *)((unsigned long)pentry->addr |
				KFREE_HOOK_BYPASS_MASK));
		cnt--;
	}

	spin_lock_irqsave(&circ_buf_lock, flags);
	cnt = CIRC_CNT(kfree_circ_buf.head, kfree_circ_buf.tail,
		KFREE_CIRC_BUF_SIZE);
	spin_unlock_irqrestore(&circ_buf_lock, flags);

	if (!dfd_disabled)
		goto out;

	if (cnt)
		goto do_flush;

out:
	return cnt;
}

static void dfd_disable(void)
{
	dfd_disabled = 1;
	dfd_flush();
	pr_info("%s: double free detection is disabled\n", __func__);
}

static void dfd_enable(void)
{
	dfd_disabled = 0;
	pr_info("%s: double free detection is enabled\n", __func__);
}

#ifdef DFD_HAS_SHRINKER
int dfd_shrink(struct shrinker *shrinker, struct shrink_control *sc)
{
#ifndef DFD_SHRINKER_DISABLE_DFD_ON_MEM_PRESSURE
	struct kfree_info_entry *pentry;
	unsigned long nr = sc->nr_to_scan;
#endif
	unsigned long flags;
	unsigned long nr_objs;

	spin_lock_irqsave(&circ_buf_lock, flags);
	nr_objs = CIRC_CNT(kfree_circ_buf.head, kfree_circ_buf.tail,
		KFREE_CIRC_BUF_SIZE);
	spin_unlock_irqrestore(&circ_buf_lock, flags);

	/* nothing to reclaim from here */
	if (nr_objs == 0) {
		nr_objs = -1;
		goto out;
	}

#ifdef DFD_SHRINKER_DISABLE_DFD_ON_MEM_PRESSURE
	/* disable double free detection. This will flush
	 * the entire circular buffer out. */
	dfd_disable();
#else
	/* return max slab objects freeable */
	if (nr == 0)
		return  nr_objs;

	if (nr > nr_objs)
		nr = nr_objs;

	pr_debug("%s: nr_objs=%lu\n", __func__, nr_objs);
	while (nr) {
		unsigned long cnt;
		pentry = NULL;
		spin_lock_irqsave(&circ_buf_lock, flags);
		cnt = CIRC_CNT(kfree_circ_buf.head, kfree_circ_buf.tail,
			KFREE_CIRC_BUF_SIZE);
		if (cnt > 0)
			pentry = circ_buf_get(&kfree_circ_buf);
		spin_unlock_irqrestore(&circ_buf_lock, flags);
		if (pentry)
			kfree((void *)((unsigned long)pentry->addr |
				KFREE_HOOK_BYPASS_MASK));
		nr--;
	}
#endif
	spin_lock_irqsave(&circ_buf_lock, flags);
	nr_objs = CIRC_CNT(kfree_circ_buf.head, kfree_circ_buf.tail,
		KFREE_CIRC_BUF_SIZE);
	spin_unlock_irqrestore(&circ_buf_lock, flags);
	if (nr_objs == 0) {
		pr_info("%s: nothing more to reclaim from here!\n", __func__);
		nr_objs = -1;
	}

out:
	return nr_objs;
}

static struct shrinker dfd_shrinker = {
	.shrink = dfd_shrink,
	.seeks = DEFAULT_SEEKS
};

static int __init dfd_shrinker_init(void)
{
	register_shrinker(&dfd_shrinker);
	return 0;
}

static void __exit dfd_shrinker_exit(void)
{
	unregister_shrinker(&dfd_shrinker);
}

module_init(dfd_shrinker_init);
module_exit(dfd_shrinker_exit);
#endif

static int __do_panic_for_dblfree(void)
{
	return (sec_dbg_level == KERNEL_SEC_DEBUG_LEVEL_MID ||
		sec_dbg_level == KERNEL_SEC_DEBUG_LEVEL_HIGH);
}

static inline int __dblfree_check_magic_any(void *addr)
{
	return (((unsigned int *)addr)[0] == KFREE_FREE_MAGIC ||
		((unsigned int *)addr)[1] == KFREE_FREE_MAGIC);
}
static inline int __dblfree_check_magic_all(void *addr)
{
	return (((unsigned int *)addr)[0] == KFREE_FREE_MAGIC &&
		((unsigned int *)addr)[1] == KFREE_FREE_MAGIC);
}
static inline void __dblfree_set_magic(void *addr)
{
	BUILD_BUG_ON(KMALLOC_MIN_SIZE < 8);
	((unsigned long *)addr)[0] = KFREE_FREE_MAGIC;
	((unsigned long *)addr)[1] = KFREE_FREE_MAGIC;
}
static inline void __dblfree_clear_magic(void *addr)
{
	BUILD_BUG_ON(KMALLOC_MIN_SIZE < 8);
	((unsigned long *)addr)[0] = 0;
	((unsigned long *)addr)[1] = 0;
}
void *kfree_hook(void *p, void *caller)
{
	unsigned long flags;
	struct kfree_info_entry *match = NULL;
	void *tofree = NULL;
	unsigned long addr = (unsigned long)p;
	struct kfree_info_entry entry;
	struct kfree_info_entry *pentry;

	if (!virt_addr_valid(addr)) {
		/* there are too many NULL pointers so don't print for NULL */
		if (addr)
			pr_debug("%s: trying to free an invalid addr %lx "\
				"from %pS\n", __func__, addr, caller);
		return NULL;
	}

	if (addr & KFREE_HOOK_BYPASS_MASK || dfd_disabled) {
		/* return original address to free */
		return (void *)(addr&~(KFREE_HOOK_BYPASS_MASK));
	}

	spin_lock_irqsave(&circ_buf_lock, flags);

	if (kfree_circ_buf.head == 0)
		pr_debug("%s: circular buffer head rounded to zero.", __func__);

	/* We can detect all the double free in the circular buffer time frame
	 * if we scan the whole circular buffer all the time, but to minimize
	 * the performance degradation we will just check for the magic values
	 * (the number of magic values can be up to KMALLOC_MIN_SIZE/4) */
	if (__dblfree_check_magic_any(p)) {
		/* memory that is to be freed may originally have had magic
		 * value, so search the whole circ buf for an actual match */
		match = circ_buf_lookup(&kfree_circ_buf, p);
		if (!match) {
			pr_debug("%s: magic set but not in circ buf\n", __func__);
		}
	}

	if (match) {
		pr_err("%s: 0x%08lx was already freed by %pS()\n",
			__func__, (unsigned long)p, match->caller);
		spin_unlock_irqrestore(&circ_buf_lock, flags);
		if (__do_panic_for_dblfree())
			panic("double free detected!");
		/* if we don't panic we just return without adding this entry
		 * to the circular buffer. This means that this kfree is ommited
		 * and we are just forgiving the double free */
		dump_stack();
		return NULL;
	}

	/* mark free magic on the freeing node */
	__dblfree_set_magic(p);

	/* do an actual kfree for the oldest entry
	 * if the circular buffer is full */
	if (CIRC_SPACE(kfree_circ_buf.head, kfree_circ_buf.tail,
		KFREE_CIRC_BUF_SIZE) == 0) {
		pentry = circ_buf_get(&kfree_circ_buf);
		if (pentry)
			tofree = pentry->addr;
	}

	/* add the new entry to the circular buffer */
	entry.addr = p;
	entry.caller = caller;
	circ_buf_put(&kfree_circ_buf, &entry);
	if (tofree) {
		if (unlikely(!__dblfree_check_magic_all(tofree))) {
			pr_err("\n%s: There has been a dangling reference on"\
				" the node %lx which was freed by %pS."\
				"\nThis could be fatal because if the second"\
				" free is called after all the free magic has"\
				" been corrupted,\nwe won't detect it has a"\
				" double free.\n",
				__func__, (unsigned long)tofree,
				pentry->caller);
			__hexdump((void *)tofree, KMALLOC_MIN_SIZE);
			pr_err("\n");
		}
		__dblfree_clear_magic(tofree);
		spin_unlock_irqrestore(&circ_buf_lock, flags);
		/* do the real kfree */
		kfree((void *)((unsigned long)tofree | KFREE_HOOK_BYPASS_MASK));
		return NULL;
	}

	spin_unlock_irqrestore(&circ_buf_lock, flags);
	return NULL;
}
#endif

/* onlyjazz.ed26 : make the restart_reason global to enable it early
   in sec_debug_init and share with restart functions */
void *restart_reason;

DEFINE_PER_CPU(struct sec_debug_core_t, sec_debug_core_reg);
DEFINE_PER_CPU(struct sec_debug_mmu_reg_t, sec_debug_mmu_reg);
DEFINE_PER_CPU(enum sec_debug_upload_cause_t, sec_debug_upload_cause);

static int force_error(const char *val, struct kernel_param *kp)
{
	pr_emerg("!!!WARN forced error : %s\n", val);

	if (!strncmp(val, "wdog", 4)) {
		pr_emerg("Generating a wdog bark!\n");
		raw_local_irq_disable();
		while (1)
			;
	} else if (!strncmp(val, "dabort", 6)) {
		pr_emerg("Generating a data abort exception!\n");
		*(unsigned int *)0x0 = 0x0;
	} else if (!strncmp(val, "pabort", 6)) {
		pr_emerg("Generating a prefetch abort exception!\n");
		((void (*)(void))0x0)();
	} else if (!strncmp(val, "undef", 5)) {
		pr_emerg("Generating a undefined instruction exception!\n");
		BUG();
	} else if (!strncmp(val, "bushang", 7)) {
		void __iomem *p;
		pr_emerg("Generating Bus Hang!\n");
		p = ioremap_nocache(0x04300000, 32);
		*(unsigned int *)p = *(unsigned int *)p;
		mb();
		pr_info("*p = %x\n", *(unsigned int *)p);
		pr_emerg("Clk may be enabled.Try again if it reaches here!\n");
	} else if (!strncmp(val, "dblfree", 7)) {
		void *p = kmalloc(sizeof(int), GFP_KERNEL);
		kfree(p);
		msleep(1000);
		kfree(p);
	} else if (!strncmp(val, "danglingref", 11)) {
		unsigned int *p = kmalloc(sizeof(int), GFP_KERNEL);
		kfree(p);
		*p = 0x1234;
	} else if (!strncmp(val, "lowmem", 6)) {
		int i = 0;
		pr_emerg("Allocating memory until failure!\n");
		while (kmalloc(128*1024, GFP_KERNEL))
			i++;
		pr_emerg("Allocated %d KB!\n", i*128);

	} else if (!strncmp(val, "memcorrupt", 10)) {
		int *ptr = kmalloc(sizeof(int), GFP_KERNEL);
		*ptr++ = 4;
		*ptr = 2;
		panic("MEMORY CORRUPTION");
#ifdef CONFIG_SEC_DEBUG_DOUBLE_FREE
	} else if (!strncmp(val, "dfdenable", 9)) {
		dfd_enable();
	} else if (!strncmp(val, "dfddisable", 10)) {
		dfd_disable();
#endif
	} else {
		pr_emerg("No such error defined for now!\n");
	}

	return 0;
}


static int dbg_set_cpu_affinity(const char *val, struct kernel_param *kp)
{
	char *endptr;
	pid_t pid;
	int cpu;
	struct cpumask mask;
	long ret;
	pid = (pid_t)memparse(val, &endptr);
	if (*endptr != '@') {
		pr_info("%s: invalid input strin: %s\n", __func__, val);
		return -EINVAL;
	}
	cpu = memparse(++endptr, &endptr);
	cpumask_clear(&mask);
	cpumask_set_cpu(cpu, &mask);
	pr_info("%s: Setting %d cpu affinity to cpu%d\n",
		__func__, pid, cpu);
	ret = sched_setaffinity(pid, &mask);
	pr_info("%s: sched_setaffinity returned %ld\n", __func__, ret);
	return 0;
}

/* for sec debug level */
static int __init sec_debug_level(char *str)
{
	get_option(&str, &sec_dbg_level);
	return 0;
}
early_param("level", sec_debug_level);

bool kernel_sec_set_debug_level(int level)
{
	if (!(level == KERNEL_SEC_DEBUG_LEVEL_LOW
			|| level == KERNEL_SEC_DEBUG_LEVEL_MID
			|| level == KERNEL_SEC_DEBUG_LEVEL_HIGH)) {
		pr_notice(KERN_NOTICE "(kernel_sec_set_debug_level) The debug"\
				"value is invalid(0x%x)!! Set default"\
				"level(LOW)\n", level);
		sec_dbg_level = KERNEL_SEC_DEBUG_LEVEL_LOW;
		return -EINVAL;
	}

	sec_dbg_level = level;

	switch (level) {
	case KERNEL_SEC_DEBUG_LEVEL_LOW:
		enable = 0;
		enable_user = 0;
		break;
	case KERNEL_SEC_DEBUG_LEVEL_MID:
		enable = 1;
		enable_user = 0;
		break;
	case KERNEL_SEC_DEBUG_LEVEL_HIGH:
		enable = 1;
		enable_user = 1;
		break;
	default:
		enable = 1;
		enable_user = 1;
	}

	/* write to param */
	sec_set_param(param_index_debuglevel, &sec_dbg_level);

	pr_notice(KERN_NOTICE "(kernel_sec_set_debug_level)"\
			"The debug value is 0x%x !!\n", level);

	return 1;
}
EXPORT_SYMBOL(kernel_sec_set_debug_level);

int kernel_sec_get_debug_level(void)
{
	sec_get_param(param_index_debuglevel, &sec_dbg_level);

	if (!(sec_dbg_level == KERNEL_SEC_DEBUG_LEVEL_LOW
			|| sec_dbg_level == KERNEL_SEC_DEBUG_LEVEL_MID
			|| sec_dbg_level == KERNEL_SEC_DEBUG_LEVEL_HIGH)) {
		/*In case of invalid debug level, default (debug level low)*/
		pr_notice(KERN_NOTICE "(%s) The debug value is"\
				"invalid(0x%x)!! Set default level(LOW)\n",
				__func__, sec_dbg_level);
		sec_dbg_level = KERNEL_SEC_DEBUG_LEVEL_LOW;
		sec_set_param(param_index_debuglevel, &sec_dbg_level);
	}
	return sec_dbg_level;
}
EXPORT_SYMBOL(kernel_sec_get_debug_level);

/* core reg dump function*/
static void sec_debug_save_core_reg(struct sec_debug_core_t *core_reg)
{
	/* we will be in SVC mode when we enter this function. Collect
	   SVC registers along with cmn registers. */
	asm("str r0, [%0,#0]\n\t"	/* R0 is pushed first to core_reg */
	    "mov r0, %0\n\t"		/* R0 will be alias for core_reg */
	    "str r1, [r0,#4]\n\t"	/* R1 */
	    "str r2, [r0,#8]\n\t"	/* R2 */
	    "str r3, [r0,#12]\n\t"	/* R3 */
	    "str r4, [r0,#16]\n\t"	/* R4 */
	    "str r5, [r0,#20]\n\t"	/* R5 */
	    "str r6, [r0,#24]\n\t"	/* R6 */
	    "str r7, [r0,#28]\n\t"	/* R7 */
	    "str r8, [r0,#32]\n\t"	/* R8 */
	    "str r9, [r0,#36]\n\t"	/* R9 */
	    "str r10, [r0,#40]\n\t"	/* R10 */
	    "str r11, [r0,#44]\n\t"	/* R11 */
	    "str r12, [r0,#48]\n\t"	/* R12 */
	    /* SVC */
	    "str r13, [r0,#52]\n\t"	/* R13_SVC */
	    "str r14, [r0,#56]\n\t"	/* R14_SVC */
	    "mrs r1, spsr\n\t"		/* SPSR_SVC */
	    "str r1, [r0,#60]\n\t"
	    /* PC and CPSR */
	    "sub r1, r15, #0x4\n\t"	/* PC */
	    "str r1, [r0,#64]\n\t"
	    "mrs r1, cpsr\n\t"		/* CPSR */
	    "str r1, [r0,#68]\n\t"
	    /* SYS/USR */
	    "mrs r1, cpsr\n\t"		/* switch to SYS mode */
	    "and r1, r1, #0xFFFFFFE0\n\t"
	    "orr r1, r1, #0x1f\n\t"
	    "msr cpsr,r1\n\t"
	    "str r13, [r0,#72]\n\t"	/* R13_USR */
	    "str r14, [r0,#76]\n\t"	/* R14_USR */
	    /* FIQ */
	    "mrs r1, cpsr\n\t"		/* switch to FIQ mode */
	    "and r1,r1,#0xFFFFFFE0\n\t"
	    "orr r1,r1,#0x11\n\t"
	    "msr cpsr,r1\n\t"
	    "str r8, [r0,#80]\n\t"	/* R8_FIQ */
	    "str r9, [r0,#84]\n\t"	/* R9_FIQ */
	    "str r10, [r0,#88]\n\t"	/* R10_FIQ */
	    "str r11, [r0,#92]\n\t"	/* R11_FIQ */
	    "str r12, [r0,#96]\n\t"	/* R12_FIQ */
	    "str r13, [r0,#100]\n\t"	/* R13_FIQ */
	    "str r14, [r0,#104]\n\t"	/* R14_FIQ */
	    "mrs r1, spsr\n\t"		/* SPSR_FIQ */
	    "str r1, [r0,#108]\n\t"
		/* IRQ */
	    "mrs r1, cpsr\n\t"		/* switch to IRQ mode */
	    "and r1, r1, #0xFFFFFFE0\n\t"
	    "orr r1, r1, #0x12\n\t"
	    "msr cpsr,r1\n\t"
	    "str r13, [r0,#112]\n\t"	/* R13_IRQ */
	    "str r14, [r0,#116]\n\t"	/* R14_IRQ */
	    "mrs r1, spsr\n\t"		/* SPSR_IRQ */
	    "str r1, [r0,#120]\n\t"
	    /* MON */
	    "mrs r1, cpsr\n\t"		/* switch to monitor mode */
	    "and r1, r1, #0xFFFFFFE0\n\t"
	    "orr r1, r1, #0x16\n\t"
	    "msr cpsr,r1\n\t"
	    "str r13, [r0,#124]\n\t"	/* R13_MON */
	    "str r14, [r0,#128]\n\t"	/* R14_MON */
	    "mrs r1, spsr\n\t"		/* SPSR_MON */
	    "str r1, [r0,#132]\n\t"
	    /* ABT */
	    "mrs r1, cpsr\n\t"		/* switch to Abort mode */
	    "and r1, r1, #0xFFFFFFE0\n\t"
	    "orr r1, r1, #0x17\n\t"
	    "msr cpsr,r1\n\t"
	    "str r13, [r0,#136]\n\t"	/* R13_ABT */
	    "str r14, [r0,#140]\n\t"	/* R14_ABT */
	    "mrs r1, spsr\n\t"		/* SPSR_ABT */
	    "str r1, [r0,#144]\n\t"
	    /* UND */
	    "mrs r1, cpsr\n\t"		/* switch to undef mode */
	    "and r1, r1, #0xFFFFFFE0\n\t"
	    "orr r1, r1, #0x1B\n\t"
	    "msr cpsr,r1\n\t"
	    "str r13, [r0,#148]\n\t"	/* R13_UND */
	    "str r14, [r0,#152]\n\t"	/* R14_UND */
	    "mrs r1, spsr\n\t"		/* SPSR_UND */
	    "str r1, [r0,#156]\n\t"
	    /* restore to SVC mode */
	    "mrs r1, cpsr\n\t"		/* switch to SVC mode */
	    "and r1, r1, #0xFFFFFFE0\n\t"
	    "orr r1, r1, #0x13\n\t"
	    "msr cpsr,r1\n\t" :		/* output */
	    : "r"(core_reg)			/* input */
	    : "%r0", "%r1"		/* clobbered registers */
	);

	return;
}

static void sec_debug_save_mmu_reg(struct sec_debug_mmu_reg_t *mmu_reg)
{
	asm("mrc    p15, 0, r1, c1, c0, 0\n\t"	/* SCTLR */
	    "str r1, [%0]\n\t"
	    "mrc    p15, 0, r1, c2, c0, 0\n\t"	/* TTBR0 */
	    "str r1, [%0,#4]\n\t"
	    "mrc    p15, 0, r1, c2, c0,1\n\t"	/* TTBR1 */
	    "str r1, [%0,#8]\n\t"
	    "mrc    p15, 0, r1, c2, c0,2\n\t"	/* TTBCR */
	    "str r1, [%0,#12]\n\t"
	    "mrc    p15, 0, r1, c3, c0,0\n\t"	/* DACR */
	    "str r1, [%0,#16]\n\t"
	    "mrc    p15, 0, r1, c5, c0,0\n\t"	/* DFSR */
	    "str r1, [%0,#20]\n\t"
	    "mrc    p15, 0, r1, c6, c0,0\n\t"	/* DFAR */
	    "str r1, [%0,#24]\n\t"
	    "mrc    p15, 0, r1, c5, c0,1\n\t"	/* IFSR */
	    "str r1, [%0,#28]\n\t"
	    "mrc    p15, 0, r1, c6, c0,2\n\t"	/* IFAR */
	    "str r1, [%0,#32]\n\t"
	    /* Don't populate DAFSR and RAFSR */
	    "mrc    p15, 0, r1, c10, c2,0\n\t"	/* PMRRR */
	    "str r1, [%0,#44]\n\t"
	    "mrc    p15, 0, r1, c10, c2,1\n\t"	/* NMRRR */
	    "str r1, [%0,#48]\n\t"
	    "mrc    p15, 0, r1, c13, c0,0\n\t"	/* FCSEPID */
	    "str r1, [%0,#52]\n\t"
	    "mrc    p15, 0, r1, c13, c0,1\n\t"	/* CONTEXT */
	    "str r1, [%0,#56]\n\t"
	    "mrc    p15, 0, r1, c13, c0,2\n\t"	/* URWTPID */
	    "str r1, [%0,#60]\n\t"
	    "mrc    p15, 0, r1, c13, c0,3\n\t"	/* UROTPID */
	    "str r1, [%0,#64]\n\t"
	    "mrc    p15, 0, r1, c13, c0,4\n\t"	/* POTPIDR */
	    "str r1, [%0,#68]\n\t" :		/* output */
	    : "r"(mmu_reg)			/* input */
	    : "%r1", "memory"			/* clobbered register */
	);
}

static void sec_debug_save_context(void)
{
	unsigned long flags;
	local_irq_save(flags);
	sec_debug_save_mmu_reg(&per_cpu
			(sec_debug_mmu_reg, smp_processor_id()));
	sec_debug_save_core_reg(&per_cpu
			(sec_debug_core_reg, smp_processor_id()));
	pr_emerg("(%s) context saved(CPU:%d)\n", __func__,
			smp_processor_id());
	local_irq_restore(flags);
}

#define RESTART_REASON_ADDR 0x65C
static void sec_debug_set_upload_magic(unsigned magic)
{
	pr_emerg("(%s) %x\n", __func__, magic);

	restart_reason = MSM_IMEM_BASE + RESTART_REASON_ADDR;
	__raw_writel(magic, restart_reason);

	flush_cache_all();
	outer_flush_all();
}

static int sec_debug_normal_reboot_handler(struct notifier_block *nb,
		unsigned long l, void *p)
{
	sec_debug_set_upload_magic(0x0);
	return 0;
}

static void sec_debug_set_upload_cause(enum sec_debug_upload_cause_t type)
{
	per_cpu(sec_debug_upload_cause, smp_processor_id()) = type;
	*(unsigned int *)0xc0000004 = type;
	pr_emerg("(%s) %x\n", __func__, type);
}

void sec_debug_hw_reset(void)
{
	pr_emerg("(%s) %s\n", __func__, sec_build_info);
	pr_emerg("(%s) rebooting...\n", __func__);
	flush_cache_all();
	outer_flush_all();
	msm_restart(0, "sec_debug_hw_reset");

	while (1)
		;
}
EXPORT_SYMBOL(sec_debug_hw_reset);

#ifdef CONFIG_SEC_DEBUG_LOW_LOG
unsigned sec_debug_get_reset_reason(void)
{
return reset_reason;
}
#endif
static int sec_debug_panic_handler(struct notifier_block *nb,
		unsigned long l, void *buf)
{
	unsigned int len;
	emerg_pet_watchdog();
	sec_debug_set_upload_magic(0x776655ee);

	if (!enable) {
#ifdef CONFIG_SEC_DEBUG_LOW_LOG
		sec_debug_hw_reset();
#endif
		return -EPERM;
	}
	len = strnlen(buf, 15);
	if (!strncmp(buf, "User Fault", len))
		sec_debug_set_upload_cause(UPLOAD_CAUSE_USER_FAULT);
	else if (!strncmp(buf, "Crash Key", len))
		sec_debug_set_upload_cause(UPLOAD_CAUSE_FORCED_UPLOAD);
	else if (!strncmp(buf, "CP Crash", len))
		sec_debug_set_upload_cause(UPLOAD_CAUSE_CP_ERROR_FATAL);
	else if (!strncmp(buf, "MDM Crash", len))
		sec_debug_set_upload_cause(UPLOAD_CAUSE_MDM_ERROR_FATAL);
	else if (strnstr(buf, "external_modem", len) != NULL)
		sec_debug_set_upload_cause(UPLOAD_CAUSE_MDM_ERROR_FATAL);
	else if (strnstr(buf, "modem", len) != NULL)
		sec_debug_set_upload_cause(UPLOAD_CAUSE_MODEM_RST_ERR);
	else if (strnstr(buf, "riva", len) != NULL)
		sec_debug_set_upload_cause(UPLOAD_CAUSE_RIVA_RST_ERR);
	else if (strnstr(buf, "lpass", len) != NULL)
		sec_debug_set_upload_cause(UPLOAD_CAUSE_LPASS_RST_ERR);
	else if (strnstr(buf, "dsps", len) != NULL)
		sec_debug_set_upload_cause(UPLOAD_CAUSE_DSPS_RST_ERR);
	else if (!strnicmp(buf, "subsys", len))
		sec_debug_set_upload_cause(UPLOAD_CAUSE_PERIPHERAL_ERR);
	else
		sec_debug_set_upload_cause(UPLOAD_CAUSE_KERNEL_PANIC);

/* enable after SSR feature
	ssr_panic_handler_for_sec_dbg();
*/
	sec_debug_dump_stack();
	sec_debug_hw_reset();
	return 0;
}

/*
 * Called from dump_stack()
 * This function call does not necessarily mean that a fatal error
 * had occurred. It may be just a warning.
 */
int sec_debug_dump_stack(void)
{
	if (!enable)
		return -EPERM;

	sec_debug_save_context();

	/* flush L1 from each core.
	   L2 will be flushed later before reset. */
	flush_cache_all();
	return 0;
}
EXPORT_SYMBOL(sec_debug_dump_stack);

void sec_debug_check_crash_key(unsigned int code, int value)
{
	static enum { NONE, STEP1, STEP2, STEP3} state = NONE;
	pr_info("[%s] code(0x%x), value(%d)\n", __func__, code, value);

	if (!enable)
		return;

	switch (state) {
	case NONE:
		if (code == KEY_VOLUMEDOWN && value)
			state = STEP1;
		else
			state = NONE;
		break;
	case STEP1:
		if (code == KEY_POWER && value)
			state = STEP2;
		else
			state = NONE;
		break;
	case STEP2:
		if (code == KEY_POWER && !value)
			state = STEP3;
		else
			state = NONE;
		break;
	case STEP3:
		if (code == KEY_POWER && value) {
			emerg_pet_watchdog();
			dump_all_task_info();
			dump_cpu_stat();
			panic("Crash Key");
		} else {
			state = NONE;
		}
		break;
	}
}

static struct notifier_block nb_reboot_block = {
	.notifier_call = sec_debug_normal_reboot_handler
};

static struct notifier_block nb_panic_block = {
	.notifier_call = sec_debug_panic_handler,
};

static void sec_debug_set_build_info(void)
{
	char *p = sec_build_info;
	strlcat(p, "Kernel Build Info : ", sizeof(sec_build_info));
	strlcat(p, "Date:", sizeof(sec_build_info));
	strlcat(p, sec_build_time[0], sizeof(sec_build_info));
	strlcat(p, "Time:", sizeof(sec_build_info));
	strlcat(p, sec_build_time[1], sizeof(sec_build_info));
}

static int __init __init_sec_debug_log(void)
{
	int i;
	struct sec_debug_log *vaddr;
	int size;

	if (secdbg_paddr == 0 || secdbg_size == 0) {
		pr_info("%s: sec debug buffer not provided. Using kmalloc..\n",
			__func__);
		size = sizeof(struct sec_debug_log);
		vaddr = kmalloc(size, GFP_KERNEL);
	} else {
		size = secdbg_size;
		vaddr = ioremap_nocache(secdbg_paddr, secdbg_size);
	}

	pr_info("%s: vaddr=0x%x paddr=0x%x size=0x%x "\
		"sizeof(struct sec_debug_log)=0x%x\n", __func__,
		(unsigned int)vaddr, secdbg_paddr, secdbg_size,
		sizeof(struct sec_debug_log));

	if ((vaddr == NULL) || (sizeof(struct sec_debug_log) > size)) {
		pr_info("%s: ERROR! init failed!\n", __func__);
		return -EFAULT;
	}

	memset(vaddr->sched, 0x0, sizeof(struct sched_log));
	memset(vaddr->irq, 0x0, sizeof(struct irq_log));
	memset(vaddr->irq_exit, 0x0, sizeof(struct irq_exit_log));
	memset(vaddr->timer_log, 0x0, sizeof(struct timer_log));
#ifdef CONFIG_SEC_DEBUG_MSGLOG
	memset(vaddr->secmsg, 0x0, sizeof(struct secmsg_log));
#endif
#ifdef CONFIG_SEC_DEBUG_AVC_LOG
	memset(vaddr->secavc, 0x0, sizeof(struct secavc_log));
#endif

	for (i = 0; i < CONFIG_NR_CPUS; i++) {
		atomic_set(&(vaddr->idx_sched[i]), -1);
		atomic_set(&(vaddr->idx_irq[i]), -1);
		atomic_set(&(vaddr->idx_irq_exit[i]), -1);
		atomic_set(&(vaddr->idx_timer[i]), -1);
#ifdef CONFIG_SEC_DEBUG_MSG_LOG
		atomic_set(&(vaddr->idx_secmsg[i]), -1);
#endif
#ifdef CONFIG_SEC_DEBUG_AVC_LOG
		atomic_set(&(vaddr->idx_secavc[i]), -1);
#endif
	}
#ifdef CONFIG_SEC_DEBUG_DCVS_LOG
	for (i = 0; i < CONFIG_NR_CPUS; i++)
		atomic_set(&(vaddr->dcvs_log_idx[i]), -1);
#endif
#ifdef CONFIG_SEC_DEBUG_FUELGAUGE_LOG
		atomic_set(&(vaddr->fg_log_idx), -1);
#endif

	secdbg_log = vaddr;

	pr_info("%s: init done\n", __func__);

	return 0;
}

#ifdef CONFIG_SEC_DEBUG_SUBSYS
int sec_debug_save_die_info(const char *str, struct pt_regs *regs)
{
	if (!secdbg_krait)
		return -ENOMEM;
	snprintf(secdbg_krait->excp.pc_sym, sizeof(secdbg_krait->excp.pc_sym),
		"%pS", (void *)regs->ARM_pc);
	snprintf(secdbg_krait->excp.lr_sym, sizeof(secdbg_krait->excp.lr_sym),
		"%pS", (void *)regs->ARM_lr);

	return 0;
}

int sec_debug_save_panic_info(const char *str, unsigned int caller)
{
	if (!secdbg_krait)
		return -ENOMEM;
	snprintf(secdbg_krait->excp.panic_caller,
		sizeof(secdbg_krait->excp.panic_caller), "%pS", (void *)caller);
	snprintf(secdbg_krait->excp.panic_msg,
		sizeof(secdbg_krait->excp.panic_msg), "%s", str);
	snprintf(secdbg_krait->excp.thread,
		sizeof(secdbg_krait->excp.thread), "%s:%d", current->comm,
		task_pid_nr(current));

	return 0;
}

int sec_debug_subsys_add_infomon(char *name, unsigned int size, unsigned int pa)
{
	if (!secdbg_krait)
		return -ENOMEM;

	if (secdbg_krait->info_mon.idx >= ARRAY_SIZE(secdbg_krait->info_mon.var))
		return -ENOMEM;

	strlcpy(secdbg_krait->info_mon.var[secdbg_krait->info_mon.idx].name,
		name, sizeof(secdbg_krait->info_mon.var[0].name));
	secdbg_krait->info_mon.var[secdbg_krait->info_mon.idx].sizeof_type
		= size;
	secdbg_krait->info_mon.var[secdbg_krait->info_mon.idx].var_paddr = pa;

	secdbg_krait->info_mon.idx++;

	return 0;
}

int sec_debug_subsys_add_varmon(char *name, unsigned int size, unsigned int pa)
{
	if (!secdbg_krait)
		return -ENOMEM;

	if (secdbg_krait->var_mon.idx > ARRAY_SIZE(secdbg_krait->var_mon.var))
		return -ENOMEM;

	strlcpy(secdbg_krait->var_mon.var[secdbg_krait->var_mon.idx].name, name,
		sizeof(secdbg_krait->var_mon.var[0].name));
	secdbg_krait->var_mon.var[secdbg_krait->var_mon.idx].sizeof_type = size;
	secdbg_krait->var_mon.var[secdbg_krait->var_mon.idx].var_paddr = pa;

	secdbg_krait->var_mon.idx++;

	return 0;
}

#ifdef CONFIG_SEC_DEBUG_MDM_FILE_INFO
void sec_set_mdm_subsys_info(char *str_buf)
{
	snprintf(secdbg_krait->mdmerr_info,
		sizeof(secdbg_krait->mdmerr_info), "%s", str_buf);
}
#endif
static int ___build_root_init(char *str)
{
	char *st, *ed;
	int len;
	ed = strstr(str, "/android/kernel");
	if (!ed || ed == str)
		return -1;
	*ed = '\0';
	st = strrchr(str, '/');
	if (!st)
		return -1;
	st++;
	len = (unsigned long)ed - (unsigned long)st + 1;
	memmove(str, st, len);
	return 0;
}

int sec_debug_subsys_init(void)
{
	pr_info("%s: msm_shared_ram_phys=%x SMEM_ID_VENDOR2=%d size=%d\n",
		__func__, msm_shared_ram_phys,  SMEM_ID_VENDOR2,
		sizeof(struct sec_debug_subsys));

	secdbg_subsys = (struct sec_debug_subsys *)smem_alloc2(
		SMEM_ID_VENDOR2,
		sizeof(struct sec_debug_subsys));

	if (secdbg_subsys == NULL) {
		pr_info("%s: smem alloc failed!\n", __func__);
		return -ENOMEM;
	}

	secdbg_krait = &secdbg_subsys->priv.krait;

	secdbg_subsys->krait = (struct sec_debug_subsys_data_krait *)(
		(unsigned int)&secdbg_subsys->priv.krait -
		(unsigned int)MSM_SHARED_RAM_BASE + msm_shared_ram_phys);
	secdbg_subsys->rpm = (struct sec_debug_subsys_data *)(
		(unsigned int)&secdbg_subsys->priv.rpm -
		(unsigned int)MSM_SHARED_RAM_BASE + msm_shared_ram_phys);
	secdbg_subsys->modem = (struct sec_debug_subsys_data_modem *)(
		(unsigned int)&secdbg_subsys->priv.modem -
		(unsigned int)MSM_SHARED_RAM_BASE + msm_shared_ram_phys);
	secdbg_subsys->dsps = (struct sec_debug_subsys_data *)(
		(unsigned int)&secdbg_subsys->priv.dsps -
		(unsigned int)MSM_SHARED_RAM_BASE + msm_shared_ram_phys);

	pr_info("%s: krait(%x) rpm(%x) modem(%x) dsps(%x)\n", __func__,
		(unsigned int)secdbg_subsys->krait,
		(unsigned int)secdbg_subsys->rpm,
		(unsigned int)secdbg_subsys->modem,
		(unsigned int)secdbg_subsys->dsps);

	strlcpy(secdbg_krait->name, "Krait", sizeof(secdbg_krait->name) + 1);
	strlcpy(secdbg_krait->state, "Init", sizeof(secdbg_krait->state) + 1);
	secdbg_krait->nr_cpus = CONFIG_NR_CPUS;

	sec_debug_subsys_set_kloginfo(&secdbg_krait->log.idx_paddr,
		&secdbg_krait->log.log_paddr, &secdbg_krait->log.size);
	sec_debug_subsys_set_logger_info(&secdbg_krait->logger_log);

	secdbg_krait->tz_core_dump =
		(struct tzbsp_dump_buf_s **)get_wdog_regsave_paddr();
	get_fbinfo(0, &secdbg_krait->fb_info.fb_paddr,
		&secdbg_krait->fb_info.xres,
		&secdbg_krait->fb_info.yres,
		&secdbg_krait->fb_info.bpp,
		&secdbg_krait->fb_info.rgb_bitinfo.r_off,
		&secdbg_krait->fb_info.rgb_bitinfo.r_len,
		&secdbg_krait->fb_info.rgb_bitinfo.g_off,
		&secdbg_krait->fb_info.rgb_bitinfo.g_len,
		&secdbg_krait->fb_info.rgb_bitinfo.b_off,
		&secdbg_krait->fb_info.rgb_bitinfo.b_len,
		&secdbg_krait->fb_info.rgb_bitinfo.a_off,
		&secdbg_krait->fb_info.rgb_bitinfo.a_len);

	ADD_STR_TO_INFOMON(unit_name);
	ADD_VAR_TO_INFOMON(system_rev);
	if (___build_root_init(build_root) == 0)
		ADD_STR_TO_INFOMON(build_root);
	ADD_STR_TO_INFOMON(linux_banner);

	ADD_VAR_TO_VARMON(boost_uv);
	ADD_VAR_TO_VARMON(speed_bin);
	ADD_VAR_TO_VARMON(pvs_bin);

	if (secdbg_paddr) {
		secdbg_krait->sched_log.sched_idx_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, idx_sched);
		secdbg_krait->sched_log.sched_buf_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, sched);
		secdbg_krait->sched_log.sched_struct_sz =
			sizeof(struct sched_log);
		secdbg_krait->sched_log.sched_array_cnt = SCHED_LOG_MAX;

		secdbg_krait->sched_log.irq_idx_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, idx_irq);
		secdbg_krait->sched_log.irq_buf_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, irq);
		secdbg_krait->sched_log.irq_struct_sz =
			sizeof(struct irq_log);
		secdbg_krait->sched_log.irq_array_cnt = SCHED_LOG_MAX;

		secdbg_krait->sched_log.irq_exit_idx_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, idx_irq_exit);
		secdbg_krait->sched_log.irq_exit_buf_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, irq_exit);
		secdbg_krait->sched_log.irq_exit_struct_sz =
			sizeof(struct irq_exit_log);
		secdbg_krait->sched_log.irq_exit_array_cnt = SCHED_LOG_MAX;

#ifdef CONFIG_SEC_DEBUG_MSG_LOG
		secdbg_krait->sched_log.msglog_idx_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, idx_secmsg);
		secdbg_krait->sched_log.msglog_buf_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, secmsg);
		secdbg_krait->sched_log.msglog_struct_sz =
			sizeof(struct secmsg_log);
		secdbg_krait->sched_log.msglog_array_cnt = MSG_LOG_MAX;
#else
		secdbg_krait->sched_log.msglog_idx_paddr = 0;
		secdbg_krait->sched_log.msglog_buf_paddr = 0;
		secdbg_krait->sched_log.msglog_struct_sz = 0;
		secdbg_krait->sched_log.msglog_array_cnt = 0;
#endif

#ifdef CONFIG_SEC_DEBUG_AVC_LOG
		secdbg_krait->avc_log.secavc_idx_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, idx_secavc);
		secdbg_krait->avc_log.secavc_buf_paddr = secdbg_paddr +
			offsetof(struct sec_debug_log, secavc);
		secdbg_krait->avc_log.secavc_struct_sz =
			sizeof(struct secavc_log);
		secdbg_krait->avc_log.secavc_array_cnt = AVC_LOG_MAX;
#else
		secdbg_krait->avc_log.secavc_idx_paddr = 0;
		secdbg_krait->avc_log.secavc_buf_paddr = 0;
		secdbg_krait->avc_log.secavc_struct_sz = 0;
		secdbg_krait->avc_log.secavc_array_cnt = 0;
#endif
	}

	/* fill magic nubmer last to ensure data integrity when the magic
	 * numbers are written
	 */
	secdbg_subsys->magic[0] = SEC_DEBUG_SUBSYS_MAGIC0;
	secdbg_subsys->magic[1] = SEC_DEBUG_SUBSYS_MAGIC1;
	secdbg_subsys->magic[2] = SEC_DEBUG_SUBSYS_MAGIC2;
	secdbg_subsys->magic[3] = SEC_DEBUG_SUBSYS_MAGIC3;
	return 0;
}
late_initcall(sec_debug_subsys_init);
#endif

int __init sec_debug_init(void)
{
	restart_reason = MSM_IMEM_BASE + RESTART_REASON_ADDR;

	pr_emerg("%s: enable=%d\n", __func__, enable);

	/* check restart_reason here */
	pr_emerg("%s: restart_reason : 0x%x\n", __func__,
		(unsigned int)restart_reason);

	register_reboot_notifier(&nb_reboot_block);
	atomic_notifier_chain_register(&panic_notifier_list, &nb_panic_block);

	if (!enable)
		return -EPERM;

#ifdef CONFIG_SEC_DEBUG_SCHED_LOG
	__init_sec_debug_log();
#endif

	debug_semaphore_init();
	sec_debug_set_build_info();
	sec_debug_set_upload_magic(0x776655ee);
	sec_debug_set_upload_cause(UPLOAD_CAUSE_INIT);

	return 0;
}

int sec_debug_is_enabled(void)
{
	return enable;
}

/* klaatu - schedule log */
#ifdef CONFIG_SEC_DEBUG_SCHED_LOG
void __sec_debug_task_sched_log(int cpu, struct task_struct *task,
						char *msg)
{
	unsigned i;

	if (!secdbg_log)
		return;

	if (!task && !msg)
		return;

	i = atomic_inc_return(&(secdbg_log->idx_sched[cpu]))
		& (SCHED_LOG_MAX - 1);
	secdbg_log->sched[cpu][i].time = cpu_clock(cpu);
	if (task) {
		strlcpy(secdbg_log->sched[cpu][i].comm, task->comm,
			sizeof(secdbg_log->sched[cpu][i].comm));
		secdbg_log->sched[cpu][i].pid = task->pid;
	} else {
		strlcpy(secdbg_log->sched[cpu][i].comm, msg,
			sizeof(secdbg_log->sched[cpu][i].comm));
		secdbg_log->sched[cpu][i].pid = -1;
	}
}

void sec_debug_task_sched_log_short_msg(char *msg)
{
	__sec_debug_task_sched_log(smp_processor_id(), NULL, msg);
}

void sec_debug_task_sched_log(int cpu, struct task_struct *task)
{
	__sec_debug_task_sched_log(cpu, task, NULL);
}

void sec_debug_timer_log(unsigned int type, int int_lock, void *fn)
{
	int cpu = smp_processor_id();
	unsigned i;

	if (!secdbg_log)
		return;

	i = atomic_inc_return(&(secdbg_log->idx_timer[cpu]))
		& (SCHED_LOG_MAX - 1);
	secdbg_log->timer_log[cpu][i].time = cpu_clock(cpu);
	secdbg_log->timer_log[cpu][i].type = type;
	secdbg_log->timer_log[cpu][i].int_lock = int_lock;
	secdbg_log->timer_log[cpu][i].fn = (void *)fn;
}

void sec_debug_irq_sched_log(unsigned int irq, void *fn, int en)
{
	int cpu = smp_processor_id();
	unsigned i;

	if (!secdbg_log)
		return;

	i = atomic_inc_return(&(secdbg_log->idx_irq[cpu]))
		& (SCHED_LOG_MAX - 1);
	secdbg_log->irq[cpu][i].time = cpu_clock(cpu);
	secdbg_log->irq[cpu][i].irq = irq;
	secdbg_log->irq[cpu][i].fn = (void *)fn;
	secdbg_log->irq[cpu][i].en = en;
	secdbg_log->irq[cpu][i].preempt_count = preempt_count();
	secdbg_log->irq[cpu][i].context = &cpu;
}

#ifdef CONFIG_SEC_DEBUG_IRQ_EXIT_LOG
void sec_debug_irq_enterexit_log(unsigned int irq,
					unsigned long long start_time)
{
	int cpu = smp_processor_id();
	unsigned i;

	if (!secdbg_log)
		return;

	i = atomic_inc_return(&(secdbg_log->idx_irq_exit[cpu]))
		& (SCHED_LOG_MAX - 1);
	secdbg_log->irq_exit[cpu][i].time = start_time;
	secdbg_log->irq_exit[cpu][i].end_time = cpu_clock(cpu);
	secdbg_log->irq_exit[cpu][i].irq = irq;
	secdbg_log->irq_exit[cpu][i].elapsed_time =
		secdbg_log->irq_exit[cpu][i].end_time - start_time;
}
#endif

#ifdef CONFIG_SEC_DEBUG_MSG_LOG
asmlinkage int sec_debug_msg_log(void *caller, const char *fmt, ...)
{
	int cpu = smp_processor_id();
	int r = 0;
	int i;
	va_list args;

	if (!secdbg_log)
		return 0;

	i = atomic_inc_return(&(secdbg_log->idx_secmsg[cpu]))
		& (MSG_LOG_MAX - 1);
	secdbg_log->secmsg[cpu][i].time = cpu_clock(cpu);
	va_start(args, fmt);
	r = vsnprintf(secdbg_log->secmsg[cpu][i].msg,
		sizeof(secdbg_log->secmsg[cpu][i].msg), fmt, args);
	va_end(args);

	secdbg_log->secmsg[cpu][i].caller0 = __builtin_return_address(0);
	secdbg_log->secmsg[cpu][i].caller1 = caller;
	secdbg_log->secmsg[cpu][i].task = current->comm;

	return r;
}

#endif

#ifdef CONFIG_SEC_DEBUG_AVC_LOG
asmlinkage int sec_debug_avc_log(const char *fmt, ...)
{
	int cpu = smp_processor_id();
	int r = 0;
	int i;
	va_list args;

	if (!secdbg_log)
		return 0;

	i = atomic_inc_return(&(secdbg_log->idx_secavc[cpu]))
		& (AVC_LOG_MAX - 1);
	va_start(args, fmt);
	r = vsnprintf(secdbg_log->secavc[cpu][i].msg,
		sizeof(secdbg_log->secavc[cpu][i].msg), fmt, args);
	va_end(args);

	return r;
}

#endif
#endif	/* CONFIG_SEC_DEBUG_SCHED_LOG */

/* klaatu - semaphore log */
#ifdef CONFIG_SEC_DEBUG_SEMAPHORE_LOG
void debug_semaphore_init(void)
{
	int i = 0;
	struct sem_debug *sem_debug = NULL;

	spin_lock_init(&sem_debug_lock);
	sem_debug_free_head_cnt = 0;
	sem_debug_done_head_cnt = 0;

	/* initialize list head of sem_debug */
	INIT_LIST_HEAD(&sem_debug_free_head.list);
	INIT_LIST_HEAD(&sem_debug_done_head.list);

	for (i = 0; i < SEMAPHORE_LOG_MAX; i++) {
		/* malloc semaphore */
		sem_debug = kmalloc(sizeof(struct sem_debug), GFP_KERNEL);

		/* add list */
		list_add(&sem_debug->list, &sem_debug_free_head.list);
		sem_debug_free_head_cnt++;
	}

	sem_debug_init = 1;
}

void debug_semaphore_down_log(struct semaphore *sem)
{
	struct list_head *tmp;
	struct sem_debug *sem_dbg;
	unsigned long flags;

	if (!sem_debug_init)
		return;

	spin_lock_irqsave(&sem_debug_lock, flags);
	list_for_each(tmp, &sem_debug_free_head.list) {
		sem_dbg = list_entry(tmp, struct sem_debug, list);
		sem_dbg->task = current;
		sem_dbg->sem = sem;
		sem_dbg->pid = current->pid;
		sem_dbg->cpu = smp_processor_id();
		list_del(&sem_dbg->list);
		list_add(&sem_dbg->list, &sem_debug_done_head.list);
		sem_debug_free_head_cnt--;
		sem_debug_done_head_cnt++;
		break;
	}
	spin_unlock_irqrestore(&sem_debug_lock, flags);
}

void debug_semaphore_up_log(struct semaphore *sem)
{
	struct list_head *tmp;
	struct sem_debug *sem_dbg;
	unsigned long flags;

	if (!sem_debug_init)
		return;

	spin_lock_irqsave(&sem_debug_lock, flags);
	list_for_each(tmp, &sem_debug_done_head.list) {
		sem_dbg = list_entry(tmp, struct sem_debug, list);
		if (sem_dbg->sem == sem && sem_dbg->pid == current->pid) {
			list_del(&sem_dbg->list);
			list_add(&sem_dbg->list, &sem_debug_free_head.list);
			sem_debug_free_head_cnt++;
			sem_debug_done_head_cnt--;
			break;
		}
	}
	spin_unlock_irqrestore(&sem_debug_lock, flags);
}

/* rwsemaphore logging */
void debug_rwsemaphore_init(void)
{
	int i = 0;
	struct rwsem_debug *rwsem_debug = NULL;
	spin_lock_init(&rwsem_debug_lock);
	rwsem_debug_free_head_cnt = 0;
	rwsem_debug_done_head_cnt = 0;

	/* initialize list head of sem_debug */
	INIT_LIST_HEAD(&rwsem_debug_free_head.list);
	INIT_LIST_HEAD(&rwsem_debug_done_head.list);

	for (i = 0; i < RWSEMAPHORE_LOG_MAX; i++) {
		/* malloc semaphore */
		rwsem_debug =
			kmalloc(sizeof(struct rwsem_debug), GFP_KERNEL);
		/* add list */
		list_add(&rwsem_debug->list, &rwsem_debug_free_head.list);
		rwsem_debug_free_head_cnt++;
	}

	rwsem_debug_init = 1;
}

void debug_rwsemaphore_down_log(struct rw_semaphore *sem, int dir)
{
	struct list_head *tmp;
	struct rwsem_debug *sem_dbg;
	unsigned long flags;

	if (!rwsem_debug_init)
		return;

	spin_lock_irqsave(&rwsem_debug_lock, flags);
	list_for_each(tmp, &rwsem_debug_free_head.list) {
		sem_dbg = list_entry(tmp, struct rwsem_debug, list);
		sem_dbg->task = current;
		sem_dbg->sem = sem;
		sem_dbg->pid = current->pid;
		sem_dbg->cpu = smp_processor_id();
		sem_dbg->direction = dir;
		list_del(&sem_dbg->list);
		list_add(&sem_dbg->list, &rwsem_debug_done_head.list);
		rwsem_debug_free_head_cnt--;
		rwsem_debug_done_head_cnt++;
		break;
	}
	spin_unlock_irqrestore(&rwsem_debug_lock, flags);
}

void debug_rwsemaphore_up_log(struct rw_semaphore *sem)
{
	struct list_head *tmp;
	struct rwsem_debug *sem_dbg;
	unsigned long flags;

	if (!rwsem_debug_init)
		return;

	spin_lock_irqsave(&rwsem_debug_lock, flags);
	list_for_each(tmp, &rwsem_debug_done_head.list) {
		sem_dbg = list_entry(tmp, struct rwsem_debug, list);
		if (sem_dbg->sem == sem && sem_dbg->pid == current->pid) {
			list_del(&sem_dbg->list);
			list_add(&sem_dbg->list, &rwsem_debug_free_head.list);
			rwsem_debug_free_head_cnt++;
			rwsem_debug_done_head_cnt--;
			break;
		}
	}
	spin_unlock_irqrestore(&rwsem_debug_lock, flags);
}
#endif	/* CONFIG_SEC_DEBUG_SEMAPHORE_LOG */

static int __init sec_dbg_setup(char *str)
{
	unsigned size = memparse(str, &str);

	pr_emerg("%s: str=%s\n", __func__, str);

	if (size && (size == roundup_pow_of_two(size)) && (*str == '@')) {
		secdbg_paddr = (unsigned int)memparse(++str, NULL);
		secdbg_size = size;
	}

	pr_emerg("%s: secdbg_paddr = 0x%x\n", __func__, secdbg_paddr);
	pr_emerg("%s: secdbg_size = 0x%x\n", __func__, secdbg_size);

	return 1;
}

__setup("sec_dbg=", sec_dbg_setup);


static void sec_user_fault_dump(void)
{
	if (enable == 1 && enable_user == 1)
		panic("User Fault");
}

static int sec_user_fault_write(struct file *file, const char __user *buffer,
		size_t count, loff_t *offs)
{
	char buf[100];

	if (count > sizeof(buf) - 1)
		return -EINVAL;
	if (copy_from_user(buf, buffer, count))
		return -EFAULT;
	buf[count] = '\0';

	if (strncmp(buf, "dump_user_fault", 15) == 0)
		sec_user_fault_dump();

	return count;
}

static const struct file_operations sec_user_fault_proc_fops = {
	.write = sec_user_fault_write,
};

static int __init sec_debug_user_fault_init(void)
{
	struct proc_dir_entry *entry;

	entry = proc_create("user_fault", S_IWUSR|S_IWGRP, NULL,
			&sec_user_fault_proc_fops);
	if (!entry)
		return -ENOMEM;
	return 0;
}
device_initcall(sec_debug_user_fault_init);
#ifdef CONFIG_SEC_DEBUG_DCVS_LOG
void sec_debug_dcvs_log(int cpu_no, unsigned int prev_freq,
						unsigned int new_freq)
{
	unsigned int i;
	if (!secdbg_log)
		return;

	i = atomic_inc_return(&(secdbg_log->dcvs_log_idx[cpu_no]))
		& (DCVS_LOG_MAX - 1);
	secdbg_log->dcvs_log[cpu_no][i].cpu_no = cpu_no;
	secdbg_log->dcvs_log[cpu_no][i].prev_freq = prev_freq;
	secdbg_log->dcvs_log[cpu_no][i].new_freq = new_freq;
	secdbg_log->dcvs_log[cpu_no][i].time = cpu_clock(cpu_no);
}
#endif
#ifdef CONFIG_SEC_DEBUG_FUELGAUGE_LOG
void sec_debug_fuelgauge_log(unsigned int voltage, unsigned short soc,
				unsigned short charging_status)
{
	unsigned int i;
	int cpu = smp_processor_id();

	if (!secdbg_log)
		return;

	i = atomic_inc_return(&(secdbg_log->fg_log_idx))
		& (FG_LOG_MAX - 1);
	secdbg_log->fg_log[i].time = cpu_clock(cpu);
	secdbg_log->fg_log[i].voltage = voltage;
	secdbg_log->fg_log[i].soc = soc;
	secdbg_log->fg_log[i].charging_status = charging_status;
}
#endif
