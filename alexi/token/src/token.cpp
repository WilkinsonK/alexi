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
        os << "<Token[" << token.kind->name << "]";

        auto file    = token.mark.file;
        auto lnumber = token.mark.line.id;
        auto loffset = token.mark.position - token.mark.line.position;
        os << " [" << file << "@" << lnumber << ":" << loffset << "]";

        if (!token.kind->natural) os << " {" << token.view << "}";

        return os << ">";
    }
}
