-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

task("mull", function()
    set_category("okl-plugin")
    set_menu({
        usage = "xmake mull [options] [tests]",
        description = "Build and run tests with mull's mutation testing.",
        options = {
            {nil, "allow_surviving", "k", nil, "Do not treat surviving mutants as an error."},
            {nil, "frontend", "kv", nil, "Path(s) to the mull-ir-frontend plugin (without the mull-ir-frontend file)."},
            {'g', "group", "kv", nil, "Run all tests of the given group. Supports pattern matching.",
                "e.g.",
                "    xmake mull -g test",
                "    xmake mull -g test_*",
                "    xmake mull --group=benchmark/*"},
            {'j', "jobs", "kv", tostring(os.default_njob()), "Set the number of parallel compilation and mutation jobs."},
            {nil, "mutation_score_threshold", "kv", tostring(100), "Minimum mutation score (0-100) required for success."},
            {"o", "output", "kv", nil, "Output directory for the reports."},
            {nil, "reporters", "kv", "IDE", "Output reporters to use, more than one can be used at the same time."},
            {'r', "rebuild", "k",  nil, "Rebuild the test targets."},
            {nil, "tests", "v", nil, "Run mutation testing for the given tests. Supports pattern matching.", "e.g.",
                "    xmake mull foo",
                "    xmake mull */foo",
                "    xmake mull targetname/*"}
        }
    })
    on_run("main")
end)
