Release Notes for 5.01.01.02 release 

Introduction 

This release includes the Linux graphics drivers for the SGX530 family of chipsets 
AM35x/37xx, AM335x, 387x(TI814x)/389x(TI816x).The Graphics SDK contains 
documentation, demo programs and tools. 

Note: Sanity Tested on AM335x EVM & AM43x EVMs only - unit test report not 
included in the release package 

Installation and Usage 

Please look for detailed installation and usage instructions at the below link- 
http://processors.wiki.ti.com/index.php/Graphics_SDK_Quick_installation_and_user_
guide 

 ---


Features 

. NEW - SGX Display Class Driver Interface with DRM FBdev emulation - 
AM335x/AM43x only 
. NEW - 3.14 kernel support - Sanity tested on AM43x & AM335x GP-EVM only 
with kernel tree as mentioned in Pre-requisites section below. 
. Based on 1.10 RC DDK (1.10@2359475) 
. PowerVR SDK 3.0 


 ---


Pre-requisites 

Kernel(Linux PSP) versions supported or used for validation 

. AM335x & AM43x - 
. Tree: git://git.ti.com/ti-linux-kernel/ti-linux-kernel.git 
. Branch: ti-linux-3.14.y 




Toolchain 

. Linaro 4.7.3 hardfp toolchain(arm-linux-gnueabihf-) was used for creating this 
release package. 



What is supported 

Khronos API support 

. OpenGL ES 1.1 
. OpenGL ES 2.0 
. EGL 
. Limited 2D acceleration via PVR2D API 


Window Systems 

. Null Window System 


Buffer Swap modes 

The following modes of buffer swapping are supported, 

. Front (1 buffer) 
. Flip (chained buffers) 


Selection of these modes is done via entries in /etc/powervr.ini. Please refer 
to http://processors.wiki.ti.com/index.php/SGXDbg#WindowSystem 

Extensions 

. IMG propreitary Bufferclass texture streaming enabled by default. 


 ---


What is not supported 

. Kernel (for OMAP35x/AM35x/37xx) configured without DSS2 framework 
support will not work with the current display class driver. 
. Xorg driver build (SUPPORT_XORG=1) not supported. 
. Debug build is meant for debug/testing purposes only & should not be used 
for measuring performance. 


 ---



Debugging SGX driver related issues 

For debugging issues with SGX driver and frequently asked questions, please go 
through 
http://processors.wiki.ti.com/index.php/SGXDbg 

 ---


Fixed in this Release 

. None 


 ---


Limitations/ Known Issues 

. Warning when quitting applications using IMG propreitary Bufferclass texture 
extension - Issue in freeing of physically contiguous dma memory 


 ---


Contents 

The folders in the installed package contain binaries for several family of devices 
containing the SGX530 core. Brief description of these folders is below. 

 ---


Graphics SDK folder contents 

 

Folder Name 

Description 

Used by 

2 

gfx_dbg_es3.x / 
gfx_rel_es3.x 

Debug/Release binaries for 
platforms having SGX core 
revision 1.2.1 

(AM35xx) How to check? 

3 

gfx_dbg_es5.x / 

Debug/Release binaries for 
platforms having SGX core 

(AM37xx) How to check? 




gfx_rel_es5.x 

revision 1.2.5 

4 

gfx_dbg_es6.x / 
gfx_rel_es6.x 

Debug/Release binaries for 
38xx(387x,389x) devices (SGX 
core revision 1.2.5) 

38xx How to check? 

4 

gfx_dbg_es8.x / 
gfx_rel_es8.x 

Debug/Release binaries for 335x 
devices (SGX core revision 1.2.5) 

AM335x EVM 

4 

gfx_dbg_es9.x / 
gfx_rel_es9.x 

Debug/Release binaries for 43xx 
devices (SGX core revision 1.2.5) 

AM43x EVM 

5 

GFX_Linux_KM 

Source for Graphics Kernel 
Modules 

Contains kernel dependent code, changes 
depending on Linux kernel used 

6 

GFX_Linux_SDK 

Demos, Training courses, PVR 
Tools,classes and header files 

Used for building new applications (Available 
only in the SDK package, not in the BIN 
package) 

7 

include 

Contains updated header file for 
GL extensions, specifically the 
buffer class streaming extension 

Useful for video texturing applications 
involving bufferclass driver 

8 

tools 

Contains devmem2, and other 
packages 

Useful for running/debugging/benchmarking 
of Graphics SDK 



Retrieved from 
"http://processors.wiki.ti.com/index.php?title=RN_5_01_01_02&oldid=192762" 

. Content is available under Creative Commons Attribution-ShareAlike unless 
otherwise noted. 


 


