#pragma once

#include "alexi/aliases.hpp"
#include "alexi/kinds.hpp"
#include "alexi/matcher.hpp"

namespace alexi::matchers {
    struct Matchers;
    using Self = Matchers;
}

namespace alexi {
    using alexi::matchers::Matchers;
}

namespace alexi::matchers {
    using alexi::matcher::Matcher;

    ALEXI_STRUCT(Matchers) {
        using Inner     = Vec<std::shared_ptr<Matcher>>;
        using Iter      = Inner::iterator;
        using ConstIter = Inner::const_iterator;

        Inner inner;

        Matchers(const Kinds &);
        Iter begin(void);
        ConstIter begin(void) const;
        Iter end(void);
        ConstIter end(void) const;
    };
}
