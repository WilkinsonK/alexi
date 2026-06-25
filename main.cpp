#include "alexi/lexer.hpp"

using namespace alexi;

int main(void) {
    Kinds kinds{};
    kinds.add({
        .pattern = R"(^[A-Za-z_]\w*)",
        .name = "IDENTIFIER",
        .natural = false,
    });

    kinds.add({
        .pattern = R"([ \t\v]+)",
        .name = "WHITESPACE",
    });

    kinds.add({
        .pattern = R"(\r\n|\r|\n)",
        .name    = "NEWLINE",
        .action  = Action::CONSUME | Action::MULTILINE
    });

    alexi::Lexer lex{
        .marker = { .file = "__test__", },
        .matchers = kinds,
        .data = "lorem ipsum\ndelor amet\ngreat googly moogily",
    };

    while (1) {
        auto t = lex.next_token();
        if (t == "EOF") break;
        std::cout << t << std::endl;
    }
    return 0;
}
