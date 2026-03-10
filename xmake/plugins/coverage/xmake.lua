-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

task("coverage", function()
    set_category("okl-plugin")
    set_menu({
        usage = "xmake coverage [options] [target]",
        description = "Generate LLVM code coverage reports from raw profiles.",
        options = {
            {"f", "format", "kv", "html", "Output format (html, text, lcov)."},
            {"o", "output", "kv", nil, "Output directory for the report."},
            {nil, "targets", "v", nil, "Generate report for the given targets."}
        }
    })
    on_run("main")
end)
