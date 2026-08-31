#!/usr/bin/env lua5.5

-- adc-all.lua

local Monitor = require("lib.monitor")

local monitor = Monitor.new()

function main()
    monitor:open()

    local board = monitor:board()

    local pins = get_pins(board)

    for _, pin in ipairs(pins) do
        monitor:pause("press enter when ready to test pin " .. pin)
        print("<<< ADC pin " .. pin .. " >>>\n")
        adc_test(board, pin)
        print("<<< ADC pin " .. pin .. " >>>")
    end

    monitor:close()
end

function adc_test(board, pin)
    read_loop(pin, "vcc")
    read_loop(pin, "int")

    if board == "arduino_mega_2560" then
        read_loop(pin, "int1v1")
        read_loop(pin, "int2v56")
    end

    if board == "arduino_nano_every" then
        read_loop(pin, "int0v55")
        read_loop(pin, "int1v1")
        read_loop(pin, "int1v5")
        read_loop(pin, "int2v5")
        read_loop(pin, "int4v34")
    end
end

function read_loop(pin, vref)
    monitor:adc_init(vref)
    for i = 1, 5 do
        local reading = monitor:adc_read(pin)
        print("vref " .. vref .. " pin A" .. pin .. " = " .. string.format("%.2f", reading))
        monitor:delay(100)
    end
    print()
end

function get_pins(board)
    if board == "arduino_uno_r3" then
        return { 0, 1, 2, 3, 4, 5 }
    end

    if board == "arduino_mega_2560" then
        return { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }
    end

    if board == "arduino_nano_every" then
        return { 0, 1, 2, 3, 4, 5, 6, 7 }
    end

    error("unknown board: " .. board)
end

main()
