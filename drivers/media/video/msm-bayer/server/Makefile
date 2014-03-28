GCC_VERSION      := $(shell $(CONFIG_SHELL) $(PWD)/scripts/gcc-version.sh $(CROSS_COMPILE)gcc)

ifeq ($(CONFIG_MSM_CAMERA_V4L2),y)
  EXTRA_CFLAGS += -Idrivers/media/video/msm-bayer
  EXTRA_CFLAGS += -Idrivers/media/video/msm-bayer/io
  EXTRA_CFLAGS += -Idrivers/media/video/msm-bayer/csi
  EXTRA_CFLAGS += -Idrivers/media/video/msm-bayer/eeprom
  EXTRA_CFLAGS += -Idrivers/media/video/msm-bayer/sensors
  EXTRA_CFLAGS += -Idrivers/media/video/msm-bayer/actuators
  obj-$(CONFIG_MSM_CAMERA)   += msm_cam_server.o
endif
