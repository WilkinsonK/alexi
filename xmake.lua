set_languages("c++23", "c23")
set_warnings("all", "error", "pedantic")
add_rules("plugin.compile_commands.autoupdate")
add_rules("mode.debug", "mode.release")

-- Ensures that the toolchain and runtimes are compatible
-- with C++ modules.
if is_host("macosx") then
    set_config("toolchain", "clang")
    set_config("runtimes", "c++_shared")
end

includes("alexi")
includes("test")

target("alexi_main")
    set_kind("binary")
    add_files("main.cpp")
    add_deps("alexi")
