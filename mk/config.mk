# config.mk

BOARD ?= arduino_uno_r3
PORT  ?= /dev/ttyACM0
BAUD  ?= 115200

ifeq ($(BOARD), arduino_uno_r3)
	MCU             := atmega328p
	F_CPU           := 16000000L
	VARIANT         := uno
	ERASE_OPTION    := -D
	FLASH_SECTIONS  := -j .text -j .data
	UPLOAD_PROTOCOL := arduino
	UPLOAD_SPEED    := $(BAUD)
	UPLOAD_PORT     := $(PORT)
else ifeq ($(BOARD), arduino_mega_2560)
	MCU             := atmega2560
	F_CPU           := 16000000L
	VARIANT         := mega
	ERASE_OPTION    := -D
	FLASH_SECTIONS  := -j .text -j .data
	UPLOAD_PROTOCOL := wiring
	UPLOAD_SPEED    := $(BAUD)
	UPLOAD_PORT     := $(PORT)
else ifeq ($(BOARD), arduino_nano_every)
	MCU             := atmega4809
	F_CPU           := 16000000L
	VARIANT         := every
	ERASE_OPTION    := # none
	FLASH_SECTIONS  := -j .text -j .data -j .rodata
	UPLOAD_PROTOCOL := jtag2updi
	UPLOAD_SPEED    := $(BAUD)
	UPLOAD_PORT     := $(PORT)
else
	$(error Unknown BOARD: $(BOARD))
endif
