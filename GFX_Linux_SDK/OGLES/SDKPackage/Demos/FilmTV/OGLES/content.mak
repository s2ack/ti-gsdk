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
	$(CONTENTDIR)/Table.cpp \
	$(CONTENTDIR)/Floor.cpp \
	$(CONTENTDIR)/Wall.cpp \
	$(CONTENTDIR)/TV.cpp \
	$(CONTENTDIR)/TVCase.cpp \
	$(CONTENTDIR)/TVSpeaker.cpp \
	$(CONTENTDIR)/Alum.cpp \
	$(CONTENTDIR)/Skirting.cpp \
	$(CONTENTDIR)/Camera.cpp \
	$(CONTENTDIR)/FilmTVScene.cpp

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

$(CONTENTDIR)/Table.cpp:
	$(FILEWRAP)  -o $@ Table.pvr

$(CONTENTDIR)/Floor.cpp:
	$(FILEWRAP)  -o $@ Floor.pvr

$(CONTENTDIR)/Wall.cpp:
	$(FILEWRAP)  -o $@ Wall.pvr

$(CONTENTDIR)/TV.cpp:
	$(FILEWRAP)  -o $@ TV.pvr

$(CONTENTDIR)/TVCase.cpp:
	$(FILEWRAP)  -o $@ TVCase.pvr

$(CONTENTDIR)/TVSpeaker.cpp:
	$(FILEWRAP)  -o $@ TVSpeaker.pvr

$(CONTENTDIR)/Alum.cpp:
	$(FILEWRAP)  -o $@ Alum.pvr

$(CONTENTDIR)/Skirting.cpp:
	$(FILEWRAP)  -o $@ Skirting.pvr

$(CONTENTDIR)/Camera.cpp:
	$(FILEWRAP)  -o $@ Camera.pvr

$(CONTENTDIR)/FilmTVScene.cpp:
	$(FILEWRAP)  -o $@ FilmTVScene.pod

############################################################################
# End of file (content.mak)
############################################################################
