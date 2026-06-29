#include <catch2/catch_test_macros.hpp>

#include "alexi/action.hpp"
#include "alexi/kind.hpp"
#include "alexi/kinds.hpp"
#include "alexi/token.hpp"
#include "alexi/location.hpp"

using namespace alexi;

TEST_CASE("Token kind can initialize", "[kind]") {
    Kind k{
        .pattern = R"(\w)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
    };

    REQUIRE(k.order == .1);
}

TEST_CASE("Token kinds can be equated deeply", "[kind]") {
    Kind k1{
        .pattern = R"(\w)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
    };

    Kind k2{
        .pattern = R"(\w)",
        .name    = "UNIDENTIFIED",
        .action  = Action::UNKNOWN,
    };

    Kind k3{
        .pattern = R"(\w)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
    };

    REQUIRE(k1 != k2);
    REQUIRE(k1 == k3);
}

TEST_CASE("Token kinds can be compared by order", "[kind]") {
    Kind k1{
        .pattern = R"(\w)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
        .order   = .064
    };

    Kind k2{
        .pattern = R"(\w)",
        .name    = "UNIDENTIFIED",
        .action  = Action::UNKNOWN,
        .order   = .128
    };

    Kind k3{
        .pattern = R"(\w)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
        .order   = .01
    };

    REQUIRE(k3 < k1);
    REQUIRE(k2 > k1);
}

TEST_CASE("Token kinds are hashable", "[kind]") {
    Kind k1{
        .pattern = R"(\w)",
        .name    = "UKNOWN",
        .action  = Action::UNKNOWN,
        .order   = .64
    };

    std::size_t value = std::hash<Kind>{}(k1);
    REQUIRE(value != 0);
    REQUIRE(value != -1);
}

TEST_CASE("Token kinds have consistent hash", "[kind]") {
    Kind k1{
        .pattern = R"(\w)",
        .name    = "SAME",
        .action  = Action::UNKNOWN,
    };

    std::size_t hash1 = std::hash<Kind>{}(k1);
    std::size_t hash2 = std::hash<Kind>{}(k1);
    REQUIRE(hash1 == hash2);
}

TEST_CASE("Different kinds produce different hashes", "[kind]") {
    Kind k1{
        .pattern = R"(\w)",
        .name    = "KIND1",
        .action  = Action::UNKNOWN,
    };
    Kind k2{
        .pattern = R"(\d)",
        .name    = "KIND2",
        .action  = Action::CONSUME,
    };

    std::size_t hash1 = std::hash<Kind>{}(k1);
    std::size_t hash2 = std::hash<Kind>{}(k2);
    REQUIRE(hash1 != hash2);
}

TEST_CASE("Token kinds can be stored", "[kinds]") {
    Kinds kinds;
    kinds.add({ .pattern = R"(\w)", .name = "NOTHING" });

    REQUIRE(kinds.find("NOTHING") != nullptr);
    REQUIRE(kinds.find("SOMETHING") == nullptr);
}

TEST_CASE("Token kinds can be dropped", "[kinds]") {
    Kinds kinds;
    kinds.add({ .pattern = R"(\w)", .name = "NOTHING" });

    REQUIRE(kinds.drop("NOTHING")->name == "NOTHING");
    REQUIRE(kinds.find("NOTHING") == nullptr);
}

TEST_CASE("Token kinds can be stored using |=", "[kinds]") {
    Kinds kinds;
    kinds |= { .pattern = R"(\w)", .name = "NOTHING" };

    REQUIRE(kinds["NOTHING"]->name == "NOTHING");
    REQUIRE(kinds["SOMETHING"] == nullptr);
}

TEST_CASE("Token kinds can be dropped using ()", "[kinds]") {
    Kinds kinds;
    kinds |= { .pattern = R"(\w)", .name = "NOTHING" };

    REQUIRE(kinds["NOTHING"] != nullptr);
    REQUIRE(kinds("NOTHING")->name == "NOTHING");
    REQUIRE(kinds["NOTHING"] == nullptr);
}

TEST_CASE("Token kinds must have an identifier", "[kinds]") {
    Kinds kinds;
    REQUIRE_THROWS(kinds |= { .pattern = R"(\w)", .name = "" });
}

TEST_CASE("Token kinds must have a valid pattern", "[kinds]") {
    Kinds kinds;
    REQUIRE_THROWS(kinds |= { .pattern = R"()", .name = "FAULTY" });
    REQUIRE_THROWS(kinds |= { .pattern = R"(.*)", .name = "FAULTY" });
}

TEST_CASE("Token kinds must have a valid action", "[kinds]") {
    Kinds kinds;
    REQUIRE_THROWS(kinds |= { .pattern = R"(\w)", .name = "FAULTY", .action = Action::NOTHING });
    REQUIRE_THROWS(kinds |= { .pattern = R"(\w)", .name = "FAULTY", .action = Action::IGNORE | Action::CONSUME });
    REQUIRE_THROWS(kinds |= { .pattern = R"(\w)", .name = "FAULTY", .action = Action::CONSUME | Action::IGNORE });
}

TEST_CASE("Token kinds must have an order within the bounds of 0 and 1", "[kinds]") {
    Kinds kinds;
    REQUIRE_THROWS(kinds |= { .pattern = R"(\w)", .name = "TOO_HIGH", .order = 1.2 });
    REQUIRE_THROWS(kinds |= { .pattern = R"(\w)", .name = "AT_BOUND", .order = 1.0 });
}

TEST_CASE("Token order boundary at 0.999 is valid", "[kinds]") {
    Kinds kinds;
    REQUIRE_NOTHROW(kinds |= { .pattern = R"(\w)", .name = "VALID_HIGH", .order = 0.999 });
}

TEST_CASE("Token order boundary at 0.0 is valid", "[kinds]") {
    Kinds kinds;
    REQUIRE_NOTHROW(kinds |= { .pattern = R"(\w)", .name = "VALID_LOW", .order = 0.0 });
}

// ============================================================================
// TOKEN TYPE TESTS
// ============================================================================

TEST_CASE("Token UEOF creates end-of-file token", "[token]") {
    Mark mark;
    Token ueof = Token::UEOF(mark);
    
    REQUIRE(ueof.kind->name == "UEOF");
    REQUIRE(ueof.view.empty());
}

TEST_CASE("Token equality with Kind name", "[token]") {
    Kind k{.pattern = R"(\w)", .name = "IDENTIFIER"};
    Token t{.kind = std::make_shared<Kind>(k), .view = "myvar", .mark = {}};
    
    REQUIRE(t == "IDENTIFIER");
    REQUIRE(!(t == "UNKNOWN"));
}

TEST_CASE("Token equality with char* name", "[token]") {
    Kind k{.pattern = R"([0-9]+)", .name = "NUMBER"};
    Token t{.kind = std::make_shared<Kind>(k), .view = "42", .mark = {}};
    
    REQUIRE(t == "NUMBER");
}

TEST_CASE("Token size matches view length", "[token]") {
    Kind k{.pattern = R"(\w+)", .name = "WORD"};
    Str view = "hello";
    Token t{.kind = std::make_shared<Kind>(k), .view = view, .mark = {}};
    
    REQUIRE(t.size() == view.length());
    REQUIRE(t.size() == 5);
}

TEST_CASE("Token get_action returns kind action", "[token]") {
    Kind k{.pattern = R"( +)", .name = "SPACE", .action = Action::IGNORE};
    Token t{.kind = std::make_shared<Kind>(k), .view = "   ", .mark = {}};
    
    REQUIRE(t.get_action() == Action::IGNORE);
}
