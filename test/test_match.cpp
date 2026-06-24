#pragma clang diagnostic ignored "-Wc2y-extensions"
#include <catch2/catch_test_macros.hpp>
import alexi.match;
import alexi.util;
import std;

using namespace alexi;

TEST_CASE("Token can be produced from stream", "[match]") {
    Matcher m = Matcher::from({
        .pattern = R"([A-Za-z_]\w*)",
        .name    = "IDENTIFIER",
    });

    auto s = std::string_view("hello world?");
    auto t = m.match(s);
    REQUIRE(t != std::nullopt);
    REQUIRE(t.value() == "hello");
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
