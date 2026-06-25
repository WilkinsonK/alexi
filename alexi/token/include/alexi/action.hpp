#pragma once

#include <iostream>

namespace alexi {
    using Flag = unsigned int;
    enum class Action : Flag {
        NOTHING   = 0,
        CONSUME   = 1 << 0,
        IGNORE    = 1 << 1,
        MULTILINE = 1 << 3,
        UNKNOWN   = 1 << sizeof(Flag)
    };

    Action operator&(const Action &, const Action &);
    Action operator&=(const Action &, const Action &rhs);
    Action operator|(const Action &, const Action &);
    Action operator|=(const Action &, const Action &);
    std::ostream &operator<<(std::ostream &, const Action &);
}
