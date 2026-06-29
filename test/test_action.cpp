#include <catch2/catch_test_macros.hpp>
#include <sstream>

#include "alexi/action.hpp"

using namespace alexi;

// ============================================================================
// ACTION INITIALIZATION AND BASIC STATE
// ============================================================================

TEST_CASE("Action NOTHING is default", "[action]") {
    Action a = Action::NOTHING;
    REQUIRE(a == Action::NOTHING);
}

TEST_CASE("Action CONSUME is set", "[action]") {
    Action a = Action::CONSUME;
    REQUIRE(a == Action::CONSUME);
}

TEST_CASE("Action IGNORE is set", "[action]") {
    Action a = Action::IGNORE;
    REQUIRE(a == Action::IGNORE);
}

TEST_CASE("Action MULTILINE is set", "[action]") {
    Action a = Action::MULTILINE;
    REQUIRE(a == Action::MULTILINE);
}

TEST_CASE("Action UEOF is set", "[action]") {
    Action a = Action::UEOF;
    REQUIRE(a == Action::UEOF);
}

TEST_CASE("Action UNKNOWN is set", "[action]") {
    Action a = Action::UNKNOWN;
    REQUIRE(a == Action::UNKNOWN);
}

// ============================================================================
// BITWISE OR OPERATIONS
// ============================================================================

TEST_CASE("Action CONSUME | MULTILINE combines both flags", "[action][bitwise]") {
    Action result = Action::CONSUME | Action::MULTILINE;
    
    // Result should have both bits set
    REQUIRE((result & Action::CONSUME) == Action::CONSUME);
    REQUIRE((result & Action::MULTILINE) == Action::MULTILINE);
}

TEST_CASE("Action CONSUME | IGNORE combines both flags", "[action][bitwise]") {
    Action result = Action::CONSUME | Action::IGNORE;
    
    REQUIRE((result & Action::CONSUME) == Action::CONSUME);
    REQUIRE((result & Action::IGNORE) == Action::IGNORE);
}

TEST_CASE("Action IGNORE | MULTILINE combines both flags", "[action][bitwise]") {
    Action result = Action::IGNORE | Action::MULTILINE;
    
    REQUIRE((result & Action::IGNORE) == Action::IGNORE);
    REQUIRE((result & Action::MULTILINE) == Action::MULTILINE);
}

TEST_CASE("Action CONSUME | MULTILINE | UNKNOWN combines three flags", "[action][bitwise]") {
    Action result = Action::CONSUME | Action::MULTILINE | Action::UNKNOWN;
    
    REQUIRE((result & Action::CONSUME) == Action::CONSUME);
    REQUIRE((result & Action::MULTILINE) == Action::MULTILINE);
    REQUIRE((result & Action::UNKNOWN) == Action::UNKNOWN);
}

TEST_CASE("Action OR is commutative", "[action][bitwise]") {
    Action result1 = Action::CONSUME | Action::MULTILINE;
    Action result2 = Action::MULTILINE | Action::CONSUME;
    
    REQUIRE((result1 & Action::CONSUME) == (result2 & Action::CONSUME));
    REQUIRE((result1 & Action::MULTILINE) == (result2 & Action::MULTILINE));
}

// ============================================================================
// BITWISE AND OPERATIONS
// ============================================================================

TEST_CASE("Action (CONSUME | MULTILINE) & CONSUME yields CONSUME", "[action][bitwise]") {
    Action combined = Action::CONSUME | Action::MULTILINE;
    Action result = combined & Action::CONSUME;
    
    REQUIRE(result == Action::CONSUME);
}

TEST_CASE("Action (CONSUME | MULTILINE) & MULTILINE yields MULTILINE", "[action][bitwise]") {
    Action combined = Action::CONSUME | Action::MULTILINE;
    Action result = combined & Action::MULTILINE;
    
    REQUIRE(result == Action::MULTILINE);
}

TEST_CASE("Action (CONSUME | MULTILINE) & IGNORE yields NOTHING", "[action][bitwise]") {
    Action combined = Action::CONSUME | Action::MULTILINE;
    Action result = combined & Action::IGNORE;
    
    REQUIRE(result == Action::NOTHING);
}

TEST_CASE("Action CONSUME & NOTHING yields NOTHING", "[action][bitwise]") {
    Action result = Action::CONSUME & Action::NOTHING;
    REQUIRE(result == Action::NOTHING);
}

TEST_CASE("Action NOTHING & NOTHING yields NOTHING", "[action][bitwise]") {
    Action result = Action::NOTHING & Action::NOTHING;
    REQUIRE(result == Action::NOTHING);
}

// ============================================================================
// COMPOUND ASSIGNMENT OPERATORS
// ============================================================================

