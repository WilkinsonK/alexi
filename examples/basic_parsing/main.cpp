#include <format>
#include <ranges>
#include <stdexcept>
#include <variant>

#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"

using namespace std::literals;
using namespace alexi;

#ifndef LANG_FILE
#define LANG_FILE "lang.alexi"
#endif

const Vec<Str> KEYWORDS{"kwd", "use"};
const Vec<Kind> KINDS{
    kinds::LITERALSTR,
    kinds::SEMICOLON,
    kinds::IDENTIFIER
};

struct Statment;

struct KwdStatement {
    Token keyword;
};

struct UseStatement {
    Token kind;
};

struct Statement {
    using Inner = std::variant<
        KwdStatement,
        UseStatement,
        Vec<struct Statement>
    >;

    Inner inner;
};

struct AlexiParser {
    Lexer lexer;

    const Token eat(void) {
        return lexer.next_token();
    }

    const Token expect(Vec<Str> expecting) {
        auto t = eat();
        if (std::find(expecting.begin(), expecting.end(), t.kind->name) == expecting.end()) {
            auto joinit = expecting | std::views::join_with(" or "sv);
            Str expected{joinit.begin(), joinit.end()};
            throw std::runtime_error(std::format("Expected {} but got {}", expected, t.kind->name));
        }
        return t;
    }

    Statement parse(void) {
        std::vector<Statement> inner{};
        return { .inner = inner };
    }

    Token next_token(void) {
        return lexer.next_token();
    }
};

struct Alexi {
    Engine engine;

    Alexi(void) : engine() {
        engine = engine.use_keywords(KEYWORDS).use_kinds(KINDS);
    }

    AlexiParser from_file(Path path) {
        return { .lexer = engine.from_file(path) };
    }
};

int main(void) {
    auto parser = Alexi().from_file(LANG_FILE);

    parser.expect({"EOF"});
    return 0;
}
