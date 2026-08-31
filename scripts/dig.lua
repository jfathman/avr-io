#!/usr/bin/env lua5.5

-- dig.lua

local Monitor = require("lib.monitor")

function main()
    local monitor = Monitor.new()

    monitor:open()

    local pin = 13

    monitor:dig_mode(pin, "out")

    monitor:dig_off(pin)

    for i = 1, 6 do
        monitor:dig_toggle(pin)
        print("pin " .. pin .. " = " .. monitor:dig_rda(pin))
        monitor:delay(500)
    end

    monitor:close()
end

main()
