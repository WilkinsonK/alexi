#pragma once

#include <format>
#include <initializer_list>
#include <iostream>
#include <memory>

#include "alexi/aliases.hpp"
#include "alexi/formatting.hpp"
#include "alexi/location.hpp"
#include "alexi/kind.hpp"

namespace alexi::token {
    struct Token;
    using Self = Token;
}

namespace alexi {
    using alexi::token::Token;
}

namespace alexi::token {
    ALEXI_STRUCT(Token) {
        const std::shared_ptr<Kind> kind;
        const Str                   view;
        const Mark                  mark;

        static Self UEOF(const Mark &);

        Action get_action(void) const;
        Len size(void) const;
        bool operator==(const std::initializer_list<Kind>) const;
        bool operator==(const Vec<Kind>) const;
        bool operator==(const Kind&) const;
        bool operator==(const char*) const;
        bool operator==(const Str&) const;
        bool operator==(const StrV&) const;
    };

    std::ostream &operator<<(std::ostream&, const Self&);
}

namespace std {
    using namespace alexi::token;
    ALEXI_FORMATTER(
        Token,
        "<Token[{}] {}{}>",
        t.kind->name,
        t.mark,
        !t.kind->natural ? std::format(" {}", t.view) : ""
    );
}
