#pragma once

#include <iostream>

#include "alexi/aliases.hpp"
#include "alexi/location.hpp"
#include "alexi/kind.hpp"

namespace alexi {
    ALEXI_STRUCT(Token) {
        const std::shared_ptr<Kind> kind;
        const Str                   view;
        const Mark                  mark;

        Self set_mark(const Mark);
        Len size(void) const;
        bool operator==(const Kind &);
        bool operator==(const char *);
        bool operator==(const Str &);
        bool operator==(const StrV &);
    };

    std::ostream &operator<<(std::ostream &, const Token &);
}
