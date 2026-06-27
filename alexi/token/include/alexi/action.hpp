#pragma once

#include <iostream>

namespace alexi {
    using Flag = unsigned long;
    // Determines token handling behavior if/when a lexer
    // is able to match some token.
    enum class Action : Flag {
        // Should not be used. Is effectively "UNKNOWN".
        NOTHING = 0,
        // Consume the token and return it. Move the lexer
        // forward.
        CONSUME = 1 << 0,
        // Ignore the token and continue looking for
        // another. Move the lexer forward.
        IGNORE = 1 << 1,
        // Token is potentially a multiline value. Consume
        // or ignore, move the lexer forward and update the
        // line counter.
        MULTILINE = 1 << 3,
        // Technically an unexpected EOF. Either quit
        // analysis or panic.
        UEOF = 1 << 4,
        // Panic as the token is not known to the lexer.
        UNKNOWN = 1 << sizeof(Flag)
    };

    Action operator&(const Action &, const Action &);
    Action operator&=(const Action &, const Action &rhs);
    Action operator|(const Action &, const Action &);
    Action operator|=(const Action &, const Action &);
    std::ostream &operator<<(std::ostream &, const Action &);
}
