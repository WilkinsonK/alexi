module;
#include "util_common.hpp"

export module alexi.token:token;
import :kind;
import alexi.util;
import std;

namespace alexi {
    export ALEXI_STRUCT(Token) {
        const std::shared_ptr<Kind> kind;
        const Str                   view;
        const Mark                  mark;

        Self set_mark(const Mark);
        Len size(void) const;
        constexpr bool operator==(const Kind &);
        constexpr bool operator==(const char *);
        constexpr bool operator==(const Str &);
        constexpr bool operator==(const StrV &);
    };

    Token Token::set_mark(const Mark mark) {
        return {
            .kind = kind,
            .view = view,
            .mark = mark
        };
    }

    Len Token::size(void) const {
        return view.size();
    }

    constexpr inline bool Token::operator==(const Kind &other) {
        return *kind == other;
    }

    constexpr inline bool Token::operator==(const char *other) {
        return view == other;
    }

    constexpr inline bool Token::operator==(const Str &other) {
        return view == other;
    }

    constexpr inline bool Token::operator==(const StrV &other) {
        return view == other;
    }

    export constexpr inline std::ostream &operator<<(std::ostream &os, const Token &token) {
        #define __structfield(N) "."#N"=" << kind . N
        #define __structstrfield(N) "."#N"=\"" << kind . N << "\""
        os << "<Token[" << token.kind->name << "]";
        if (token.kind->natural) os << " {" << token.view << "}";
        os << ">";
        return os;
        #undef __structstrfield
        #undef __structfield
    }
}
