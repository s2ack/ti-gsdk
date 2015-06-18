#
# File			make_demo.mak
# Title			Used to build a demo
# Author		PowerVR
#
# Copyright		Copyright (C) by Imagination Technologies Limited.
#


ifndef PLATFORM
$(error Error building application. You must define the PLATFORM variable to be the value of the target platform you want to build for. )
endif

#---------------------------------------------------------------------

include $(SDKDIR)/Builds/OGLES2/$(PLATFORM)/make_platform.mak

#---------------------------------------------------------------------

.PHONY: print_info build_tools wrap_content_files

ifndef SHELLOS
SHELLOS = Linux$(WS)
endif

SHELLOSPATH = $(SDKDIR)/Shell/OS/$(SHELLOS)

CONTENT := $(addprefix ../../Content/, $(subst .o,.cpp,$(OBJECTS)))
OBJECTS += $(OUTNAME).o PVRShell.o PVRShellAPI.o PVRShellOS.o
OBJECTS := $(addprefix $(PLAT_OBJPATH)/, $(OBJECTS))

INCLUDES += -I$(SDKDIR)/Tools/OGLES2 						\
			-I$(SDKDIR)/Shell 								\
			-I$(SDKDIR)/Shell/API/KEGL 						\
			-I$(SDKDIR)/Builds/OGLES2/$(PLATFORM)/Include 	\
			-I$(SDKDIR)/Builds/OGLES2/Include 				\
			-I$(SHELLOSPATH) 								\
			$(addprefix -I, $(PLAT_INC))

VPATH += ../..             : \
		 ../../Content     : \
		 $(SDKDIR)/Shell   : \
		 $(SDKDIR)/Shell/API/KEGL   : \
		 $(SHELLOSPATH)

LINK         += -L$(SDKDIR)/Tools/OGLES2/Build/LinuxGeneric/$(PLAT_OBJPATH) -logles2tools

ARCH := $(shell getconf LONG_BIT)
FILEWRAP_PATH =  $(SDKDIR)/Utilities/Filewrap/Linux_x86_$(ARCH)/Filewrap

#---------------------------------------------------------------------

all: $(PLAT_OBJPATH)/$(OUTNAME)

$(PLAT_OBJPATH)/$(OUTNAME) : print_info build_tools wrap_content_files $(OBJECTS) 
	@mkdir -p $(PLAT_OBJPATH)
	@echo "+l+ $@"
	$(PLAT_CPP) -o $(PLAT_OBJPATH)/$(OUTNAME) $(OBJECTS) $(LINK) $(PLAT_LINK)

$(PLAT_OBJPATH)/%.o: %.c
	@mkdir -p $(PLAT_OBJPATH)
	@echo "+c+ $(OUTNAME) $@"
	$(PLAT_CC) -c $(PLAT_CFLAGS) $(INCLUDES) $^ -o$@

$(PLAT_OBJPATH)/%.o: %.cpp
	@mkdir -p $(PLAT_OBJPATH)
	@echo "+c+ $(OUTNAME) $@"
	$(PLAT_CPP) -c $(PLAT_CFLAGS) $(INCLUDES) $^ -o$@

$(PLAT_OBJPATH)/%.o: ../../Content/%.cpp
	@mkdir -p $(PLAT_OBJPATH)
	@echo "+c+ $@"
	$(PLAT_CPP) -c $(PLAT_CFLAGS) $(INCLUDES) $^ -o$@

print_info:
	@echo ""
	@echo "******************************************************"
	@echo "*"
	@echo "* Name:         $(OUTNAME)"
	@echo "* PWD:          $(shell pwd)"
	@echo "* Binary path:  $(shell pwd)/$(PLAT_OBJPATH)"
	@echo "* Library path: $(shell cd $(LIBDIR) && pwd)"
	@echo "* WS:           $(WS)"
	@echo "*"
	@echo "******************************************************"

build_tools: $(SDKDIR)/Tools/OGLES2/Build/LinuxGeneric/$(PLAT_OBJPATH)/libogles2tools.a

$(SDKDIR)/Tools/OGLES2/Build/LinuxGeneric/$(PLAT_OBJPATH)/libogles2tools.a:
	@echo "********************"
	@echo "* Building tools.  *"
	@echo "********************"
	make -C $(SDKDIR)/Tools/OGLES2/Build/LinuxGeneric/

$(CONTENT): wrap_content_files

wrap_content_files:
	@test -f ../../content.mak &&                       \
		echo "+t+ Making content.mak" &&                \
		make -C ../.. -f content.mak                    \
			FILEWRAP=$(FILEWRAP_PATH)                   \
			|| true

clean: print_info
	@rm -vf $(OBJECTS) || true
	@test -f ../../content.mak && make -C ../../ -f content.mak clean || true
	@make -C $(SDKDIR)/Tools/OGLES2/Build/LinuxGeneric/ clean || true

