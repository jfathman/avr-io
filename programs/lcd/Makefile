# Makefile

PROGNAME  := lcd
BUILD_DIR := build
PROJBASE  := ../..

INCLUDES  += -I$(PROJBASE)/drivers/lcd-hd44780/include
LDFLAGS   += -L$(PROJBASE)/drivers/lcd-hd44780/lib
LDLIBS    += -llcd-hd44780

OBJS := $(BUILD_DIR)/$(PROGNAME).o

include $(PROJBASE)/mk/program.mk
