-- Tests disabled by default, only shown when testing=true is configured

function declare_suite_tests(...)
    local tags = table.pack(...)
    for i = 1, tags.n do
        local tag = tags[i]
        add_tests(tag, {runargs = {"[" .. tag .. "]"}})
    end
end

function declare_suite(name, ...)
    target("test_" .. name)
        set_kind("binary")
        add_packages("catch2")
        add_deps("alexi")
        add_files("test_" .. name .. ".cpp")
        set_group("test")
        declare_suite_tests(...)
end

if has_config("testing") then
    add_requires("catch2 v3.15.1")
    declare_suite("lexer", "lexer")
    declare_suite("lexer_advanced", "lexer")
    declare_suite("token", "kind", "kinds")
    declare_suite("match", "match")
    declare_suite("engine", "engine")
    declare_suite("action", "action")
    declare_suite("location", "location")
    declare_suite("exception", "exception")
end
