#include <memory>

#include "alexi/match.hpp"
#include "alexi/matcher.hpp"

using alexi::match::Match;

namespace alexi::matcher {
    Self::Matcher(const Kind &kind) :
        Matcher(std::make_shared<Kind>(kind))
        {}

    Self::Matcher(std::shared_ptr<Kind> kind) :
        pattern(std::regex(kind->pattern, multiline)),
        kind(kind)
        {}

    Self Matcher::from(const Kind &other) {
        return from(std::make_shared<Kind>(other));
    }

    Self Matcher::from(std::shared_ptr<Kind> other) {
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
}
