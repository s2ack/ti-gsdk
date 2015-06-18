.PHONY: all install uninstall clean

-include rules.make

INCS     ?= -I../include \
            -I$(GLES_DIR) \
            -I$(GLES_DIR)/LinuxARMV7/Include \
            -I$(GSDK_KM_DIR)/include4 \
            -I$(CMEM_DIR)/include \
            -I../module
LIB_PATH ?= -L$(GLES_DIR)/LinuxARMV7/lib \
            -Wl,--rpath=$(GLES_DIR)/LinuxARMV7/lib \
            -L$(GSDK_ROOT)/gfx_rel_es3.x \
            -Wl,--rpath=$(GSDK_ROOT)/gfx_rel_es3.x

INST_DEST    ?= $(TGTFS_PATH)
CMEM_LIB     ?= $(CMEM_DIR)/lib/cmem.a470MV

CFLAGS   := -W -Wall -O2 -DLINUX $(INCS)
LIBS     := -lEGL
LIBS    += $(CMEM_LIB)
LDFLAGS  := $(LIB_PATH)

LIBS    += -lGLES_CM

TARGET = sampleTwoTriangles

SOURCES = main.c common.c
HEADERS = common.h
OBJFILES = $(SOURCES:%.c=%.o)

all:	$(TARGET)

$(TARGET):	$(OBJFILES)
	$(CC) $^ -o $@ $(LDFLAGS) $(LIBS)

$(OBJFILES):	%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

install:	$(TARGET)
	install -m 0755 $^ $(INST_DEST)

uninstall:
	cd $(INST_DEST) && rm -f $(TARGET)

.PHONY: clean
clean:
	-rm -f $(OBJFILES) $(TARGET)
