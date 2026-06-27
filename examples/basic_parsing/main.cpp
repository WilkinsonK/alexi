#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <variant>

#include "alexi/engine.hpp"
#include "alexi/kind.hpp"
#include "alexi/kinds.hpp"
#include "alexi/token.hpp"

using namespace std::literals;
using namespace alexi;

#ifndef LANG_FILE
#define LANG_FILE "lang.alexi"
#endif

const Vec<Str> KEYWORDS{"kwd", "use"};
const Vec<Kind> KINDS{
    kinds::LITERALSTR,
    kinds::SEMICOLON,
    kinds::IDENTIFIER
};

struct KwdStatement {
    Token keyword;
};

struct UseStatement {
    Token kind;
};

struct Statement {
    using Inner = std::variant<
        KwdStatement,
        UseStatement,
        Vec<Statement>,
        std::monostate
    >;

    const Inner inner;

    #define __declconst(T) Statement(const T &)
    __declconst(KwdStatement);
    __declconst(UseStatement);
    __declconst(Inner);
    #undef __declconst
    Statement(void);
    Statement(const Statement &);

    template <typename T>
    inline bool is(void) const {
        return std::holds_alternative<T>(inner);
    }

    template <typename T>
    inline operator T() const {
        return std::get<T>(inner);
    }

    template <typename T>
    bool operator==(const T&) const {
        return is<T>();
    }
};

#define __declconst(T) Statement::Statement(const T &stmt) : inner(stmt)
__declconst(KwdStatement) {}
__declconst(UseStatement) {}
__declconst(Statement::Inner) {}
#undef __declconst

Statement::Statement(void) : Statement(std::monostate{}) {}
Statement::Statement(const Statement &other) : Statement(other.inner) {}

std::ostream &operator<<(std::ostream &os, const Statement &);
std::ostream &operator<<(std::ostream &os, const Statement::Inner &);
std::ostream &operator<<(std::ostream &os, const Vec<Statement> &);
std::ostream &operator<<(std::ostream &os, const KwdStatement &);
std::ostream &operator<<(std::ostream &os, const UseStatement &);

std::ostream &operator<<(std::ostream &os, const Statement &stmt) {
    return os << stmt.inner;
}

std::ostream &operator<<(std::ostream &os, const Statement::Inner &stmt) {
    std::visit([&os](auto &&value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::same_as<KwdStatement, T>)
            os << value;
        else if constexpr (std::same_as<UseStatement, T>)
            os << value;
        else
            throw std::runtime_error("unsupported variant");
    }, stmt);
    #undef __is_type
    return os;
}

std::ostream &operator<<(std::ostream &os, const Vec<Statement> &stmt) {
    for (auto it = stmt.begin(); it != stmt.end(); it++) {
        os << *stmt.begin();
        if (it+1 != stmt.end()) os << ", ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const KwdStatement &stmt) {
    return os << "KwdStatement(" << stmt.keyword << ")";
}

std::ostream &operator<<(std::ostream &os, const UseStatement &stmt) {
    return os << "UseStatement(" << stmt.kind << ")";
}

struct AlexiParser {
    private:
    Lexer lexer;

    const Token eat(void);
    const Token expect(const Kind&);
    const Token expect(const Str&);
    const Token expect(const Vec<Kind>&);
    const Token expect(const Vec<Str>&);

    public:
    AlexiParser(const Lexer &lexer);
    const Statement parse(void);
    template <typename T>
    const Statement parse(void);
};

AlexiParser::AlexiParser(const Lexer &lexer) : lexer(lexer) {}

template <>
const Statement AlexiParser::parse<KwdStatement>(void) {
    auto t = expect(kinds::LITERALSTR);
    expect(kinds::SEMICOLON);
    return KwdStatement{ .keyword = t };
}

template <>
const Statement AlexiParser::parse<UseStatement>(void) {
    auto t = expect(kinds::IDENTIFIER);
    expect(kinds::SEMICOLON);
    return UseStatement { .kind = t };
}

template <>
const Statement AlexiParser::parse<Vec<Statement>>(void) {
    Vec<Statement> root;
    while (1) {
        Statement stmt = parse();
        if (stmt == std::monostate{}) break;
        root.push_back(stmt);
    }
    return (const Statement)root;
}

const Statement AlexiParser::parse(void) {
    const auto t = eat();
    if (t == kinds::UEOF) return {};
    // TODO: How do I want to determine what I'm parsing?
    if (t == "KEYWORD" && t.view == "kwd") return parse<KwdStatement>();
    if (t == "KEYWORD" && t.view == "use") return parse<UseStatement>();
    throw std::runtime_error(std::format("Unexpected identifier '{}'", t.view));
}

const Token AlexiParser::eat(void) {
    return lexer.next_token();
}

const Token AlexiParser::expect(const Kind &expecting) {
    return expect(expecting.name);
}

const Token AlexiParser::expect(const Str &expecting) {
    auto t = eat();
    if (t != expecting)
        throw std::runtime_error(std::format("Expected {} but got {}", expecting, t.kind->name));
    return t;
}

const Token AlexiParser::expect(const Vec<Kind> &expecting) {
    Vec<Str> expecting_names;
    std::for_each(expecting.begin(), expecting.end(), [&expecting_names](const auto &k) {
        expecting_names.push_back(k.name);
    });
    return expect(expecting_names);
}

const Token AlexiParser::expect(const Vec<Str> &expecting) {
    auto t = eat();
    if (std::find(expecting.begin(), expecting.end(), t.kind->name) == expecting.end()) {
        auto joinit = expecting | std::views::join_with(" or "sv);
        Str expected{joinit.begin(), joinit.end()};
        throw std::runtime_error(std::format("Expected {} but got {}", expected, t.kind->name));
    }
    return t;
}

struct Alexi {
    Engine engine;

    Alexi(void) : engine() {
        engine = engine.use_keywords(KEYWORDS).use_kinds(KINDS);
    }

    AlexiParser from_file(Path path) {
        return AlexiParser(engine.from_file(path));
    }
};

int main(void) {
    auto ast = (Vec<Statement>)Alexi().from_file(LANG_FILE).parse<Vec<Statement>>();
    for (const auto &stmt : ast)
        std::cout << stmt << std::endl;
    return 0;
}
