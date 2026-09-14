#!/usr/bin/env lua5.5

-- lcd.lua

local Monitor = require("lib.monitor")

function main()
    local monitor = Monitor.new()

    monitor:open()

    monitor:lcd_init()

    monitor:lcd_cursor(1, 2)
    monitor:lcd_write("Begin LCD Test")
    monitor:delay(2000)

    monitor:lcd_clear(1)
    monitor:lcd_write("1234567890123456")
    monitor:delay(2000)

    monitor:lcd_clear(2)
    monitor:lcd_write("abcdefghijklmnop")
    monitor:delay(2000)

    monitor:lcd_clear()
    monitor:lcd_write("clear")
    monitor:delay(2000)

    monitor:lcd_clear()
    monitor:lcd_cursor(2)
    monitor:lcd_write("data during test")
    monitor:delay(2000)

    monitor:lcd_clear(1)
    monitor:lcd_write("display off")
    monitor:delay(1500)
    monitor:lcd_display_off()
    monitor:delay(2000)

    monitor:lcd_clear(1)
    monitor:lcd_write("display on")
    monitor:lcd_display_on()
    monitor:delay(2000)

    monitor:lcd_clear(1)
    monitor:lcd_write("backlight off")
    monitor:delay(1500)
    monitor:lcd_backlight_off()
    monitor:delay(2000)

    monitor:lcd_clear(1)
    monitor:lcd_write("backlight on")
    monitor:lcd_backlight_on()
    monitor:delay(2000)

    monitor:lcd_clear()

    monitor:lcd_cursor(1)
    local lower = "abcdefghijklmnop"
    for i = 1, 16 do
        local char = lower:sub(i, i)
        monitor:lcd_write(char)
        monitor:delay(100)
    end

    monitor:lcd_cursor(2)
    local upper = "ABCDEFGHIJKLMNOP"
    for i = 1, 16 do
        local char = upper:sub(i, i)
        monitor:lcd_write(char)
        monitor:delay(100)
    end
    monitor:delay(2000)

    monitor:lcd_clear(1)
    monitor:lcd_write("clear 1")
    monitor:delay(2000)
    monitor:lcd_clear(1)

    monitor:lcd_clear(2)
    monitor:lcd_write("clear 2")
    monitor:delay(2000)

    monitor:lcd_clear()
    monitor:lcd_write("End of LCD Test!")
    monitor:delay(1000)
    for i = 1, 8 do
        monitor:lcd_cursor(2, i)
        monitor:lcd_write("*")
        monitor:lcd_cursor(2, 17 - i)
        monitor:lcd_write("*")
        monitor:delay(200)
    end

    monitor:close()
end

main()
