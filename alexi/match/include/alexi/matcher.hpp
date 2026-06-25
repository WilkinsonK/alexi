#pragma once

#include <regex>

#include "alexi/aliases.hpp"
#include "alexi/kinds.hpp"
#include "alexi/token.hpp"

namespace alexi {
    using namespace std::regex_constants;

    template <typename M>
    ALEXI_STRUCT(Match, M) {
        M               matches;
        match_flag_type flags = match_continuous;

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

    ALEXI_STRUCT(Matcher) {
        public:
        const std::regex            pattern;
        const std::shared_ptr<Kind> kind;

        Matcher(const Kind &);
        Matcher(std::shared_ptr<Kind>);

        static Matcher from(const Kind &);
        static Matcher from(std::shared_ptr<Kind>);
        Opt<Token> match(const Str &, const Mark = {});
        Opt<Token> match(const StrV &, const Mark = {});
    };

    ALEXI_STRUCT(Matchers) {
        std::vector<std::shared_ptr<Matcher>> inner;

        Matchers(const Kinds &);

        using iterator = std::vector<std::shared_ptr<Matcher>>::iterator;
        using const_iterator = std::vector<std::shared_ptr<Matcher>>::const_iterator;

        iterator begin(void);
        const_iterator begin(void) const;
        iterator end(void);
        const_iterator end(void) const;
    };
}
