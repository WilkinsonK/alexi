#include "alexi/kind.hpp"

namespace alexi::kind {
    std::shared_ptr<Self> Self::to_shared(void) const {
        return std::make_shared<Self>(*this);
    }

    bool Self::operator==(const Self &other) const {
        return
            pattern == other.pattern &&
            name == other.name &&
            action == other.action &&
            order == other.order;
    }

    bool Self::operator<(const Self &other) const {
        return order < other.order;
    }

    bool Self::operator>(const Self &other) const {
        return order > other.order;
    }

    std::ostream &operator<<(std::ostream &os, const Self &kind) {
        #define __structfield(N) "."#N"=" << kind . N
        #define __structstrfield(N) "."#N"=\"" << kind . N << "\""
        return os << "<Kind{" <<
            __structstrfield(pattern) << ", " <<
            __structstrfield(name) << ", " <<
            __structfield(action) << ", " <<
            __structfield(order) <<
        "}>";
        #undef __structstrfield
        #undef __structfield
    }
}

namespace std {
    void hash_combine(std::size_t &seed, std::size_t &hx) {
        seed ^= hx + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    void hash_combine(std::size_t &seed, std::size_t *hx, std::size_t hx_n) {
        if (hx_n < 1)
            return;
        hash_combine(seed, *hx);
        return hash_combine(seed, ++hx, --hx_n);
    }

    size_t hash<Kind>::operator()(const Kind &kind) const {
        size_t hx[3] = {
            hash<string>{}(kind.name),
            hash<int>{}(static_cast<int>(kind.action)),
            hash<size_t>{}(kind.order)
        };
        size_t seed = hash<string>{}(kind.pattern);
        hash_combine(seed, hx, 3);
        return seed;
    }
}
