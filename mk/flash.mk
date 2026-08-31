# flash.mk

# -------------------------------------------
# Arduino is a USB 2.0 device, but it is best
# to configure the Linux VM to use USB 3.0 in
# VirtualBox for improved serial reliability.
# -------------------------------------------

.PHONY: flash

flash: $(FIRMWARE)
	@if [ ! -e $(UPLOAD_PORT) ]; then \
		echo "serial device not found: $(UPLOAD_PORT)"; \
		echo "--------------------------------------"; \
		echo "hint: capture USB device in VirtualBox"; \
		echo "--------------------------------------"; \
		exit 1; \
	fi
	@# Use '1200 baud touch' to trigger the SAMD11
	@# to reset the atmega4809 mcu prior to upload.
	@if [ "$(BOARD)" = "arduino_nano_every" ]; then \
		stty -F $(UPLOAD_PORT) 1200 hupcl; \
		sleep 0.5; \
	else \
		stty -F $(UPLOAD_PORT) hupcl; \
	fi
	@$(AVRDUDE) $(ERASE_OPTION) \
		-p $(MCU) \
		-c $(UPLOAD_PROTOCOL) \
		-b $(UPLOAD_SPEED) \
		-P $(UPLOAD_PORT) \
		-U flash:w:$(FIRMWARE):i
	stty -F $(UPLOAD_PORT) -hupcl
