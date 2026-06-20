add_requires("catch2 v3.15.1")

target("test_token")
    set_kind("binary")
    add_packages("catch2")
    add_deps("alexi_shared")
    add_files("*.cpp")
    add_tests("token_kind", { runargs = {"[token_kind]"}})
