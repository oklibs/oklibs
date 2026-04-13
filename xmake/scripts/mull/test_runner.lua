-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

-- Minimal version of xmake `action/tests/main.lua`.

function _load_output_files(files)
    local outputs = {}

    for _, file in ipairs(files) do
        if not os.isfile(file) then
            raise("file not found: %s", file)
        end
        table.insert(outputs, io.readfile(file))
    end

    return outputs
end

function main(program, opts, ...)
    opts = string.deserialize(opts)

    local outfile = os.tmpfile()
    local run_timeout = opts.run_timeout
    local ok, _ = os.execv(program, {...}, {try = true, timeout = run_timeout, curdir = opts.rundir, envs = opts.runenvs, stdout = outfile})
    local outdata = (outfile and os.isfile(outfile)) and io.readfile(outfile) or ""
    os.rm(outfile)

    if opts.trim_output and outdata then
        outdata = outdata:trim()
    end

    local passed
    if ok == 0 then
        local pass_outputs = table.wrap(opts.pass_outputs)
        local fail_outputs = table.wrap(opts.fail_outputs)
        table.join2(pass_outputs, _load_output_files(opts.pass_output_files))
        table.join2(fail_outputs, _load_output_files(opts.fail_output_files))

        for _, pass_output in ipairs(pass_outputs) do
            if opts.plain then
                if pass_output == outdata then
                    passed = true
                    break
                end
            else
                if outdata:match("^" .. pass_output .. "$") then
                    passed = true
                    break
                end
            end
        end

        for _, fail_output in ipairs(fail_outputs) do
            if opts.plain then
                if fail_output == outdata then
                    passed = false
                    break
                end
            else
                if outdata:match("^" .. fail_output .. "$") then
                    passed = false
                    break
                end
            end
        end

        passed = passed ~= nil and passed or #pass_outputs == 0
    end

    if opts.should_fail then
        passed = not passed and true or false
    end

    if not passed then
        raise()
    end
end
