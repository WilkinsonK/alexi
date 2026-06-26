set_default(false)
add_requires("catch2 v3.15.1")

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
        declare_suite_tests(...)
end

declare_suite("token", "kind", "kinds")
declare_suite("match", "match")
