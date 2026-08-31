# toolchain.mk

# ---------
# Toolchain
# ---------

CC      := avr-gcc
CXX     := avr-g++
AR      := avr-ar
OBJCOPY := avr-objcopy
OBJDUMP := avr-objdump
SIZE    := avr-size
AVRDUDE := avrdude

# ------------
# Build config
# ------------

OPTIMIZE  := -Os -ffunction-sections -fdata-sections

OPTIONS   := -fno-exceptions -Wall -Wextra -Werror -Wfatal-errors

DEFINES   += -DBOARD_$(BOARD) # support #ifdef
DEFINES   += -DBOARD=$(BOARD) # support string
DEFINES   += -DMCU=$(MCU)
DEFINES   += -DF_CPU=$(F_CPU)

INCLUDES  += -I$(PROJBASE)/core/variants/$(VARIANT)
INCLUDES  += -I./include
INCLUDES  += -I.

CFLAGS    += -mmcu=$(MCU) -std=gnu99 -g $(OPTIMIZE) $(DEFINES) $(INCLUDES)
CXXFLAGS  += -mmcu=$(MCU) -std=c++17 -g $(OPTIMIZE) $(DEFINES) $(INCLUDES) $(OPTIONS)
LDFLAGS   += -mmcu=$(MCU) -Wl,--gc-sections
LDFLAGS   += -Wl,-u,vfprintf -lprintf_flt -lm

# ---------------------------------------------------
# Define vpath pattern rules use to search for source
# ---------------------------------------------------

vpath %.c   $(VPATH_DIRS)
vpath %.cpp $(VPATH_DIRS)

# --------------------------------------------------------
# Pattern rules (will search for dependencies using vpath)
# --------------------------------------------------------

$(BUILD_DIR)/%.o : %.S
	@echo "compiling:  $<"
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(CC) $(CFLAGS) -MM $< -o- | sed 's!^$*.o!$@!' > $(@:.o=.d)

$(BUILD_DIR)/%.o : %.c
	@echo "compiling:  $<"
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(CC) $(CFLAGS) -MM $< -o- | sed 's!^$*.o!$@!' > $(@:.o=.d)

$(BUILD_DIR)/%.o : %.cpp
	@echo "compiling:  $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@$(CXX) $(CXXFLAGS) -MM $< -o- | sed 's!^$*.o!$@!' > $(@:.o=.d)
