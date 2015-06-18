# Replace below variables with the paths for graphics SDK as on your system
GRAPHICS_SDK_BASE = /home/user/Graphics_SDK_4_06_00_01
GRAPHICS_SDK_XORG_BASE = $(GRAPHICS_SDK_BASE)/targetfs/XSGX
TOOLCHAIN_PATH = /opt/toolchain/arago/arago-2011.09-static-armv7a-linux-gnueabi-sdk-latest/arago-2011.09/armv7a/bin
CROSS_COMPILE = $(TOOLCHAIN_PATH)/arm-arago-linux-gnueabi-g++
#By default the setting below(gfx_rel_es5.x) is for DM/AM37xx EVM. Replace this with your device.
#You can get details on what library version to use below for your device by doing make help with graphics SDK build(Makefile) & not with this makefile
GRAPHICS_DEVICE_LIB = gfx_rel_es5.x

gl2_yuvtex: gl2_yuvtex.cpp
	$(CROSS_COMPILE)  -g -o gl2_yuvtex -I$(GRAPHICS_SDK_XORG_BASE)/include -I$(GRAPHICS_SDK_BASE)/include/OGLES2 gl2_yuvtex.cpp -L$(GRAPHICS_SDK_BASE)/$(GRAPHICS_DEVICE_LIB) -Wl,--rpath-link,$(GRAPHICS_SDK_BASE)/$(GRAPHICS_DEVICE_LIB):$(GRAPHICS_SDK_XORG_BASE)/lib -lEGL -lGLESv2 -L$(GRAPHICS_SDK_XORG_BASE)/lib -lX11

clean:
	rm -rf gl2_yuvtex
