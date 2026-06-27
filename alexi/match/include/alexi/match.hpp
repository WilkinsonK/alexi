#pragma once

#include <regex>

#include "alexi/aliases.hpp"

namespace alexi::match {
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
}
