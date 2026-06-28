#include <ranges>

#include "alexi/kinds.hpp"

namespace alexi::kinds {
    bool Self::add(const Kind kind) {
        return inner.insert(kind.validate().to_shared()).second;
    }

    Self::Iter Self::begin(void) {
        return inner.begin();
    }

    Self::ConstIter Self::begin(void) const {
        return inner.begin();
    }

    const Self::Item Self::find(const Str name) const {
        auto response = inner | std::views::filter([name](const auto &k) { return k->name == name; });
        if (response.begin() != response.end())
            return *response.begin();
        return nullptr;
    }

    Self::Item Self::drop(const Str name) {
        return inner.extract(find(name)).value();
    }

    const Self::Item Self::operator[](const Str name) const {
        return find(name);
    }

    Self::Item Self::operator()(const Str name) {
        return drop(name);
    }

    bool Self::operator|=(const Kind kind) {
        return add(kind);
    }

    Self::Iter Self::end(void) {
        return inner.end();
    }

    Self::ConstIter Self::end(void) const {
        return inner.end();
    }
}

namespace alexi::kinds {
    Kind KEYWORD(const Vec<Str> keywords) {
        return {
            .pattern = pattern::IDENTIFIER,
            .name = "KEYWORD",
            .order = 0.7,
            .natural = false,
            .predicate = [keywords](auto view) {
                return std::find(
                    std::begin(keywords),
                    std::end(keywords),
                    view) != std::end(keywords);
            }
        };
    }
}
