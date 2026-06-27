#include <algorithm>
#include <memory>

#include "alexi/matchers.hpp"

namespace alexi::matchers {
    Self::Matchers(const Kinds &kinds) : inner{} {
        std::for_each(kinds.begin(), kinds.end(), [&](const auto &kind) {
            inner.push_back(std::make_shared<Matcher>(kind));
        });

        std::sort(begin(), end(), [](const auto &lhs, const auto &rhs) {
            return *lhs->kind < *rhs->kind;
        });
    }

    Self::Iter Matchers::begin(void) {
        return inner.begin();
    }

    Self::ConstIter Matchers::begin(void) const {
        return inner.begin();
    }

    Self::Iter Matchers::end(void) {
        return inner.end();
    }

    Self::ConstIter Matchers::end(void) const {
        return inner.end();
    }
}
