-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

task("oklib", function()
    set_category("Okl plugin")
    on_run("main")
    set_menu {
        usage = "xmake oklib [options] [library]",
        description = "Create a new library.",
        options = {
            {'f', "force", "k", nil, "Force to create library in a non-empty directory."},
            {'l', "language", "kv", "c++", "The library's language."},
            {nil, "library", "v", nil, "Create the library with the given name."}
        }
    }
end)