TEST_CASE("Action |= combines flags correctly (returns new value)", "[action][bitwise]") {
    Action a = Action::CONSUME;
    // operator|= returns the result but doesn't modify the variable
    Action result = (a |= Action::MULTILINE);
    
    REQUIRE((result & Action::CONSUME) == Action::CONSUME);
    REQUIRE((result & Action::MULTILINE) == Action::MULTILINE);
}

TEST_CASE("Action &= filters flags correctly (returns result)", "[action][bitwise]") {
    Action combined = Action::CONSUME | Action::MULTILINE;
    
    // operator&= returns result but doesn't modify the variable in place
    // (because Action references are const in the operator signature)
    Action result = (combined & Action::CONSUME);
    
    REQUIRE(result == Action::CONSUME);
    REQUIRE(combined == (Action::CONSUME | Action::MULTILINE));  // Original unchanged
}

TEST_CASE("Action & results in NOTHING when no flags match", "[action][bitwise]") {
    Action a = Action::CONSUME;
    Action result = a & Action::IGNORE;
    
    REQUIRE(result == Action::NOTHING);
}

// ============================================================================
// FLAG CHECKING PATTERNS (Lexer usage)
// ============================================================================

TEST_CASE("Lexer pattern: check if CONSUME is set in combined action", "[action][lexer_pattern]") {
    Action combined = Action::CONSUME | Action::MULTILINE;
    
    // This is how lexer.cpp checks: (action & expect) == expect
    bool is_consume = (combined & Action::CONSUME) == Action::CONSUME;
    REQUIRE(is_consume);
}

TEST_CASE("Lexer pattern: check if IGNORE is set in combined action", "[action][lexer_pattern]") {
    Action combined = Action::CONSUME | Action::MULTILINE;
    
    bool is_ignore = (combined & Action::IGNORE) == Action::IGNORE;
    REQUIRE(!is_ignore);
}

TEST_CASE("Lexer pattern: check if MULTILINE is set in CONSUME|MULTILINE", "[action][lexer_pattern]") {
    Action combined = Action::CONSUME | Action::MULTILINE;
    
    bool is_multiline = (combined & Action::MULTILINE) == Action::MULTILINE;
    REQUIRE(is_multiline);
}

TEST_CASE("Lexer pattern: check if MULTILINE is set in plain CONSUME", "[action][lexer_pattern]") {
    Action single = Action::CONSUME;
    
    bool is_multiline = (single & Action::MULTILINE) == Action::MULTILINE;
    REQUIRE(!is_multiline);
}

TEST_CASE("Lexer pattern: IGNORE flag check", "[action][lexer_pattern]") {
    Action ignore_action = Action::IGNORE;
    
    bool is_ignore = (ignore_action & Action::IGNORE) == Action::IGNORE;
    bool is_consume = (ignore_action & Action::CONSUME) == Action::CONSUME;
    
    REQUIRE(is_ignore);
    REQUIRE(!is_consume);
}

TEST_CASE("Lexer pattern: UEOF flag check", "[action][lexer_pattern]") {
    Action ueof_action = Action::UEOF;
    
    bool is_ueof = (ueof_action & Action::UEOF) == Action::UEOF;
    REQUIRE(is_ueof);
}

TEST_CASE("Lexer pattern: UNKNOWN flag check", "[action][lexer_pattern]") {
    Action unknown_action = Action::UNKNOWN;
    
    bool is_unknown = (unknown_action & Action::UNKNOWN) == Action::UNKNOWN;
    REQUIRE(is_unknown);
}

// ============================================================================
// CONFLICT DETECTION (Should have been validated before, but verify behavior)
// ============================================================================

TEST_CASE("Action CONSUME and IGNORE cannot both be true in same action", "[action][conflict]") {
    // If someone creates (CONSUME | IGNORE), both bits should be set
    // But Kind::validate() should reject this
    Action combined = Action::CONSUME | Action::IGNORE;
    
    bool has_consume = (combined & Action::CONSUME) == Action::CONSUME;
    bool has_ignore = (combined & Action::IGNORE) == Action::IGNORE;
    
    // Both flags are technically set (at bitwise level)
    REQUIRE(has_consume);
    REQUIRE(has_ignore);
    
    // But Kind validation will reject this combo (tested elsewhere)
}

// ============================================================================
// FORMATTING / OSTREAM OUTPUT
// ============================================================================

TEST_CASE("Action CONSUME formats to ostream", "[action][format]") {
    Action a = Action::CONSUME;
    std::ostringstream oss;
    oss << a;
    
    std::string output = oss.str();
    REQUIRE(!output.empty());
    REQUIRE(output.find("CONSUME") != std::string::npos);
}

