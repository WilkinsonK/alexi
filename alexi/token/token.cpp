#include <ranges>

#include "alexi/action.hpp"
#include "alexi/kind.hpp"
#include "alexi/kinds.hpp"
#include "alexi/token.hpp"

namespace alexi {
    Action operator&(const Action &lhs, const Action &rhs) {
        return static_cast<Action>(static_cast<Flag>(lhs) & static_cast<Flag>(rhs));
    }

    Action operator&=(const Action &lhs, const Action &rhs) {
        return lhs & rhs;
    }

    Action operator|(const Action &lhs, const Action &rhs) {
        return static_cast<Action>(static_cast<Flag>(lhs) | static_cast<Flag>(rhs));
    }

    Action operator|=(const Action &lhs, const Action &rhs) {
        return lhs | rhs;
    }

    std::ostream &operator<<(std::ostream &os, const Action &action) {
        #define __strcase(T) case Action::T: os << #T; break
        switch (action) {
            __strcase(NOTHING);
            __strcase(CONSUME);
            __strcase(IGNORE);
            __strcase(MULTILINE);
            __strcase(UEOF);
            __strcase(UNKNOWN);
        };
        return os;
        #undef __strcase
    }
}

namespace alexi {
    std::shared_ptr<Kind> Kind::to_shared(void) const {
        return std::make_shared<Kind>(*this);
    }

    bool Kind::operator==(const Self &other) const {
        return
            pattern == other.pattern &&
            name == other.name &&
            action == other.action &&
            order == other.order;
    }

    bool Kind::operator<(const Self &other) const {
        return order < other.order;
    }

    bool Kind::operator>(const Self &other) const {
        return order > other.order;
    }

    std::ostream &operator<<(std::ostream &os, const Kind &kind) {
        #define __structfield(N) "."#N"=" << kind . N
        #define __structstrfield(N) "."#N"=\"" << kind . N << "\""
        return os << "<Kind{" <<
            __structstrfield(pattern) << ", " <<
            __structstrfield(name) << ", " <<
            __structfield(action) << ", " <<
            __structfield(order) <<
        "}>";
        #undef __structstrfield
        #undef __structfield
    }
}

namespace std {
    void hash_combine(std::size_t &seed, std::size_t &hx) {
        seed ^= hx + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    void hash_combine(std::size_t &seed, std::size_t *hx, std::size_t hx_n) {
        if (hx_n < 1)
            return;
        hash_combine(seed, *hx);
        return hash_combine(seed, ++hx, --hx_n);
    }

    size_t hash<Kind>::operator()(const Kind &kind) const {
        size_t hx[3] = {
            hash<string>{}(kind.name),
            hash<int>{}(static_cast<int>(kind.action)),
            hash<size_t>{}(kind.order)
        };
        size_t seed = hash<string>{}(kind.pattern);
        hash_combine(seed, hx, 3);
        return seed;
    }
}

namespace alexi {
    bool Kinds::add(const Kind kind) {
        return inner.insert(std::make_shared<Kind>(kind)).second;
    }

    Kinds::iterator Kinds::begin(void) {
        return inner.begin();
    }

    Kinds::const_iterator Kinds::begin(void) const {
        return inner.begin();
    }

    const std::shared_ptr<Kind> Kinds::find(const Str name) const {
        auto response = inner | std::views::filter([name](const auto &k) { return k->name == name; });
        if (response.begin() != response.end())
            return *response.begin();
        return nullptr;
    }

    std::shared_ptr<Kind> Kinds::drop(const Str name) {
        return inner.extract(find(name)).value();
    }

    const std::shared_ptr<Kind> Kinds::operator[](const Str name) const {
        return find(name);
    }

    std::shared_ptr<Kind> Kinds::operator()(const Str name) {
        return drop(name);
    }

    bool Kinds::operator|=(const Kind kind) {
        return add(kind);
    }

    Kinds::iterator Kinds::end(void) {
        return inner.end();
    }

    Kinds::const_iterator Kinds::end(void) const {
        return inner.end();
    }
}

namespace alexi {
    Token Token::UEOF(const Mark &mark) {
        return {
            .kind = kinds::UEOF.to_shared(),
            .view = "",
            .mark = mark
        };
    }

    Action Token::get_action(void) const {
        return kind->action;
    }

    Len Token::size(void) const {
        return view.size();
    }

    bool Token::operator==(const Kind &other) const {
        return *kind == other;
    }

    bool Token::operator==(const char *other) const {
        return kind->name == other;
    }

    bool Token::operator==(const Str &other) const {
        return kind->name == other;
    }

    bool Token::operator==(const StrV &other) const {
        return kind->name == other;
    }

    std::ostream &operator<<(std::ostream &os, const Token &token) {
        os << "<Token[" << token.kind->name << "]";

        auto file    = token.mark.file;
        auto lnumber = token.mark.line.id;
        auto loffset = token.mark.position - token.mark.line.position;
        os << " [" << file << "@" << lnumber << ":" << loffset << "]";

        if (!token.kind->natural) os << " {" << token.view << "}";

        return os << ">";
    }
}

namespace alexi::kinds {
    Kind KEYWORD(const Vec<Str> keywords) {
        return {
            .pattern = pattern::IDENTIFIER,
            .name = "KEYWORD",
            .order = 0.7,
            .predicate = [keywords](auto view) {
                return std::find(
                    std::begin(keywords),
                    std::end(keywords),
                    view) != std::end(keywords);
            }
        };
    }
}
