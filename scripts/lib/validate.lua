-- validate.lua

local validate = {}

local monitor

function validate.init(mon)
    monitor = mon
end

function validate.adc_read(pin, vref, max)
    monitor:adc_init(vref)
    local reading = monitor:adc_read(pin)
    print(string.format("adc read vref %s pin %u = %.2f", vref, pin, reading))
    assert(reading >= 0 and reading <= max, "adc out of range")
end

function validate.dig_read(pin, mode)
    monitor:dig_mode(pin, mode)
    local a = monitor:dig_rda(pin)
    print(string.format("dig read mode %s pin %u rda = %u", mode, pin, a))
    if mode == "out" then
        local b = monitor:dig_rdc(pin)
        print(string.format("dig read mode %s pin %u rdc = %u", mode, pin, b))
        assert(a == b, "dig_read rda != rdc")
    end
    print()
end

function validate.dig_write(pin, op)
    local expected

    monitor:dig_mode(pin, "out")

    if     op == "on"    then expected = 1 ; monitor:dig_on(pin)
    elseif op == "off"   then expected = 0 ; monitor:dig_off(pin)
    elseif op == "set"   then expected = 1 ; monitor:dig_set(pin)
    elseif op == "clear" then expected = 0 ; monitor:dig_clear(pin)
    else
        error("invalid dig_write op = " .. op)
    end
    print("dig write mode out pin " .. pin .. " " .. op)

    local a = monitor:dig_rda(pin)
    print(string.format("dig write mode out pin %u rda = %u", pin, a))
    assert(a == expected, "dig rda != " .. expected)
    local b = monitor:dig_rdc(pin)
    print(string.format("dig write mode out pin %u rdc = %u", pin, b))
    assert(b == expected, "dig rdc != " .. expected)
    print()
end

function validate.pwm_waveform(pin, msec, duty_cycles)
    for _, duty_cycle in ipairs(duty_cycles) do
        monitor:pwm_write(pin, duty_cycle)
        print("pwm pin " .. pin .. " duty cycle = " .. duty_cycle)
        local value = monitor:pwm_read(pin)
        assert(value == duty_cycle, "pwm read != duty cycle")
        monitor:delay(msec)
    end
end

return validate
