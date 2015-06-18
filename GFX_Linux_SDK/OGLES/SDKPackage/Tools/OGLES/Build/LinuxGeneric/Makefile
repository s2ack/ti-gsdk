#
# File			Makefile
# Title			Makes the tools
#
# Copyright		Copyright (C) by Imagination Technologies Limited.
#

.PHONY: clean

SDKDIR  = ../../../..
include $(SDKDIR)/Builds/OGLES/$(PLATFORM)/make_platform.mak

OUTNAME = liboglestools.a
INCLUDES += -I$(SDKDIR)/Tools -I$(SDKDIR)/Tools/OGLES $(addprefix -I, $(PLAT_INC))
VPATH = $(SDKDIR)/Tools : $(SDKDIR)/Tools/OGLES

OBJECTS = PVRTFixedPoint.o \
		PVRTError.o	\
		PVRTMatrixX.o \
		PVRTMatrixF.o \
		PVRTQuaternionF.o	\
		PVRTQuaternionX.o	\
		PVRTMisc.o \
		PVRTBackground.o \
		PVRTTrans.o \
		PVRTModelPOD.o \
		PVRTVertex.o \
		PVRTDecompress.o \
		PVRTTriStrip.o \
		PVRTglesExt.o \
		PVRTPrint3D.o \
		PVRTPrint3DAPI.o \
		PVRTTexture.o	\
		PVRTTextureAPI.o	\
		PVRTResourceFile.o	\
		PVRTShadowVol.o \
		PVRTString.o	\
		PVRTStringHash.o	\
		PVRTUnicode.o	\
		PVRTVector.o

OBJECTS := $(addprefix $(PLAT_OBJPATH)/, $(OBJECTS))

$(PLAT_OBJPATH)/$(OUTNAME) : $(OBJECTS) 
	@mkdir -p $(PLAT_OBJPATH)
	@echo "+a+ $@"
	@$(PLAT_AR) -r $(PLAT_OBJPATH)/$(OUTNAME) $(OBJECTS)

$(PLAT_OBJPATH)/%.o: %.cpp
	@mkdir -p $(PLAT_OBJPATH)
	@echo "+c+ $(OUTNAME) $@"
	@$(PLAT_CC) -c $(INCLUDES) $(PLAT_CFLAGS) $^ -o$@

clean:
	@for i in $(OBJECTS); do test -f $$i && rm -vf $$i || true; done
