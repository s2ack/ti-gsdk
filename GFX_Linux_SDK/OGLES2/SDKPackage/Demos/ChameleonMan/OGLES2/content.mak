#--------------------------------------------------------------------------
# Name         : content.mak
# Title        : Makefile to build content files
#
# Copyright    : Copyright (C)  Imagination Technologies Limited.
#              : No part of this software, either material or conceptual 
#              : may be copied or distributed, transmitted, transcribed,
#              : stored in a retrieval system or translated into any 
#              : human or computer language in any form by any means,
#              : electronic, mechanical, manual or other-wise, or 
#              : disclosed to third parties without the express written
#              : permission of VideoLogic Limited, Unit 8, HomePark
#              : Industrial Estate, King's Langley, Hertfordshire,
#              : WD4 8LZ, U.K.
#
# Description  : Makefile to wrap content files for examples in the PowerVR SDK
#
# Platform     :
#
#--------------------------------------------------------------------------

#############################################################################
## Variables
#############################################################################
FILEWRAP 	= ..\..\..\..\Utilities\Filewrap\Windows_x86_32\Filewrap.exe
CONTENTDIR = Content

#############################################################################
## Instructions
#############################################################################

RESOURCES = \
	$(CONTENTDIR)/skyline.cpp \
	$(CONTENTDIR)/Wall_diffuse_baked.cpp \
	$(CONTENTDIR)/Tang_space_BodyMap.cpp \
	$(CONTENTDIR)/Tang_space_LegsMap.cpp \
	$(CONTENTDIR)/Tang_space_BeltMap.cpp \
	$(CONTENTDIR)/FinalChameleonManLegs.cpp \
	$(CONTENTDIR)/FinalChameleonManHeadBody.cpp \
	$(CONTENTDIR)/lamp.cpp \
	$(CONTENTDIR)/ChameleonBelt.cpp \
	$(CONTENTDIR)/SkinnedVertShader.cpp \
	$(CONTENTDIR)/SkinnedFragShader.cpp \
	$(CONTENTDIR)/DefaultVertShader.cpp \
	$(CONTENTDIR)/DefaultFragShader.cpp \
	$(CONTENTDIR)/ChameleonScene.cpp

all: resources
	
help:
	@echo Valid targets are:
	@echo resources, clean
	@echo FILEWRAP can be used to override the default path to the Filewrap utility.

clean:
	-rm $(RESOURCES)

resources: $(CONTENTDIR) $(RESOURCES)

$(CONTENTDIR):
	-mkdir $@

$(CONTENTDIR)/skyline.cpp:
	$(FILEWRAP)  -o $@ skyline.pvr

$(CONTENTDIR)/Wall_diffuse_baked.cpp:
	$(FILEWRAP)  -o $@ Wall_diffuse_baked.pvr

$(CONTENTDIR)/Tang_space_BodyMap.cpp:
	$(FILEWRAP)  -o $@ Tang_space_BodyMap.pvr

$(CONTENTDIR)/Tang_space_LegsMap.cpp:
	$(FILEWRAP)  -o $@ Tang_space_LegsMap.pvr

$(CONTENTDIR)/Tang_space_BeltMap.cpp:
	$(FILEWRAP)  -o $@ Tang_space_BeltMap.pvr

$(CONTENTDIR)/FinalChameleonManLegs.cpp:
	$(FILEWRAP)  -o $@ FinalChameleonManLegs.pvr

$(CONTENTDIR)/FinalChameleonManHeadBody.cpp:
	$(FILEWRAP)  -o $@ FinalChameleonManHeadBody.pvr

$(CONTENTDIR)/lamp.cpp:
	$(FILEWRAP)  -o $@ lamp.pvr

$(CONTENTDIR)/ChameleonBelt.cpp:
	$(FILEWRAP)  -o $@ ChameleonBelt.pvr

$(CONTENTDIR)/SkinnedVertShader.cpp:
	$(FILEWRAP)  -s  -o $@ SkinnedVertShader.vsh
	-$(FILEWRAP)  -oa $@ SkinnedVertShader.vsc

$(CONTENTDIR)/SkinnedFragShader.cpp:
	$(FILEWRAP)  -s  -o $@ SkinnedFragShader.fsh
	-$(FILEWRAP)  -oa $@ SkinnedFragShader.fsc

$(CONTENTDIR)/DefaultVertShader.cpp:
	$(FILEWRAP)  -s  -o $@ DefaultVertShader.vsh
	-$(FILEWRAP)  -oa $@ DefaultVertShader.vsc

$(CONTENTDIR)/DefaultFragShader.cpp:
	$(FILEWRAP)  -s  -o $@ DefaultFragShader.fsh
	-$(FILEWRAP)  -oa $@ DefaultFragShader.fsc

$(CONTENTDIR)/ChameleonScene.cpp:
	$(FILEWRAP)  -o $@ ChameleonScene.pod

############################################################################
# End of file (content.mak)
############################################################################
