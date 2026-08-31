#!/usr/bin/env lua5.5

-- run.lua

local Monitor = require("lib.monitor")

function main()
    local args = table.concat(arg, " ")

    if #args == 0 then
        print("error: args missing")
        os.exit(1)
    end

    local monitor = Monitor.new()

    monitor:open()

    monitor:send(args)

    monitor:close()
end

main()
