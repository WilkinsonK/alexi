#pragma clang diagnostic ignored "-Wc2y-extensions"
#include <catch2/catch_test_macros.hpp>

#include "alexi/action.hpp"
#include "alexi/kind.hpp"
#include "alexi/kinds.hpp"
#include "alexi/matcher.hpp"

using namespace alexi;

TEST_CASE("Token kind can initialize", "[kind]") {
    Kind k{
        .pattern = R"(.*)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
    };

    REQUIRE(k.order == 1.0);
}

TEST_CASE("Token kinds can be equated deeply", "[kind]") {
    Kind k1{
        .pattern = R"(.*)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
    };

    Kind k2{
        .pattern = R"(.*)",
        .name    = "UNIDENTIFIED",
        .action  = Action::UNKNOWN,
    };

    Kind k3{
        .pattern = R"(.*)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
    };

    REQUIRE(k1 != k2);
    REQUIRE(k1 == k3);
}

TEST_CASE("Token kinds can be compared by order", "[kind]") {
    Kind k1{
        .pattern = R"(.*)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
        .order   = 64
    };

    Kind k2{
        .pattern = R"(.*)",
        .name    = "UNIDENTIFIED",
        .action  = Action::UNKNOWN,
        .order   = 128
    };

    Kind k3{
        .pattern = R"(.*)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
        .order   = 1
    };

    REQUIRE(k3 < k1);
    REQUIRE(k2 > k1);
}

TEST_CASE("Token kinds are hashable", "[kind]") {
    Kind k1{
        .pattern = R"(.*)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
        .order   = 64
    };

    std::size_t value = std::hash<Kind>{}(k1);
    REQUIRE(value != 0);
    REQUIRE(value != -1);
    REQUIRE(value == 0x2a701d7796013a09);
}

TEST_CASE("Token kinds can be stored", "[kinds]") {
    Kinds kinds;
    kinds.add({ .pattern = R"()", .name = "NOTHING" });

    REQUIRE(kinds.find("NOTHING") != nullptr);
    REQUIRE(kinds.find("SOMETHING") == nullptr);
}

TEST_CASE("Token kinds can be dropped", "[kinds]") {
    Kinds kinds;
    kinds.add({ .pattern = R"()", .name = "NOTHING" });

    REQUIRE(kinds.drop("NOTHING")->name == "NOTHING");
    REQUIRE(kinds.find("NOTHING") == nullptr);
}

TEST_CASE("Token kinds can be stored using |=", "[kinds]") {
    Kinds kinds;
    kinds |= { .pattern = R"()", .name = "NOTHING" };

    REQUIRE(kinds["NOTHING"]->name == "NOTHING");
    REQUIRE(kinds["SOMETHING"] == nullptr);
}

TEST_CASE("Token kinds can be dropped using ()", "[kinds]") {
    Kinds kinds;
    kinds |= { .pattern = R"()", .name = "NOTHING" };

    REQUIRE(kinds["NOTHING"] != nullptr);
    REQUIRE(kinds("NOTHING")->name == "NOTHING");
    REQUIRE(kinds["NOTHING"] == nullptr);
}
