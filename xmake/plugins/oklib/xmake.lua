-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

task("oklib", function()
    set_category("Okl plugin")
    on_run("main")
    set_menu {
        usage = "xmake oklib [options] [target]",
        description = "Create a new library.",
        options = {
            {'f', "force", "k", nil, "Force to create project in a non-empty directory."},
            {'l', "language", "kv", "c++", "The project language."},
            {nil, "target", "v", nil, "Create the given target."}
        }
    }
end)
