/*
 *  linux/init/version.c
 *
 *  Copyright (C) 1992  Theodore Ts'o
 *
 *  May be freely distributed as part of Linux.
 */

#include <generated/compile.h>
#include <linux/module.h>
#include <linux/uts.h>
#include <linux/utsname.h>
#include <generated/utsrelease.h>
#include <linux/version.h>

#ifndef CONFIG_KALLSYMS
#define version(a) Version_ ## a
#define version_string(a) version(a)

extern int version_string(LINUX_VERSION_CODE);
int version_string(LINUX_VERSION_CODE);
#endif

struct uts_namespace init_uts_ns = {
	.kref = {
		.refcount	= ATOMIC_INIT(2),
	},
	.name = {
		.sysname	= UTS_SYSNAME,
		.nodename	= UTS_NODENAME,
		.release	= UTS_RELEASE,
		.version	= UTS_VERSION,
		.machine	= UTS_MACHINE,
		.domainname	= UTS_DOMAINNAME,
	},
	.user_ns = &init_user_ns,
};
EXPORT_SYMBOL_GPL(init_uts_ns);

/*  Google REQUIREMENT **
 *  GPE Device Build Requirements doc calls out that the kernel version string 
 *  should have '(android@gpe)' included in it. Here is the text.
 * 
 *  Kernel Version String
 *  For the purpose of integration with Android issue reporting used in internal dogfooding, 
 *  the kernel version string reported in /proc/version on device must contain substring '(android@gpe)'. 
 *  There are no further requirements on the specific position of such substring within the entire kernel version st
*/
#if defined(CONFIG_GED_BUILD)
#if defined(LINUX_COMPILE_BY)
#undef LINUX_COMPILE_BY
#define LINUX_COMPILE_BY	"android"
#endif /* LINUX_COMPILE_BY */
#if defined(LINUX_COMPILE_HOST)
#undef LINUX_COMPILE_HOST
#define LINUX_COMPILE_HOST	"gpe"
#endif /* LINUX_COMPILE_HOST */
#endif /* CONFIG_GED_BUILD */

/* FIXED STRINGS! Don't touch! */
const char linux_banner[] =
	"Linux version " UTS_RELEASE " (" LINUX_COMPILE_BY "@"
	LINUX_COMPILE_HOST ") (" LINUX_COMPILER ") " UTS_VERSION "\n";

const char linux_proc_banner[] =
	"%s version %s"
	" (" LINUX_COMPILE_BY "@" LINUX_COMPILE_HOST ")"
	" (" LINUX_COMPILER ") %s\n";
