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

    inline constexpr void ostream_helper(std::ostream &os, const Self &self) {
        #define __strcase(T) case Self::T: os << "["#T"]"; break
        switch (self) {
            __strcase(NOTHING);
            __strcase(CONSUME);
            __strcase(IGNORE);
            __strcase(MULTILINE);
            __strcase(UEOF);
            __strcase(UNKNOWN);
        };
        #undef __strcase
    }

    inline constexpr void ostream_helper(std::ostream &os, const Self &self, const Self &against) {
        if ((self & against) == against) ostream_helper(os, self & against);
    }

    std::ostream &operator<<(std::ostream &os, const Self &self) {
        ostream_helper(os, self, Action::CONSUME);
        ostream_helper(os, self, Action::IGNORE);
        ostream_helper(os, self, Action::MULTILINE);
        ostream_helper(os, self, Action::UEOF);
        ostream_helper(os, self, Action::UNKNOWN);
        return os;
    }
}
