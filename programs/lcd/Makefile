# Makefile

PROGNAME  := lcd
BUILD_DIR := build
PROJBASE  := ../..

INCLUDES  += -I$(PROJBASE)/drivers/hd44780/include
LDFLAGS   += -L$(PROJBASE)/drivers/hd44780/lib
LDLIBS    += -lhd44780

OBJS := $(BUILD_DIR)/$(PROGNAME).o

include $(PROJBASE)/mk/program.mk
