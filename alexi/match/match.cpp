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
        if (found.size() < 1) return std::nullopt;
        return Token {
            .kind = kind,
            .view = Str(found[0]),
            .mark = mark
        };
    }

    Matchers::Matchers(const Kinds &kinds) : inner{} {
        for (auto const &k : kinds) inner.push_back(std::make_shared<Matcher>(k));
    }

    Matchers::iterator Matchers::begin(void) {
        return inner.begin();
    }

    Matchers::const_iterator Matchers::begin(void) const {
        return inner.begin();
    }

    Matchers::iterator Matchers::end(void) {
        return inner.end();
    }

    Matchers::const_iterator Matchers::end(void) const {
        return inner.end();
    }
}
