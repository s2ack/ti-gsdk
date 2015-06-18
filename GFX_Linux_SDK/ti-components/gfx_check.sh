#!/bin/sh
echo "WSEGL settings"
cat /etc/powervr.ini
echo "------"
echo "ARM CPU information"
cat /proc/cpuinfo
echo "------"
echo "SGX driver information"
cat /proc/pvr/version
echo "------"
echo "Framebuffer settings"
fbset -i
echo "------"
echo "Rotation settings"
cat /sys/class/graphics/fb0/rotate
echo "------"
echo "Kernel Module information"
lsmod
echo "------"
echo "Boot settings"
cat /proc/cmdline
echo "------"
echo "Linux Kernel version"
uname -a

