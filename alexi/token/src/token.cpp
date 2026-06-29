#include <algorithm>
#include <format>
#include <initializer_list>

#include "alexi/kinds.hpp"
#include "alexi/token.hpp"

namespace alexi::token {
    Self Self::UEOF(const Mark &mark) {
        return {
            .kind = kinds::UEOF.to_shared(),
            .view = "",
            .mark = mark
        };
    }

    Action Self::get_action(void) const {
        return kind->action;
    }

    Len Self::size(void) const {
        return view.size();
    }


    bool Self::operator==(const std::initializer_list<Kind> others) const {
        return *this == Vec<Kind>(others);
    }

    bool Self::operator==(const Vec<Kind> others) const {
        return std::find_if(others.begin(), others.end(), [&](const auto &k){
            return *this == k;
        }) != others.end();
    }

    bool Self::operator==(const Kind &other) const {
        return *kind == other;
    }

    bool Self::operator==(const char *other) const {
        return kind->name == other;
    }

    bool Self::operator==(const Str &other) const {
        return kind->name == other;
    }

    bool Self::operator==(const StrV &other) const {
        return kind->name == other;
    }

    std::ostream &operator<<(std::ostream &os, const Self &token) {
        return os << std::format("{}", token);
    }
}
