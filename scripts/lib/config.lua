-- config.lua

local config = {}

local regions

function config.last_block(board, region)
    if regions == nil then
        if board == "arduino_uno_r3" then
            regions = {
                sram   = 0x8ff,   -- end address
                flash  = 0x7fff,  -- end address
                eeprom = 0x3ff    -- end address
            }
        elseif board == "arduino_mega_2560" then
            regions = {
                sram   = 0x21ff,  -- end address
                flash  = 0x3ffff, -- end address
                eeprom = 0xfff    -- end address
            }
        elseif board == "arduino_nano_every" then
            regions = {
                sram   = 0x3fff,  -- end address
                flash  = 0xbfff,  -- end address
                eeprom = 0xff     -- end address
            }
        else
            error("unknown board: " .. board);
        end
    end

    return string.format("%x", regions[region] + 1 - 64)
end

return config
