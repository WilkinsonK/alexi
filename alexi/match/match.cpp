#include <algorithm>
#include <memory>

#include "alexi/matcher.hpp"

namespace alexi {
    Matcher::Matcher(const Kind &kind) :
        Matcher(std::make_shared<Kind>(kind))
        {}

    Matcher::Matcher(std::shared_ptr<Kind> kind) :
        pattern(std::regex(kind->pattern, multiline)),
        kind(kind)
        {}

    Matcher Matcher::from(const Kind &other) {
        return from(std::make_shared<Kind>(other));
    }

    Matcher Matcher::from(std::shared_ptr<Kind> other) {
        return Matcher(other);
    }

    Opt<Token> Matcher::match(const Str &view, const Mark mark) {
        return match(StrV(view), mark);
    }

    Opt<Token> Matcher::match(const StrV &view, const Mark mark) {
        auto found = Match<std::cmatch>(view, pattern);
        if (found.size() < 1 || !kind->predicate(found[0]))
            return std::nullopt;
        return Token { .kind = kind, .view = Str(found[0]), .mark = mark };
    }

    Matchers::Matchers(const Kinds &kinds) : inner{} {
        std::for_each(kinds.begin(), kinds.end(), [&](const auto &kind) {
            inner.push_back(std::make_shared<Matcher>(kind));
        });

        std::sort(begin(), end(), [](const auto &lhs, const auto &rhs) {
            return *lhs->kind < *rhs->kind;
        });
    }

    Matchers::Iter Matchers::begin(void) {
        return inner.begin();
    }

    Matchers::ConstIter Matchers::begin(void) const {
        return inner.begin();
    }

    Matchers::Iter Matchers::end(void) {
        return inner.end();
    }

    Matchers::ConstIter Matchers::end(void) const {
        return inner.end();
    }
}
