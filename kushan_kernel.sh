#!/bin/sh
# COPYRIGHT kushan02- Kushan Mehta
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
export ccache=ccache
export USE_SEC_FIPS_MODE=true
export KCONFIG_NOTIMESTAMP=true
export ENABLE_GRAPHITE=true
export CROSS_COMPILE=/home/kushan/toolchain/linaro/bin/arm-eabi-
make kushan_defconfig
make -j$BUILD_JOB_NUMBER
cp -r $KERNEL_DIR/arch/arm/boot/zImage $OUTPUT/zImage
find . -name "*.ko" -exec cp {} $OUTPUT/modules/ \;
echo ""
echo "Built Kushan Kernel Image and modules"
