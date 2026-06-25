#include "alexi/engine.hpp"

using namespace alexi;

int main(void) {
    auto lex = Engine::from_data("lorem ipsum\ndelor amet\ngreat googly moogily")
        .use_kind({
            .pattern = R"(^[A-Za-z_]\w*)",
            .name = "IDENTIFIER",
            .natural = false,
        })
        .use_kind({
            .pattern = R"([ \t\v]+)",
            .name = "WHITESPACE",
            .action = Action::IGNORE
        })
        .use_kind({
            .pattern = R"(\r\n|\r|\n)",
            .name    = "NEWLINE",
            .action  = Action::IGNORE | Action::MULTILINE
        }).generate();

    while (1) {
        auto t = lex.next_token();
        if (t == "EOF") break;
        std::cout << t << std::endl;
    }
    return 0;
}
