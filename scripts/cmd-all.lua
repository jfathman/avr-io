#!/usr/bin/env lua5.5

-- cmd-all.lua

local config = require("lib.config")
local expect = require("lib.expect")
local Monitor = require("lib.monitor")
local validate = require("lib.validate")

local monitor
local board

function main()
    monitor = Monitor.new()
    expect.init(monitor)
    validate.init(monitor)

    monitor:open()
    board = monitor:board()
    run_commands()
    monitor:close()
end

function run_commands()
    monitor:send("help")

    print()

    local pin = 0

    validate.adc_read(pin, "vcc", 5.1)
    validate.adc_read(pin, "int", 1.1)

    if board == "arduino_mega_2560" then
        validate.adc_read(pin, "int1v1 ", 1.1)
        validate.adc_read(pin, "int2v56", 2.56)
    end

    if board == "arduino_nano_every" then
        validate.adc_read(pin, "int0v55", 0.55)
        validate.adc_read(pin, "int1v1 ", 1.1)
        validate.adc_read(pin, "int1v5 ", 1.5)
        validate.adc_read(pin, "int2v5 ", 2.5)
        validate.adc_read(pin, "int4v34", 4.34)
    end

    print()

    -- see doc/adc-caution.txt
    -- not tested due to caution above:
    -- adc init ext 5.0
    -- adc 0 read

    pin = 12

    validate.dig_read(pin, "in")
    validate.dig_read(pin, "inpull")

    pin = 13

    validate.dig_write(pin, "on")    ; monitor:delay(100)
    validate.dig_write(pin, "off")   ; monitor:delay(100)
    validate.dig_write(pin, "set")   ; monitor:delay(100)
    validate.dig_write(pin, "clear") ; monitor:delay(100)

    pin = 3

    msec = 100

    local increase = { 0, 32, 64, 96, 128, 160, 192, 224, 255 }
    local decrease = { 255, 224, 192, 160, 128, 96, 64, 32, 0 }

    validate.pwm_waveform(pin, msec, increase)
    validate.pwm_waveform(pin, msec, decrease)

    print()

    expect.response_line("nop",       "ok")
    expect.response_line("debug on",  "debug is on")
    expect.response_line("debug",     "debug is on")
    expect.response_line("debug off", "debug is off")
    expect.response_line("debug",     "debug is off")

    if board == "arduino_nano_every" then
        expect.line_count_eq(19, "map")
    else
        expect.line_count_eq(15, "map")
    end

    expect.line_count_eq(8, "mem")
    expect.line_count_eq(7, "heap")

    if board == "arduino_nano_every" then
        expect.line_count_eq(7, "flash")
    else
        expect.line_count_eq(6, "flash")
    end

    expect.line_count_eq(2, "crc")
    expect.line_count_eq(4, "eeprom")
    expect.line_count_eq(2, "id")

    local last_block_sram   = config.last_block(board, "sram")
    local last_block_flash  = config.last_block(board, "flash")
    local last_block_eeprom = config.last_block(board, "eeprom")

    expect.line_count_eq(9, "dump sram")
    expect.line_count_eq(9, "cont")
    expect.line_count_eq(5, "dump sram " .. last_block_sram)
    expect.response_line("cont", "sram end")

    expect.line_count_eq(9, "dump flash")
    expect.line_count_eq(9, "cont")
    expect.line_count_eq(5, "dump flash " .. last_block_flash)
    expect.response_line("cont", "flash end")

    expect.line_count_eq(9, "dump eeprom")
    expect.line_count_eq(9, "cont")
    expect.line_count_eq(5, "dump eeprom " .. last_block_eeprom)
    expect.response_line("cont", "eeprom end")

    expect.line_count_ge(1, "dump heap")
    expect.line_count_eq(3, "uptime")
    expect.line_count_eq(5, "system")
    expect.line_count_eq(6, "ver")
    expect.line_count_eq(4, "stack")
end

main()
