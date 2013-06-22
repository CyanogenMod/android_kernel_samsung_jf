GCC_VERSION      := $(shell $(CONFIG_SHELL) $(PWD)/scripts/gcc-version.sh $(CROSS_COMPILE)gcc)
EXTRA_CFLAGS += -Idrivers/media/video/msm
EXTRA_CFLAGS += -Idrivers/media/video/msm/io
obj-$(CONFIG_MSM_ACTUATOR) += msm_actuator.o
ifeq ($(CONFIG_MACH_JACTIVE_EUR)),y)
obj-$(CONFIG_MSM_ACTUATOR) += hvca_actuator.o
endif
ifeq ($(CONFIG_MACH_JACTIVE_ATT)),y)
obj-$(CONFIG_MSM_ACTUATOR) += hvca_actuator.o
endif