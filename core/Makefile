# Makefile

LIBCORE    := libcore.a
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
OBJS += $(BUILD_DIR)/Watchdog.o

.PHONY: all core init size objdump clean help

all: core

-include $(OBJS:.o=.d)

core: init $(LIB_DIR)/$(LIBCORE)

init:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(LIB_DIR)

$(LIB_DIR)/$(LIBCORE): $(OBJS)
	@echo "archiving:  $@"
	@rm -f $@
	@$(AR) -crsD $@ $^
	@echo "board:      $(BOARD)"
	@echo "mcu:        $(MCU)"

size: core
	@echo size:
	@$(SIZE) -B $(LIB_DIR)/$(LIBCORE)

objdump: core
	@echo objdump: $(OBJS)
	@$(OBJDUMP) -C -S $(OBJS)

clean:
	@rm -rf ./$(BUILD_DIR)
	@rm -rf ./$(LIB_DIR)

help:
	@echo "  all      build core and program"
	@echo "  core     build $(LIBCORE)"
	@echo "  size     report $(PROGRAM).elf flash and sram memory size"
	@echo "  objdump  dump program source and disassembly"
	@echo "  clean    remove build transient files"
	@echo "  help     display make targets"
