#pragma once

#include <functional>

#include "alexi/aliases.hpp"
#include "alexi/action.hpp"
#include "alexi/formatting.hpp"

namespace alexi::kind {
    struct Kind;
    using Self = Kind;
}

namespace alexi {
    using alexi::kind::Kind;
}

namespace alexi::kind {
    // A definition of a token type. The
    // `alexi::token::Kind` allows alexi to dynamically
    // declare individual tokens that can be parsed by
    // lexical analysis.
    ALEXI_STRUCT(Kind) {
        // The regex pattern used to determine if the next
        // available token matches this definition.
        const Str pattern;
        // A human-readable name for this kind.
        const Str name;
        // The action to take if the pattern matches a
        // token.
        const Action action = Action::CONSUME;
        // The order or precedence which determines how soon
        // or how late this kind should be matched against
        // other kind definitions.
        const Ord order  = .1;
        // Whether this is of a kind that can be naturally
        // defined. That is, the kind has a static pattern
        // which it belongs to.
        //
        // For example, '{' as a 'BRACE_LEFT' token kind
        // could be considered natural as the brace itself
        // has 0 variations.
        const bool natural = true;
        // A secondary validation which allows some `Kind`
        // to only be viable if a match also meets the
        // predicate.
        const TokenValidator predicate = [](auto _){ return true; };

        // Return a `shared_ptr` to the `Kind`.
        std::shared_ptr<Self> to_shared(void) const;
        Self validate(void) const;

        bool operator==(const Self &) const;
        bool operator<(const Self &) const;
        bool operator>(const Self &) const;
    };

    std::ostream &operator<<(std::ostream &, const Self &);
}

namespace std {
    using namespace alexi::kind;
    using namespace alexi::format;
    ALEXI_FORMATTER(
        Kind,
        "<Kind[pattern=\"{}\", name=\"{}\", action={}, order={}]>",
        t.pattern,
        t.name,
        t.action,
        t.order
    );
}

namespace std {
    using namespace alexi;

    void hash_combine(std::size_t &, std::size_t &);
    void hash_combine(std::size_t &, std::size_t *, std::size_t);

    template <>
    struct hash<Kind> {
        size_t operator()(const Kind&) const;
    };
}
