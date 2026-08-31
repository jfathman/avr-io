# Makefile

VPATH_DIRS += .
CORENAME   := core
COREPATH   := $(PROJBASE)/core/lib/libcore.a
LDFLAGS    += -L$(PROJBASE)/core/lib
INCLUDES   += -I$(PROJBASE)/core/include
FIRMWARE   := $(BUILD_DIR)/$(PROGNAME).hex

COMMIT_ID    := $(shell git rev-parse --short HEAD)
COMMIT_DATE  := $(shell git log -1 --format=%cd --date=short $(COMMIT_ID))
COMMIT_COUNT := $(shell git rev-list --count --reverse --since="$(COMMIT_DATE) 00:00:00" $(COMMIT_ID))
VERSION      := $(shell git log -1 --pretty=format:"%cd" --date=format:"%y.%j").$(COMMIT_COUNT)
CXXFLAGS     += -DCOMMIT_ID=$(COMMIT_ID) -DVERSION=$(VERSION)

include $(PROJBASE)/mk/config.mk
include $(PROJBASE)/mk/toolchain.mk

.PHONY: all program init version commit-id crc size \
	    objdump clean rebuild picocom term run help

all: program

program: init $(BUILD_DIR)/$(PROGNAME).hex

-include $(OBJS:.o=.d)

init:
	@mkdir -p ./$(BUILD_DIR)

$(BUILD_DIR)/$(PROGNAME).elf: $(OBJS) $(COREPATH)
	@echo "linking:    $(PROGNAME).elf"
	@$(CC) $(LDFLAGS) -static -o $@ $^ -lm -lc -l$(CORENAME)

$(BUILD_DIR)/$(PROGNAME).hex: $(BUILD_DIR)/$(PROGNAME).elf
	@echo "generating: $(PROGNAME).hex"
	@$(OBJCOPY) -O ihex -R .eeprom $^ $@
	@$(MAKE) --no-print-directory version
	@$(MAKE) --no-print-directory commit-id
	@$(MAKE) --no-print-directory crc
	@echo "board:      $(BOARD)"
	@echo "mcu:        $(MCU)"

version:
	@echo "version:    $(VERSION)"

commit-id:
	@echo "commit-id:  $(COMMIT_ID)"

crc: BASENAME=$(BUILD_DIR)/$(PROGNAME)
crc:
	@avr-objcopy -O binary $(FLASH_SECTIONS) $(BASENAME).elf $(BASENAME).bin
	@echo "crc:        $$($(PROJBASE)/bin/crc-16.lua $(BASENAME).bin)"

size: program
	# option -B reports .data size too large
	# option -A reports .data size correctly
	@echo "$(BUILD_DIR)/$(PROGNAME).elf"
	@$(SIZE) -A $(BUILD_DIR)/$(PROGNAME).elf \
		| grep -e .data -e .text -e .bss | awk '{print $$1 "\t" $$2}'

objdump: program
	@echo objdump: $(OBJS)
	@$(OBJDUMP) -C -S $(OBJS)

clean:
	@rm -rf ./$(BUILD_DIR)

rebuild:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory -j$(shell nproc) all

picocom:
	@$(PROJBASE)/bin/picocom.sh

term:
	@$(PROJBASE)/bin/picocom.sh

run: program flash picocom

include $(PROJBASE)/mk/flash.mk

help:
	@echo "  all        build core and program"
	@echo "  program    build $(PROGNAME).hex"
	@echo "  flash      write $(PROGNAME).hex to mcu flash memory"
	@echo "  version    report program version"
	@echo "  commit-id  report current commit id for source repository"
	@echo "  crc        report crc-16 for flash download sections"
	@echo "  size       report $(PROGNAME).elf flash and sram memory size"
	@echo "  objdump    dump program source and disassembly"
	@echo "  clean      remove build transient files"
	@echo "  rebuild    run make clean and make -j$(shell nproc) all"
	@echo "  picocom    run picocom terminal program"
	@echo "  term       run picocom terminal program"
	@echo "  run        build all and run flash and picocom"
	@echo "  help       display make targets"
