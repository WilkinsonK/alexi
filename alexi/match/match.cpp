#include "alexi/matcher.hpp"

namespace alexi {
    Matcher Matcher::from(const Kind &other) {
        return from(std::make_shared<Kind>(other));
    }

    Matcher Matcher::from(std::shared_ptr<Kind> other) {
        return {
            .pattern = std::regex(other->pattern, multiline),
            .kind    = other
        };
    }

    Opt<Token> Matcher::match(const Str &view) {
        return match(StrV(view));
    }

    Opt<Token> Matcher::match(const StrV &view) {
        auto found = Match<std::cmatch>(view, pattern);
        if (found.size() < 1) return std::nullopt;
        return Token {
            .kind = kind,
            .view = Str(found[0])
        };
    }
}

