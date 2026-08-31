#!/usr/bin/env lua5.5

function main()
    if #arg ~= 1 then
        print("Usage: crc-16.lua <filename>")
        os.exit(1)
    end

    local filename = arg[1]

    local file = assert(io.open(filename, "rb"))

    local crc = 0xFFFF

    while true do
        local char = file:read(1)
        if not char then
            break
        end
        
        local byte = string.byte(char)

        if byte == -1 then
            break
        end

        crc = crc ~ byte

        for i = 1, 8 do
            if crc & 0x0001 ~= 0 then
                crc = (crc >> 1) ~ 0xA001
            else
                crc = crc >> 1
            end
        end

        crc = crc & 0xFFFF
    end

    file:close()

    print(string.format("%04x", crc))
end

main()
