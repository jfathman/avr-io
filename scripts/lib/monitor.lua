-- monitor.lua

local Serial = require("lib.serial")

local Monitor = {}

Monitor.__index = Monitor

function Monitor.new()
    local self = setmetatable({}, Monitor)
    self._verbose = os.getenv("VERBOSE") ~= nil
    self.serial = nil
    return self
end

function Monitor:open()
    self.serial = Serial.new()
    self.serial:init()
    self.serial:open()
end

function Monitor:close()
    self.serial:close()
end

function Monitor:adc_init(option)
    self:_send(string.format("adc init %s", option))
end

function Monitor:adc_read(pin)
    local value = nil
    local command = string.format("adc %s read", pin)
    self.serial:send(command, function(line)
        if (self._verbose) then
            print(line)
        end
        if line ~= command and not tonumber(line) then
            print(line)
            error("invalid response")
        end
        value = tonumber(line)
    end)
    return value
end

function Monitor:dig_mode(pin, mode)
    self:_send(string.format("dig %s %s", pin, mode))
end

function Monitor:dig_set(pin)
    self:_send(string.format("dig %s set", pin))
end

function Monitor:dig_clear(pin)
    self:_send(string.format("dig %s clear", pin))
end

function Monitor:dig_on(pin)
    self:_send(string.format("dig %s on", pin))
end

function Monitor:dig_off(pin)
    self:_send(string.format("dig %s off", pin))
end

function Monitor:dig_toggle(pin)
    local command = string.format("dig %s toggle", pin)
    self.serial:send(command, function(line)
        if (self._verbose) then
            print(line)
        end
        if line ~= command and line ~= "on" and line ~= "off" then
            print(line)
            error("invalid response")
        end
    end)
end

function Monitor:dig_rda(pin)
    local value = nil
    local command = string.format("dig %s rda", pin)
    self.serial:send(command, function(line)
        if (self._verbose) then
            print(line)
        end
        if line ~= command and line ~= "1" and line ~= "0" then
            print(line)
            error("invalid response")
        end
        if line == "0" or line == "1" then
            value = tonumber(line)
        end
    end)
    return value
end

function Monitor:dig_rdc(pin)
    local value = nil
    local command = string.format("dig %s rdc", pin)
    self.serial:send(command, function(line)
        if (self._verbose) then
            print(line)
        end
        if line ~= command and line ~= "1" and line ~= "0" then
            print(line)
            error("invalid response")
        end
        if line == "0" or line == "1" then
            value = tonumber(line)
        end
    end)
    return value
end

function Monitor:pwm_write(pin, duty_cycle)
    self:_send(string.format("pwm %s %s", pin, duty_cycle))
end

function Monitor:pwm_read(pin)
    local value = nil
    local command = string.format("pwm %s read", pin)
    self.serial:send(command, function(line)
        if (self._verbose) then
            print(line)
        end
        if line ~= command and not tonumber(line) then
            print(line)
            error("invalid response")
        end
        value = tonumber(line)
    end)
    return value
end

function Monitor:send(command, callback)
    self.serial:send(command, function(line)
        if type(callback) == "function" then
            callback(line)
        else
            print(line)
        end
        if command == "reset" and line == "ok" then
            os.exit(0)
        end
    end)
end

function Monitor:board()
    local board = nil
    local command = string.format("system", pin)
    self.serial:send(command, function(line)
        if (self._verbose) then
            print(line)
        end
        if line == command then
            return
        end
        local first, second = line:match("(%S+)%s+(%S+)")
        if first == "board" then
            board = second
        end
    end)
    return board
end

function Monitor:delay(msec)
    self:_send(string.format("delay %s", msec))
end

function Monitor:pause(prompt)
    print("\n" .. prompt)
    io.read()
end

function Monitor:verbose(option)
    if type(option) ~= "boolean" then
        error("boolean required")
    end
    self._verbose = option;
end

function Monitor:_send(command)
    self.serial:send(command, function(line)
        if (self._verbose) then
            print(line)
        end
        if line ~= command and line ~= "ok" then
            print(line)
            error("invalid response")
        end
    end)
end

return Monitor
