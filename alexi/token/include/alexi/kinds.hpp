#pragma once

#include <set>

#include "alexi/action.hpp"
#include "alexi/kind.hpp"

namespace alexi {
    struct Kinds {
        std::set<std::shared_ptr<Kind>> inner;

        // Adds an `alexi::token::Kind` into this container
        // if the new entry is unique.
        // returns a `boolean` if the add was successful.
        bool add(const Kind kind);
        // Has an `alexi::token::Kind` removed from this
        // container.
        std::shared_ptr<Kind> drop(const Str);
        // Gets a reference to an `alexi::token::Kind` from
        // this container.
        const std::shared_ptr<Kind> find(const Str) const;
        // Gets a reference to an `alexi::token::Kind` from
        // this container.
        const std::shared_ptr<Kind> operator[](const Str) const;
        // Has an `alexi::token::Kind` removed from this
        // container.
        std::shared_ptr<Kind> operator()(const Str);
        // Adds an `alexi::token::Kind` into this container
        // if the new entry is unique.
        // returns a `boolean` if the add was successful.
        bool operator|=(const Kind);

        using iterator = std::set<std::shared_ptr<Kind>>::iterator;
        using const_iterator = std::set<std::shared_ptr<Kind>>::const_iterator;

        iterator begin(void);
        const_iterator begin(void) const;
        iterator end(void);
        const_iterator end(void) const;
    };
}

#define KINDDEF(IDENT, ...) static const Kind IDENT = { \
    .pattern = pattern::IDENT, \
    .name = #IDENT \
    __VA_OPT__(,) __VA_ARGS__ \
}
#define PATDEF(IDENT, PATTERN) constexpr auto IDENT = PATTERN

#define KINDDECLARE(IDENT, PATTERN, ...) \
    namespace alexi::pattern { PATDEF(IDENT, PATTERN); } \
    namespace alexi::kinds { KINDDEF(IDENT __VA_OPT__(,) __VA_ARGS__); }

KINDDECLARE(ARROWL, R"(<-)")
KINDDECLARE(ARROWR, R"(->)")
KINDDECLARE(ASSIGN, R"(=)")
KINDDECLARE(AT, R"(@)")
KINDDECLARE(ATAT, R"(@@)")
KINDDECLARE(BINAND, R"(&)")
KINDDECLARE(BINOR, R"(\|)")
KINDDECLARE(BINSHIFTL, R"(<<)")
KINDDECLARE(BINSHIFTR, R"(>>)")
KINDDECLARE(BINXOR, R"(\^)")
KINDDECLARE(BRACEL, R"(\{)")
KINDDECLARE(BRACER, R"(\})")
KINDDECLARE(BRACKETL, R"(\[)")
KINDDECLARE(BRACKETR, R"(\])")
KINDDECLARE(COLON, R"(:)")
KINDDECLARE(COLONCOLON, R"(::)")
KINDDECLARE(COLONEQ, R"(:=)")
KINDDECLARE(COMMA, R"(,)")
KINDDECLARE(COMMENT, R"(#.*)")
KINDDECLARE(DOLLAR, R"(\$)")
KINDDECLARE(DOT, R"(\.)")
KINDDECLARE(DOTDOT, R"(\.\.)")
KINDDECLARE(DOTEQ, R"(\.=)")
KINDDECLARE(UEOF, R"()", .action = Action::UEOF)
KINDDECLARE(IDENTIFIER, R"(^[A-Za-z_]\w*)", .order = .8, .natural = false)
KINDDECLARE(LITERALNUM, R"((0[xbo]\d+)|(\.\d+f?)|(\d+\.\d+f?)|(\d+f?))", .natural = false)
KINDDECLARE(LITERALSTR, R"("([^"\\]|\\.)*"|'([^'\\]|\\.)*'|""".*""")", .natural = false)
KINDDECLARE(LOGAND, R"(&&)")
KINDDECLARE(LOGEQ, R"(==)")
KINDDECLARE(LOGGT, R"(>)")
KINDDECLARE(LOGGTE, R"(>=)")
KINDDECLARE(LOGLT, R"(<)")
KINDDECLARE(LOGLTE, R"(<=)")
KINDDECLARE(LOGNOT, R"(!)")
KINDDECLARE(LOGOR, R"(\|\|)")
KINDDECLARE(LOGXOR, R"(\^\^)")
KINDDECLARE(MATDEC, R"(--)")
KINDDECLARE(MATDIV, R"(/)")
KINDDECLARE(MATDIVEQ, R"(/=)")
KINDDECLARE(MATFLOOR, R"(//)")
KINDDECLARE(MATFLOOREQ, R"(//=)")
KINDDECLARE(MATINC, R"(\+\+)")
KINDDECLARE(MATMINUS, R"(-)")
KINDDECLARE(MATMINUSEQ, R"(-=)")
KINDDECLARE(MATMOD, R"(\%)")
KINDDECLARE(MATMODEQ, R"(\%=)")
KINDDECLARE(MATMUL, R"(\*)")
KINDDECLARE(MATMULEQ, R"(\*=)")
KINDDECLARE(MATPLUS, R"(\+)")
KINDDECLARE(MATPLUSEQ, R"(\+=)")
KINDDECLARE(MATPOW, R"(\*\*)")
KINDDECLARE(MATPOWEQ, R"(\*\*=)")
KINDDECLARE(NEWLINE, R"(\r\n|\r|\n)", .action = Action::IGNORE | Action::MULTILINE, .order = .0)
KINDDECLARE(PARENL, R"(\()")
KINDDECLARE(PARENR, R"(\))")
KINDDECLARE(SEMICOLON, R"(;)")
KINDDECLARE(SLASH, R"(\\)")
KINDDECLARE(UNKNOWN, R"(\S*)", .action = Action::UNKNOWN, .order = .9)
KINDDECLARE(WHITESPACE, R"([ \t\v]+)", .action = Action::IGNORE, .order = .0)

namespace alexi::kinds {
    Kind KEYWORD(const Str[]);
    Kind KEYWORD(const Vec<Str>);
}

#undef KINDDECLARE
#undef KINDDEF
#undef PATDEF
