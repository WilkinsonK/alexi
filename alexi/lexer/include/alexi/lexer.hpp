#pragma once

#include "alexi/aliases.hpp"
#include "alexi/matchers.hpp"
#include "alexi/token.hpp"

namespace alexi::lexer {
    struct Lexer;
    using Self = Lexer;
}

namespace alexi {
    using alexi::lexer::Lexer;
}

namespace alexi::lexer {
    // The main driver of lexical analysis. Reads through
    // a data stream attempting to match against its
    // configured token matchers.
    ALEXI_STRUCT(Lexer) {
        // The current location of where the lexer is
        // parsing tokens from in the data stream.
        Marker marker;
        // The rules that govern what kind of tokens this
        // lexer is expecting.
        Matchers matchers;
        // The data stream the lexer is parsing from.
        Str data;

        // Retrieve the next token in the data stream. If
        // the lexer has reached the end of the stream, it
        // will instead return an "EOF" token.
        Token next_token(void);

        private:
        Token handle_next_token(Token &&token);
        Token match_next_token(void);
    };
}
