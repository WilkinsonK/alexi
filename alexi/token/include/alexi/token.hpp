#pragma once

#include <iostream>
#include <memory>

#include "alexi/aliases.hpp"
#include "alexi/location.hpp"
#include "alexi/kind.hpp"

namespace alexi {
    ALEXI_STRUCT(Token) {
        const std::shared_ptr<Kind> kind;
        const Str                   view;
        const Mark                  mark;

        Action get_action(void) const;
        Len size(void) const;
        bool operator==(const Kind &) const;
        bool operator==(const char *) const;
        bool operator==(const Str &) const;
        bool operator==(const StrV &) const;
    };

    std::ostream &operator<<(std::ostream &, const Token &);
}
