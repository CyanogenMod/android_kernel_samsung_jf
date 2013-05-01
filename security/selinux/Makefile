#
# Makefile for building the SELinux module as part of the kernel tree.
#

obj-$(CONFIG_SECURITY_SELINUX) := selinux.o

selinux-y := avc.o hooks.o selinuxfs.o netlink.o nlmsgtab.o netif.o \
	     netnode.o netport.o exports.o \
	     ss/ebitmap.o ss/hashtab.o ss/symtab.o ss/sidtab.o ss/avtab.o \
	     ss/policydb.o ss/services.o ss/conditional.o ss/mls.o ss/status.o

#TmmSecure start
ifeq ($(SEC_BUILD_OPTION_SELINUX_ENFORCE),false)
ifdef CONFIG_SECURITY_SEC
selinux-y += \
	sec/seclsm.o \
	sec/secfs.o \
	sec/secpolicy.o \
	sec/seccore.o \
	sec/secfunc.o \
	sec/seclog.o

EXTRA_CFLAGS += -DSECSUBLSM_ENABLE

endif
endif
#TmmSecure end


selinux-$(CONFIG_SECURITY_NETWORK_XFRM) += xfrm.o

selinux-$(CONFIG_NETLABEL) += netlabel.o

ccflags-y := -Isecurity/selinux -Isecurity/selinux/include

$(addprefix $(obj)/,$(selinux-y)): $(obj)/flask.h

quiet_cmd_flask = GEN     $(obj)/flask.h $(obj)/av_permissions.h
      cmd_flask = scripts/selinux/genheaders/genheaders $(obj)/flask.h $(obj)/av_permissions.h

targets += flask.h av_permissions.h
$(obj)/flask.h: $(src)/include/classmap.h FORCE
	$(call if_changed,flask)
