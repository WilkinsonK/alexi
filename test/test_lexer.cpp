#include <catch2/catch_test_macros.hpp>

#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"

using namespace alexi;

TEST_CASE("Lexer is capable of producing tokens", "[lexer]") {
    Kinds ks;
    ks.add(kinds::COMMA);
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = R"(Hello darkness, my "old friend")"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0] == "IDENTIFIER");
    REQUIRE(tokens[0].view == "Hello");
    REQUIRE(tokens[1] == "IDENTIFIER");
    REQUIRE(tokens[1].view == "darkness");
    REQUIRE(tokens[2] == "COMMA");
    REQUIRE(tokens[4] == "LITERALSTR");
    REQUIRE(tokens[5] == "UEOF");
}

TEST_CASE("Lexer throws an error on unexpected tokens", "[lexer]") {
    Kinds ks;
    ks.add(kinds::COMMA);
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = R"(Hello darkness[], my "old friend")"  // [] not matched
    };

    REQUIRE_THROWS([
&l]() {
        Vec<Token> tokens;
        while (1) {
            auto t = l.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
    }());
}

TEST_CASE("Lexer tracks lines correctly", "[lexer]") {
    Kinds ks;
    ks.add(kinds::COMMA);
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "Hello darkness\n, my 'old\n friend'"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens.size() == 6);  // Hello, darkness, COMMA, my, 'old friend', UEOF
    REQUIRE(tokens[0].mark.line.id == 1);
    REQUIRE(tokens[0].view == "Hello");
    REQUIRE(tokens[1] == "IDENTIFIER");
    REQUIRE(tokens[2].mark.line.id == 2);
    REQUIRE(tokens[2].view == ",");
    REQUIRE(l.marker.line.id == 3);
}

TEST_CASE("Lexer with empty input returns only EOF", "[lexer]") {
    Kinds ks;
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = ""
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens[0] == "UEOF");
}

TEST_CASE("Lexer single token before EOF", "[lexer]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "hello"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == "IDENTIFIER");
    REQUIRE(tokens[0].view == "hello");
    REQUIRE(tokens[1] == "UEOF");
}

TEST_CASE("Lexer respects token order precedence", "[lexer]") {
    Kinds ks;
    // IDENTIFIER has higher order (matches later) than number
    ks.add(kinds::LITERALNUM);
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "123 abc"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    REQUIRE(tokens.size() == 3);  // 123, WHITESPACE, abc, UEOF
    REQUIRE(tokens[0] == "LITERALNUM");
    REQUIRE(tokens[1] == "IDENTIFIER");
}

TEST_CASE("Lexer ignores whitespace when configured", "[lexer]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Lexer l{
        .marker = {},
        .matchers = ks,
        .data = "a   b   c"
    };

    Vec<Token> tokens;
    while (1) {
        auto t = l.next_token();
        tokens.push_back(t);
        if (t == kinds::UEOF) break;
    }

    // Whitespace is ignored by action=IGNORE
    REQUIRE(tokens.size() == 4);  // a, b, c, UEOF
    REQUIRE(tokens[0].view == "a");
    REQUIRE(tokens[1].view == "b");
    REQUIRE(tokens[2].view == "c");
}
