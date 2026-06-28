#pragma once

#include <iostream>

namespace alexi::action {
    using Flag = unsigned long;

    #if defined(_WIN32)
    const Flag Flag_Max = 1 << (sizeof(Flag)-1);
    #else
    const Flag Flag_Max = 1 << sizeof(Flag);
    #endif
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
        UNKNOWN = Flag_Max
    };

    using Self = Action;
    Self operator&(const Self &, const Self &);
    Self operator&=(const Self &, const Self &rhs);
    Self operator|(const Self &, const Self &);
    Self operator|=(const Self &, const Self &);
    std::ostream &operator<<(std::ostream &, const Self &);
}

namespace alexi {
    using alexi::action::Action;
}
