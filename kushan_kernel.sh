#!/bin/sh

KERNEL_DIR=$(pwd)
DATE="`date +"%d-%m-%Y"`"
BUILD_JOB_NUMBER=`grep processor /proc/cpuinfo|wc -l`
OUTPUT=BUILD_$DATE

make clean
mkdir $KERNEL_DIR/BUILD_$DATE
mkdir $KERNEL_DIR/BUILD_$DATE/modules
export ARCH=arm
export SUBARCH=arm
export CCOMPILE=$CROSS_COMPILE
#export CROSS_COMPILE=arm-cortex_a15-linux-gnueabihf-
#export PATH=$PATH:/home/kushan/toolchain/linaro/bin
export ccache=ccache
export USE_SEC_FIPS_MODE=true
export KCONFIG_NOTIMESTAMP=true
export ENABLE_GRAPHITE=true
#export CROSS_COMPILE=/home/kushan/toolchain/ubertc/bin/arm-eabi-
export CROSS_COMPILE=/home/kushan/toolchain/linaro/bin/arm-eabi-

make kushan_defconfig VARIANT_DEFCONFIG=cyanogen_jf_defconfig SELINUX_DEFCONFIG=selinux_defconfig
make -j$BUILD_JOB_NUMBER
cp -r $KERNEL_DIR/arch/arm/boot/zImage $OUTPUT/zImage
find . -name "*.ko" -exec cp {} $OUTPUT/modules/ \;
echo ""
echo "Built Kushan Kernel Image and modules"
