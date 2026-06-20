target("alexi_shared")
    set_kind("shared")
    add_files("*.cppm", "token/*.cppm", "util/*.cppm", { public = true })
    add_rules("utils.symbols.export_all", {export_classes = true})
