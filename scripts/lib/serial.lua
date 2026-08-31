-- serial.lua

local Serial = {}

Serial.__index = Serial

function Serial.new()
    local self = setmetatable({}, Serial)
    self.rxHandler = nil
    return self
end

function Serial:init(device, baudrate)
    self.device = device or os.getenv("PORT") or "/dev/ttyACM0"
end

function Serial:open()
    self.port, err = io.open(self.device, "r+")
    if not self.port then
        error(err)
    end
    return self.port
end

function Serial:close()
    self.port:close()
end

function Serial:send(command, callback)
    self:_send(self.port, command)
    self:_receive(self.port, callback)
end

function Serial:receiveHandler(callback)
    self.rxHandler = callback
end

function Serial:_send(port, command)
    port:write(command, "\r")
    port:flush()
end

function Serial:_receive(port, callback)
    local iobf = {}
    local line = nil

    while true do
        local char = port:read(1)
        if not char then
            break
        end
        if char == "\r" then
            if type(callback) == "function" then
                callback(line)
            elseif type(self.rxHandler) == "function" then
                self.rxHandler(line)
            else
                print(line)
            end
            iobf = {}
        else
            if char ~= "\n" then
                table.insert(iobf, char)
            end
            line = table.concat(iobf)
            if string.match(line, "^W:> ") then
                break
            end
        end
    end
end

return Serial
