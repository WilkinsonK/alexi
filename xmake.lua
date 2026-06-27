set_languages("c++23", "c23")
add_rules("plugin.compile_commands.autoupdate")
add_rules("mode.debug", "mode.release")

-- Ensures that the toolchain and runtimes are compatible
-- with C++ modules.
if is_host("macosx") then
    set_config("toolchain", "clang")
    set_config("runtimes", "c++_shared")
end

option("testing")
    set_default(false)
    set_showmenu(true)
    set_description("Builds project tests")

includes("alexi")
includes("examples")
includes("test")
