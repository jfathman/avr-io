#!/usr/bin/env lua5.5

-- dig-all.lua

local Monitor = require("lib.monitor")
    
local monitor = Monitor.new()

function main()
    monitor:open()

    local board = monitor:board()

    local pin_begin, pin_end = get_pins(board)

    for pin = pin_begin, pin_end do
        monitor:pause("press enter when ready to test pin " .. pin)
        print("<<< DIG pin " .. pin .. " >>>\n")
        dig_test(pin)
        print("<<< DIG pin " .. pin .. " >>>")
    end

    monitor:close()
end

function dig_test(pin)
    monitor:dig_mode(pin, "out")
    monitor:dig_off(pin)
    for i = 1, 10 do
        monitor:dig_toggle(pin)
        local rda = monitor:dig_rda(pin)
        local rdc = monitor:dig_rdc(pin)
        print("pin " .. pin .. " = " .. rda)
        assert(rda == rdc, "dig_read rda != rdc")
        monitor:delay(150)
    end
end

function get_pins(board)
    if board == "arduino_uno_r3" then
        return 2, 19
    end

    if board == "arduino_mega_2560" then
        return 2, 69
    end

    if board == "arduino_nano_every" then
        return 2, 21
    end

    error("unknown board: " .. board)
end

main()
