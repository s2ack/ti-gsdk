#Cross compiler toolchain path 
CROSS_COMPILE = /opt/toolchain/arago/arago-2011.09-static-armv7a-linux-gnueabi-sdk/arago-2011.09/armv7a/bin/arm-arago-linux-gnueabi-
#Path to kernel(Linux PSP release)
KERNEL_DIR    = /home/user/omap3-kernel/linux-04.02.00.07
#NFS path where the executable needs to be installed
TGTFS_PATH    = /home/user/nfs/opt
#Graphics SDK installation path
GSDK_ROOT     = /home/user/Graphics_SDK_4_06_00_01
#CMEM installation path
CMEM_DIR      = /home/user/cmem

# no need to touch from here
GLES_DIR      = $(GSDK_ROOT)/include/OGLES
GLES2_DIR     = $(GSDK_ROOT)/include/OGLES2
GSDK_KM_DIR   = $(GSDK_ROOT)/GFX_Linux_KM

CC = $(CROSS_COMPILE)gcc
