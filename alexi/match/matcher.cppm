module;
#include "util_common.hpp"

export module alexi.match:matcher;
import alexi.token;
import alexi.util;
import std;

namespace alexi {
    using namespace std::regex_constants;

    template <typename M>
    ALEXI_STRUCT(Match, M) {
        M                matches;
        match_flag_type  flags = match_continuous;

        Match(const StrV &, const std::regex &);
        Len length(Ord = 0) const;
        Len size(void) const;
        Str operator[](Ord);
    };

    template <typename M>
    Match<M>::Match(const StrV &view, const std::regex &pattern) {
        std::regex_search(view.data(), view.data() + view.size(), matches, pattern, flags);
    }

    template <typename M>
    Len Match<M>::length(Ord index) const {
        return matches.length(index);
    }

    template <typename M>
    Len Match<M>::size(void) const {
        return matches.size();
    }

    template <typename M>
    Str Match<M>::operator[](Ord index) {
        return matches[index];
    }

    export ALEXI_STRUCT(Matcher) {
        const std::regex            pattern;
        const std::shared_ptr<Kind> kind;

        static Self from(const Kind &other);
        static Self from(std::shared_ptr<Kind> other);
        Opt<Token> match(const Str &view);
        Opt<Token> match(const StrV &view);
    };

    inline Matcher Matcher::from(const Kind &other) {
        return from(std::make_shared<Kind>(other));
    }

    inline Matcher Matcher::from(std::shared_ptr<Kind> other) {
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
