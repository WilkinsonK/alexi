export module alexi.token:kind;
import :action;
import alexi.util;
import std;

namespace alexi {
    // A definition of a token type. The
    // `alexi::token::Kind` allows alexi to dynamically
    // declare individual tokens that can be parsed by
    // lexical analysis.
    export struct Kind {
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
        const Ord order  = 1.0;

        bool operator==(const Kind &) const;
        bool operator<(const Kind &) const;
        bool operator>(const Kind &) const;
    };

    bool Kind::operator==(const Kind &other) const {
        return
            pattern == other.pattern &&
            name == other.name &&
            action == other.action &&
            order == other.order;
    }

    bool Kind::operator<(const Kind &other) const {
        return order < other.order;
    }

    bool Kind::operator>(const Kind &other) const {
        return order > other.order;
    }
}

export namespace std {
    using alexi::Kind;

    inline void hash_combine(std::size_t &seed, std::size_t &hx) {
        seed ^= hx + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    inline void hash_combine(std::size_t &seed, std::size_t *hx, std::size_t hx_n) {
        if (hx_n < 1)
            return;
        hash_combine(seed, *hx);
        return hash_combine(seed, ++hx, --hx_n);
    }

    template <>
    struct hash<Kind> {
        constexpr size_t operator()(const Kind&) const;
    };

    constexpr size_t hash<Kind>::operator()(const Kind& kind) const {
        size_t hx[3] = {
            hash<string_view>{}(kind.name),
            hash<int>{}(static_cast<int>(kind.action)),
            hash<size_t>{}(kind.order)
        };
        size_t seed = hash<string_view>{}(kind.pattern);
        hash_combine(seed, hx, 3);
        return seed;
    }

    constexpr ostream &operator<<(ostream &os, const Kind &kind) {
        #define __structfield(N) "."#N"=" << kind . N
        #define __structstrfield(N) "."#N"=\"" << kind . N << "\""
        return os << "Kind{" <<
            __structstrfield(pattern) << ", " <<
            __structstrfield(name) << ", " <<
            __structfield(action) << ", " <<
            __structfield(order) <<
        "}";
        #undef __structstrfield
        #undef __structfield
    }
}
