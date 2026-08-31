-- expect.lua

local expect = {}

local monitor

function expect.init(mon)
    monitor = mon
end

function expect.response_line(command, expected)
    monitor:send(command, function(line)
        print(line)
        assert(line == command or line == expected, "invalid response")
    end)
    print()
end

function expect.line_count_eq(expected, command)
    local count = 0
    monitor:send(command, function(line)
        print(line)
        count = count + 1
    end)
    assert(count == expected, "line count != " .. expected)
    print()
end

function expect.line_count_ge(expected, command)
    local count = 0
    monitor:send(command, function(line)
        print(line)
        count = count + 1
    end)
    assert(count >= expected, "line count not >= " .. expected)
    print()
end

return expect
