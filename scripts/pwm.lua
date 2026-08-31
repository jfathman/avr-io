#!/usr/bin/env lua5.5

-- pwm.lua

local Monitor = require("lib.monitor")

function main()
    local monitor = Monitor.new()

    monitor:open()

    local pin = 3

    local duty_cycles = { 0, 32, 64, 96, 128, 160, 192, 224, 255 }

    for i = 1, #duty_cycles do
        monitor:pwm_write(pin, duty_cycles[i])
        print("pin " .. pin .. " pwm = " .. duty_cycles[i])
        monitor:delay(50)
    end

    for i = #duty_cycles, 1, -1 do
        monitor:pwm_write(pin, duty_cycles[i])
        print("pin " .. pin .. " pwm = " .. duty_cycles[i])
        monitor:delay(50)
    end

    monitor:close()
end

main()