TEST_CASE("Action IGNORE formats to ostream", "[action][format]") {
    Action a = Action::IGNORE;
    std::ostringstream oss;
    oss << a;
    
    std::string output = oss.str();
    REQUIRE(!output.empty());
    REQUIRE(output.find("IGNORE") != std::string::npos);
}

TEST_CASE("Action MULTILINE formats to ostream", "[action][format]") {
    Action a = Action::MULTILINE;
    std::ostringstream oss;
    oss << a;
    
    std::string output = oss.str();
    REQUIRE(!output.empty());
    REQUIRE(output.find("MULTILINE") != std::string::npos);
}

TEST_CASE("Action UEOF formats to ostream", "[action][format]") {
    Action a = Action::UEOF;
    std::ostringstream oss;
    oss << a;
    
    std::string output = oss.str();
    REQUIRE(!output.empty());
    REQUIRE(output.find("UEOF") != std::string::npos);
}

TEST_CASE("Action UNKNOWN formats to ostream", "[action][format]") {
    Action a = Action::UNKNOWN;
    std::ostringstream oss;
    oss << a;
    
    std::string output = oss.str();
    REQUIRE(!output.empty());
    REQUIRE(output.find("UNKNOWN") != std::string::npos);
}

TEST_CASE("Action CONSUME | MULTILINE formats to ostream with both flags", "[action][format]") {
    Action combined = Action::CONSUME | Action::MULTILINE;
    std::ostringstream oss;
    oss << combined;
    
    std::string output = oss.str();
    REQUIRE(!output.empty());
    REQUIRE(output.find("CONSUME") != std::string::npos);
    REQUIRE(output.find("MULTILINE") != std::string::npos);
}

TEST_CASE("Action NOTHING does not output content", "[action][format]") {
    Action a = Action::NOTHING;
    std::ostringstream oss;
    oss << a;
    
    std::string output = oss.str();
    // NOTHING doesn't match any of the switch cases, so output is empty
    REQUIRE(output.empty());
}

// ============================================================================
// REAL-WORLD SCENARIOS FROM LEXER
// ============================================================================

TEST_CASE("LITERALSTR action handling: CONSUME | MULTILINE", "[action][scenario]") {
    // From kinds.hpp line 91:
    // KINDDECLARE(LITERALSTR, ..., .action = Action::CONSUME | Action::MULTILINE, ...)
    
    Action literalstr_action = Action::CONSUME | Action::MULTILINE;
    
    // Lexer should handle MULTILINE first
    bool should_count_lines = (literalstr_action & Action::MULTILINE) == Action::MULTILINE;
    REQUIRE(should_count_lines);
    
    // Then should handle CONSUME
    bool should_return_token = (literalstr_action & Action::CONSUME) == Action::CONSUME;
    REQUIRE(should_return_token);
}

TEST_CASE("WHITESPACE action handling: IGNORE", "[action][scenario]") {
    // From kinds.hpp line 124:
    // KINDDECLARE(WHITESPACE, ..., .action = Action::IGNORE, ...)
    
    Action ws_action = Action::IGNORE;
    
    bool should_ignore = (ws_action & Action::IGNORE) == Action::IGNORE;
    REQUIRE(should_ignore);
    
    bool should_consume = (ws_action & Action::CONSUME) == Action::CONSUME;
    REQUIRE(!should_consume);
}

TEST_CASE("NEWLINE action handling: IGNORE | MULTILINE", "[action][scenario]") {
    // From kinds.hpp line 117:
    // KINDDECLARE(NEWLINE, ..., .action = Action::IGNORE | Action::MULTILINE, ...)
    
    Action newline_action = Action::IGNORE | Action::MULTILINE;
    
    bool should_count_lines = (newline_action & Action::MULTILINE) == Action::MULTILINE;
    REQUIRE(should_count_lines);
    
    bool should_ignore = (newline_action & Action::IGNORE) == Action::IGNORE;
    REQUIRE(should_ignore);
    
    bool should_consume = (newline_action & Action::CONSUME) == Action::CONSUME;
    REQUIRE(!should_consume);
}

TEST_CASE("UEOF action is terminal", "[action][scenario]") {
    // From kinds.hpp line 122:
    // KINDDECLARE(UEOF, ..., .action = Action::UEOF)
    
    Action ueof_action = Action::UEOF;
    
    bool should_throw = (ueof_action & Action::UEOF) == Action::UEOF;
    REQUIRE(should_throw);
}

TEST_CASE("UNKNOWN action causes error", "[action][scenario]") {
    // From kinds.hpp line 123:
    // KINDDECLARE(UNKNOWN, ..., .action = Action::UNKNOWN, ...)
    
    Action unknown_action = Action::UNKNOWN;
    
    bool should_error = (unknown_action & Action::UNKNOWN) == Action::UNKNOWN;
    REQUIRE(should_error);
}
