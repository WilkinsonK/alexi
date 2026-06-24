export module alexi.token:action;
import std;

namespace alexi {
    using Flag = unsigned int;
    export enum class Action : Flag {
        NOTHING   = 0,
        CONSUME   = 1 << 0,
        IGNORE    = 1 << 1,
        MULTILINE = 1 << 3,
        UNKNOWN   = 1 << sizeof(Flag)
    };

    inline constexpr Action operator&(const Action &lhs, const Action &rhs) {
        return static_cast<Action>(static_cast<Flag>(lhs) & static_cast<Flag>(rhs));
    }

    inline constexpr Action operator&=(const Action &lhs, const Action &rhs) {
        return lhs & rhs;
    }

    inline constexpr Action operator|(const Action &lhs, const Action &rhs) {
        return static_cast<Action>(static_cast<Flag>(lhs) | static_cast<Flag>(rhs));
    }

    inline constexpr Action operator|=(const Action &lhs, const Action &rhs) {
        return lhs | rhs;
    }
}

export namespace std {
    using alexi::Action;

    constexpr ostream &operator<<(ostream &os, const Action &action) {
        #define __strcase(T) case Action::T: os << #T; break
        switch (action) {
            __strcase(NOTHING);
            __strcase(CONSUME);
            __strcase(IGNORE);
            __strcase(MULTILINE);
            __strcase(UNKNOWN);
        };
        return os;
        #undef __strcase
    }
}
