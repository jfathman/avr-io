# Makefile

LIBRARY    := libcore.a
PROJBASE   := ..
BUILD_DIR  := ./build
VPATH_DIRS := ./src
LIB_DIR    := ./lib

include $(PROJBASE)/mk/config.mk
include $(PROJBASE)/mk/toolchain.mk

OBJS := $(BUILD_DIR)/Analog.o
OBJS += $(BUILD_DIR)/coreError.o
OBJS += $(BUILD_DIR)/delay.o
OBJS += $(BUILD_DIR)/Digital.o
OBJS += $(BUILD_DIR)/Interval.o
OBJS += $(BUILD_DIR)/memory.o
OBJS += $(BUILD_DIR)/Mcu.o
OBJS += $(BUILD_DIR)/Serial.o
OBJS += $(BUILD_DIR)/Timer.o
OBJS += $(BUILD_DIR)/I2c.o
OBJS += $(BUILD_DIR)/Watchdog.o

include $(PROJBASE)/mk/library.mk
