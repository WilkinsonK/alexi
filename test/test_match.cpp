#include <optional>
#include <memory>

#include <catch2/catch_test_macros.hpp>

#include "alexi/matcher.hpp"

using namespace alexi;

TEST_CASE("Token can be produced from stream", "[match]") {
    Matcher m = Matcher::from({
        .pattern = R"([A-Za-z_]\w*)",
        .name    = "IDENTIFIER",
    });

    auto s = std::string_view("hello world?");
    auto t = m.match(s);
    REQUIRE(t != std::nullopt);
    REQUIRE(t.value() == "IDENTIFIER");
    REQUIRE(t.value().view == "hello");
}

TEST_CASE("Token only matches on contiguous paths", "[match]") {
    Matcher m = Matcher::from({
        .pattern = R"([A-Za-z_]\w*)",
        .name    = "IDENTIFIER",
    });

    auto s = std::string_view(" hello world?");
    auto t = m.match(s);
    REQUIRE(t == std::nullopt);
}

TEST_CASE("Matcher with digit pattern", "[match]") {
    Matcher m = Matcher::from({
        .pattern = R"(\d+)",
        .name    = "NUMBER",
    });

    auto s = std::string_view("12345 text");
    auto t = m.match(s);
    REQUIRE(t != std::nullopt);
    REQUIRE(t.value().view == "12345");
}

TEST_CASE("Matcher with special character pattern", "[match]") {
    Matcher m = Matcher::from({
        .pattern = R"(\+)",
        .name    = "PLUS",
    });

    auto s = std::string_view("+ addition");
    auto t = m.match(s);
    REQUIRE(t != std::nullopt);
    REQUIRE(t.value().view == "+");
}

TEST_CASE("Matcher fails when pattern doesn't match", "[match]") {
    Matcher m = Matcher::from({
        .pattern = R"([a-z]+)",
        .name    = "LOWERCASE",
    });

    auto s = std::string_view("UPPERCASE");
    auto t = m.match(s);
    REQUIRE(t == std::nullopt);
}

TEST_CASE("Matcher tracks position via Mark parameter", "[match]") {
    Matcher m = Matcher::from({
        .pattern = R"(\w+)",
        .name    = "WORD",
    });

    Mark mark{.file = FILE_DEFAULT, .line = {.id = 2}, .position = 5};
    std::string_view s = "test";
    auto t = m.match(s, mark);
    
    REQUIRE(t != std::nullopt);
    REQUIRE(t.value().mark.position == 5);
    REQUIRE(t.value().mark.line.id == 2);
}

TEST_CASE("Matcher from Kind reference", "[match]") {
    Kind k{.pattern = R"(\*)", .name = "STAR"};
    Matcher m = Matcher::from(k);
    
    auto t = m.match(std::string_view("*"), Mark{});
    REQUIRE(t != std::nullopt);
}

TEST_CASE("Matcher from shared_ptr Kind", "[match]") {
    auto k = std::make_shared<Kind>(Kind{.pattern = R"(/)", .name = "SLASH"});
    Matcher m = Matcher::from(k);
    
    auto t = m.match(std::string_view("/"), Mark{});
    REQUIRE(t != std::nullopt);
}

TEST_CASE("Matcher with string_view", "[match]") {
    Matcher m = Matcher::from({
        .pattern = R"(\w+)",
        .name    = "IDENT",
    });

    std::string_view view = "hello";
    auto t = m.match(view, Mark{});
    
    REQUIRE(t != std::nullopt);
    REQUIRE(t.value().view == "hello");
}

TEST_CASE("Matcher with string", "[match]") {
    Matcher m = Matcher::from({
        .pattern = R"(\w+)",
        .name    = "IDENT",
    });

    Str str = "hello";
    auto t = m.match(str, Mark{});
    
    REQUIRE(t != std::nullopt);
    REQUIRE(t.value().view == "hello");
}

TEST_CASE("Matcher with predicate validation", "[match]") {
    Kind k{
        .pattern = R"(\w+)",
        .name = "KEYWORD",
        .predicate = [](const std::string_view s) { 
            return s == "if" || s == "else" || s == "while";
        }
    };
    Matcher m = Matcher::from(k);
    
    std::string_view sv_keyword = "if test";
    std::string_view sv_non_keyword = "myvar";
    
    auto match_keyword = m.match(sv_keyword, Mark{});
    auto match_non_keyword = m.match(sv_non_keyword, Mark{});
    
    REQUIRE(match_keyword != std::nullopt);
    REQUIRE(match_non_keyword == std::nullopt);
}
