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
	$(CONTENTDIR)/Marble.cpp \
	$(CONTENTDIR)/Floor.cpp \
	$(CONTENTDIR)/LanternCubemap.cpp \
	$(CONTENTDIR)/MagicLanternShaders.cpp \
	$(CONTENTDIR)/MagicLantern.cpp

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

$(CONTENTDIR)/Marble.cpp:
	$(FILEWRAP)  -o $@ Marble.pvr

$(CONTENTDIR)/Floor.cpp:
	$(FILEWRAP)  -o $@ Floor.pvr

$(CONTENTDIR)/LanternCubemap.cpp:
	$(FILEWRAP)  -o $@ LanternCubemap.pvr

$(CONTENTDIR)/MagicLanternShaders.cpp:
	$(FILEWRAP)  -s  -o $@ MagicLanternShaders.pfx

$(CONTENTDIR)/MagicLantern.cpp:
	$(FILEWRAP)  -o $@ MagicLantern.pod

############################################################################
# End of file (content.mak)
############################################################################
