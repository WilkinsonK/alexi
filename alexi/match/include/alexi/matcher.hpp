#pragma once

#include <regex>

#include "alexi/aliases.hpp"
#include "alexi/kind.hpp"
#include "alexi/token.hpp"

namespace alexi::matcher {
    struct Matcher;
    using Self = Matcher;
}

namespace alexi {
    using alexi::matcher::Matcher;
}

namespace alexi::matcher {
    using namespace std::regex_constants;

    ALEXI_STRUCT(Matcher) {
        public:
        const std::regex            pattern;
        const std::shared_ptr<Kind> kind;

        Matcher(const Kind &);
        Matcher(std::shared_ptr<Kind>);

        static Matcher from(const Kind &);
        static Matcher from(std::shared_ptr<Kind>);
        Opt<Token> match(const Str &, const Mark = {});
        Opt<Token> match(const StrV &, const Mark = {});
    };
}
