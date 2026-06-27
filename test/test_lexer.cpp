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
        .data = R"(Hello darkness[], my "old friend")"
    };

    REQUIRE_THROWS([&l](){
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

    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0].mark.line.id == 1);
    REQUIRE(tokens[2].mark.line.id == 2);
    REQUIRE(l.marker.line.id == 3);
}
