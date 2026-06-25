#pragma once

#include "alexi/aliases.hpp"
#include "alexi/matcher.hpp"
#include "alexi/token.hpp"

namespace alexi {
    ALEXI_STRUCT(Lexer) {
        Marker   marker;
        Matchers matchers;
        Str      data;

        Token next_token(void);

        private:
        Token handle_next_token(Token &&token);
        Token match_next_token(void);
    };
}
