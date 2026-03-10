-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

rule("ok.coverage", function()
    before_build(function(target)
        -- `on_config` does not get run for test targets and the path needs to be unique per target/binary.
        local profile_instr = "-fprofile-instr-generate=" .. path.join("coverage", target:name() .. ".profraw")
        target:add("cxflags", profile_instr, "-fcoverage-mapping", {tools = {"clang", "clangxx", "clang_cl", "emcc", "emxx"}})
        target:add("ldflags", profile_instr, {tools = {"clang", "clangxx", "emcc", "emxx", "lld"}})
        target:add("shflags", profile_instr, {tools = {"clang", "clangxx", "emcc", "emxx", "lld"}})
    end)
end)
