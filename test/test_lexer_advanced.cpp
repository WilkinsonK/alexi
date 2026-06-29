#include <catch2/catch_test_macros.hpp>

#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"

using namespace alexi;

// ============================================================================
// MULTILINE STRING LITERALS - BASIC
// ============================================================================

TEST_CASE("Lexer handles simple multiline string literal", "[lexer][multiline]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "\"line1\nline2\""
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == "LITERALSTR");
    REQUIRE(tokens[0].view == "\"line1\nline2\"");
    REQUIRE(l.marker.line.id == 2);  // After processing, should be on line 2
}

TEST_CASE("Lexer preserves newlines within string literals", "[lexer][multiline]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "'multi\nline\nstring'"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == "LITERALSTR");
    // String should contain both lines
    REQUIRE(tokens[0].view.find('\n') != std::string::npos);
}

TEST_CASE("Lexer triple-quoted string literal", "[lexer][multiline]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::UNKNOWN);

    Str data = """\"\"\"This is\na triple\nquoted string\"\"\"";
    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = data
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // Should match at least one string token
    auto string_token = std::find_if(tokens.begin(), tokens.end(),
        [](const Token& t) { return t == "LITERALSTR"; });
    REQUIRE(string_token != tokens.end());
}

// ============================================================================
// NEWLINE STYLE HANDLING
// ============================================================================

TEST_CASE("Lexer counts Unix newlines correctly (\\n)", "[lexer][newline]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "line1\nline2\nline3"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // Should process 3 lines
    REQUIRE(l.marker.line.id == 3);
}

TEST_CASE("Lexer counts Windows newlines correctly (\\r\\n)", "[lexer][newline]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "line1\r\nline2\r\nline3"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // Should still process 3 lines (not 6), because \r\n is one newline
    REQUIRE(l.marker.line.id == 3);
}

TEST_CASE("Lexer counts old Mac newlines correctly (\\r)", "[lexer][newline]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "line1\rline2\rline3"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // Should process 3 lines
    REQUIRE(l.marker.line.id == 3);
}

TEST_CASE("Lexer handles mixed newline styles", "[lexer][newline]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "line1\nline2\r\nline3\rline4"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // Should correctly count as 4 lines
    REQUIRE(l.marker.line.id == 4);
}

// ============================================================================
// CONSECUTIVE NEWLINES
// ============================================================================

TEST_CASE("Lexer handles two consecutive newlines", "[lexer][newline]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "line1\n\nline3"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(l.marker.line.id == 3);
}

TEST_CASE("Lexer handles three consecutive newlines", "[lexer][newline]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "line1\n\n\nline4"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(l.marker.line.id == 4);
}

// ============================================================================
// MULTILINE STRING WITH EMBEDDED NEWLINES - POSITION TRACKING
// ============================================================================

TEST_CASE("Lexer multiline string updates line count correctly", "[lexer][multiline]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    // Start at position 1, line 1
    // After multiline string with 2 newlines, should be on line 3
    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "\"string\nwith\nnewlines\" after"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // After the multiline string, next token (identifier "after") should be on line 3
    REQUIRE(tokens[0] == "LITERALSTR");
    REQUIRE(tokens[1] == "IDENTIFIER");
    REQUIRE(tokens[1].view == "after");
    REQUIRE(tokens[1].mark.line.id == 3);
}

TEST_CASE("Lexer absolute position tracking through multiline", "[lexer][multiline]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "\"a\nb\nc\" x"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens[0] == "LITERALSTR");
    REQUIRE(tokens[1] == "IDENTIFIER");
    
    // After multiline string spanning 3 lines, marker.position should reflect consumption
    REQUIRE(l.marker.position > 1);
}

// ============================================================================
// ESCAPED CHARACTERS IN STRINGS
// ============================================================================

TEST_CASE("Lexer handles escaped newline in string", "[lexer][multiline]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "\"escaped\\nnewline\""
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens[0] == "LITERALSTR");
    // Line counter should NOT increment for escaped newlines
    // (only actual newlines count - regex matches actual \r\n|\r|\n)
    REQUIRE(l.marker.line.id == 1);
}

TEST_CASE("Lexer handles escaped quote in string", "[lexer][multiline]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "\"string with \\\" quote\""
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens[0] == "LITERALSTR");
    REQUIRE(l.marker.line.id == 1);
}

// ============================================================================
// POSITION AND LINE TRACKING ACCURACY
// ============================================================================

TEST_CASE("Lexer position at token start is correct", "[lexer][position]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "hello world"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // First token should start at position 1 (default)
    REQUIRE(tokens[0].mark.position == 1);
    
    // Second token should start after "hello " (position 7)
    REQUIRE(tokens[1].mark.position == 7);
}

TEST_CASE("Lexer line position tracks correctly", "[lexer][position]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "first\nsecond"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens[0] == "IDENTIFIER");
    REQUIRE(tokens[0].mark.line.id == 1);
    
    // After newline, next token on line 2
    REQUIRE(tokens[1] == "IDENTIFIER");
    REQUIRE(tokens[1].mark.line.id == 2);
}

// ============================================================================
// COMPLEX MULTILINE SCENARIOS
// ============================================================================

TEST_CASE("Lexer handles code-like multiline structure", "[lexer][multiline]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::LITERALNUM);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::BRACEL);
    ks.add(kinds::BRACER);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "function {\n  \"multiline\nstring\"\n  42\n}"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // Should process without throwing
    REQUIRE(!tokens.empty());
    REQUIRE(l.marker.line.id >= 3);  // At least 3 lines processed
}

TEST_CASE("Lexer multiline string followed by single-line tokens", "[lexer][multiline]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::COMMA);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "\"multiline\nstring\", token, another"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens[0] == "LITERALSTR");
    REQUIRE(tokens[1] == "COMMA");
    REQUIRE(tokens[2] == "IDENTIFIER");
    REQUIRE(tokens[2].view == "token");
    REQUIRE(tokens[2].mark.line.id == 2);
}

// ============================================================================
// MULTILINE ACTION FLAG VERIFICATION
// ============================================================================

TEST_CASE("LITERALSTR kind has MULTILINE action", "[lexer][action]") {
    // Verify the built-in LITERALSTR kind has MULTILINE flag
    REQUIRE((kinds::LITERALSTR.action & Action::MULTILINE) == Action::MULTILINE);
}

TEST_CASE("NEWLINE kind has MULTILINE action", "[lexer][action]") {
    REQUIRE((kinds::NEWLINE.action & Action::MULTILINE) == Action::MULTILINE);
}

TEST_CASE("IDENTIFIER kind does NOT have MULTILINE action", "[lexer][action]") {
    REQUIRE((kinds::IDENTIFIER.action & Action::MULTILINE) != Action::MULTILINE);
}

// ============================================================================
// ERROR CASES WITH MULTILINE
// ============================================================================

TEST_CASE("Lexer throws on unmatched token in multiline context", "[lexer][multiline][error]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        // Unmatched ']' after multiline string
        .data = "\"multiline\nstring\"] bad"
    };

    REQUIRE_THROWS([&l]() {
        while (1) {
            auto t = l.next_token();
            if (t == kinds::UEOF) break;
        }
    }());
}

// ============================================================================
// REAL-WORLD LANGUAGE PATTERNS
// ============================================================================

TEST_CASE("Lexer handles Python-like multiline string", "[lexer][multiline][scenario]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::ASSIGN);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "message = \"\"\"Hello\nWorld\nPython\"\"\""
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // Should parse: message, =, """...string..."""
    auto it = std::find_if(tokens.begin(), tokens.end(),
        [](const Token& t) { return t == "LITERALSTR"; });
    REQUIRE(it != tokens.end());
    REQUIRE(it->mark.line.id == 1);  // String starts on line 1
}

TEST_CASE("Lexer handles comment-like multiline content", "[lexer][multiline][scenario]") {
    Kinds ks;
    ks.add(kinds::COMMENT);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        // COMMENT is #.* so spans to end of line only
        .data = "# comment line 1\nidentifier\n# comment line 2"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(!tokens.empty());
}
