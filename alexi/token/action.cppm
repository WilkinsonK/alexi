export module alexi.token:action;
import std;

namespace alexi {
    export enum class Action {
        CONSUME   = 1 << 0,
        IGNORE    = 1 << 1,
        MULTILINE = 1 << 2,
        UNKNOWN   = 1 << 3
    };
}

export namespace std {
    using alexi::Action;

    constexpr ostream &operator<<(ostream &os, const Action &action) {
        #define __strcase(T) case Action::T: os << #T; break
        switch (action) {
            __strcase(CONSUME);
            __strcase(IGNORE);
            __strcase(MULTILINE);
            __strcase(UNKNOWN);
        };
        return os;
        #undef __strcase
    }
}
