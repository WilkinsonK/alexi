#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"

using namespace alexi;

int main(void) {
    auto lex = Engine()
        .use_keywords({
            "let",
            "use",
            "kwd"
        })
        .use_kinds({
            kinds::PARENL,
            kinds::PARENR,
            kinds::IDENTIFIER
        })
        .from_data("(lorem use ipsum\ndelor amet\ngreat googly moogily)\n\n");

    while (1) {
        auto t = lex.next_token();
        std::cout << t << std::endl;
        if (t == "UEOF") break;
    }
    return 0;
}
