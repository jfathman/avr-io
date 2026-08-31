#!/usr/bin/env lua5.5

-- pwm-all.lua

local Monitor = require("lib.monitor")

local monitor = Monitor.new()

function main()
    monitor:open()

    local board = monitor:board()

    local pins = get_pins(board)

    for _, pin in ipairs(pins) do
        monitor:pause("press enter when ready to test pin " .. pin)
        print("<<< PWM pin " .. pin .. " >>>")
        pwm_ramp(pin)
        pwm_set(pin, 128)
        print("<<< PWM pin " .. pin .. " >>>")
    end

    monitor:close()
end

function pwm_ramp(pin)
    local duty_cycles = { 0, 32, 64, 96, 128, 160, 192, 224, 255 }

    for i = 1, #duty_cycles do
        pwm_set(pin, duty_cycles[i])
        monitor:delay(75)
    end

    for i = #duty_cycles, 1, -1 do
        pwm_set(pin, duty_cycles[i])
        monitor:delay(75)
    end
end

function pwm_set(pin, duty_cycle)
    monitor:pwm_write(pin, duty_cycle)
    print("pin " .. pin .. " pwm = " .. duty_cycle)
end

function get_pins(board)
    if board == "arduino_uno_r3" then
        return { 3, 5, 6, 9, 10, 11 }
    end

    if board == "arduino_mega_2560" then
        return { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45, 46 }
    end

    if board == "arduino_nano_every" then
        return { 3, 5, 6, 9, 10 }
    end

    error("unknown board: " .. board)
end

main()
