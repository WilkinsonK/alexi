#include "alexi/action.hpp"

namespace alexi::action {
    Self operator&(const Self &lhs, const Self &rhs) {
        return static_cast<Self>(static_cast<Flag>(lhs) & static_cast<Flag>(rhs));
    }

    Self operator&=(const Self &lhs, const Self &rhs) {
        return lhs & rhs;
    }

    Self operator|(const Self &lhs, const Self &rhs) {
        return static_cast<Self>(static_cast<Flag>(lhs) | static_cast<Flag>(rhs));
    }

    Self operator|=(const Self &lhs, const Self &rhs) {
        return lhs | rhs;
    }

    std::ostream &operator<<(std::ostream &os, const Self &Self) {
        #define __strcase(T) case Self::T: os << #T; break
        switch (Self) {
            __strcase(NOTHING);
            __strcase(CONSUME);
            __strcase(IGNORE);
            __strcase(MULTILINE);
            __strcase(UEOF);
            __strcase(UNKNOWN);
        };
        return os;
        #undef __strcase
    }
}
