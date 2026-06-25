function declare_component(name, ...)
    target("alexi_" .. name)
        set_kind("static")
        add_deps(...)
        add_includedirs(name .. "/include", {public = true})
        add_files(name .. "/*.cpp")

        return "alexi_" .. name
end

components = {
    declare_component("engine", "alexi_lexer", "alexi_token", "alexi_util"),
    declare_component("lexer", "alexi_match", "alexi_token", "alexi_util"),
    declare_component("match", "alexi_token", "alexi_util"),
    declare_component("token", "alexi_util"),
    declare_component("util")
}

target("alexi")
    set_version("3.0.0")
    set_kind("shared")
    add_deps(table.unpack(components))
    add_rules("utils.symbols.export_all")
