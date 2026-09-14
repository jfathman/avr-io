# library.mk

.PHONY: all library init size objdump clean help

all: library

-include $(OBJS:.o=.d)

library: init $(LIB_DIR)/$(LIBRARY)

init:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(LIB_DIR)

$(LIB_DIR)/$(LIBRARY): $(OBJS)
	@echo "archiving:  $@"
	@rm -f $@
	@$(AR) -crsD $@ $^
	@echo "board:      $(BOARD)"
	@echo "mcu:        $(MCU)"

size: library
	@echo size:
	@$(SIZE) -B $(LIB_DIR)/$(LIBRARY)

objdump: library
	@echo objdump: $(OBJS)
	@$(OBJDUMP) -C -S $(OBJS)

clean:
	@rm -rf ./$(BUILD_DIR)
	@rm -rf ./$(LIB_DIR)

help:
	@echo "  all      build all"
	@echo "  library  build $(LIBRARY)"
	@echo "  size     report $(LIBRARY) flash and sram memory size"
	@echo "  objdump  dump library source and disassembly"
	@echo "  clean    remove build transient files"
	@echo "  help     display make targets"
