set_default(false)

target("basic_parsing")
    set_kind("binary")
    add_files("basic_parsing/main.cpp")
    add_deps("alexi")
    add_defines("LANG_FILE=\"$(projectdir)/examples/basic_parsing/lang.alexi\"")

target("basic_usage")
    set_kind("binary")
    add_files("basic_usage/main.cpp")
    add_deps("alexi")
