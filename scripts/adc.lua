#!/usr/bin/env lua5.5

-- adc.lua

local Monitor = require("lib.monitor")

local monitor = Monitor.new()

function main()
    monitor:open()

    local board = monitor:board()

    local pin = 0

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

    monitor:close()
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

main()
