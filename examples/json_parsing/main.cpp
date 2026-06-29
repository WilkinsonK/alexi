#include <algorithm>
#include <initializer_list>
#include <map>
#include <ranges>
#include <stdexcept>
#include <string>
#include <variant>

#include "alexi.hpp"
#include "alexi/exceptions.hpp"
#include "alexi/kinds.hpp"

using namespace alexi;

#ifndef LANG_FILE
#define LANG_FILE "data.json"
#endif

const Kind JSON_STRING{
    .pattern = R"("([^"\\]|\\.)*")",
    .name    = "JSON_STRING",
    .action  = Action::CONSUME | Action::MULTILINE,
    .natural = false
};
const Vec<Kind> KINDS{
    kinds::COMMA,
    kinds::COLON,
    JSON_STRING,
    kinds::LITERALNUM,
    kinds::IDENTIFIER,
    kinds::BRACKETL,
    kinds::BRACKETR,
    kinds::BRACEL,
    kinds::BRACER
};

template <typename T, typename Var>
struct is_variant_member;

template <typename T, typename... Args>
struct is_variant_member<T, std::variant<Args...>>
    : std::disjunction<std::is_same<T, Args>...> {};

template <typename T, typename Var>
concept IsVariantOf = is_variant_member<T, Var>::value;

struct Node {
    using Array  = Vec<Node>;
    using Object = std::map<Str, Node>;
    using Null   = std::nullptr_t;
    using Inner  = std::variant<
        Array,
        Object,
        Null,
        Str,
        bool,
        double,
        std::monostate
    >;

    Inner inner;

    Node(void) : Node(std::monostate{}) {}
    template <IsVariantOf<Inner> T>
    Node(const T inner) : inner(inner) {}

    template <IsVariantOf<Inner> T>
    inline bool is(void) const {
        return std::holds_alternative<T>(inner);
    }

    template <IsVariantOf<Inner> T>
    inline operator T() const {
        return std::get<T>(inner);
    }

    template <IsVariantOf<Inner> T>
    bool operator==(const T&) const {
        return is<T>();
    }
};

struct Parser {
    private:
    Lexer lexer;

    public:
    Parser(const Lexer &);
    const Token eat(void);
    template <typename ...K>
    const Token expect(const K&...);
    const Token expect(const std::initializer_list<Kind>);
    template <typename T>
    const Node parse(const Token &);
    template <typename T>
    const Node parse(void);
    const Node parse(void);
};

Parser::Parser(const Lexer &lexer) : lexer(lexer) {}

const Token Parser::eat(void) {
    return lexer.next_token();
}

template <typename ...K>
const Token Parser::expect(const K & ...args) {
    return expect({args...});
}

template <>
const Node Parser::parse<Str>(const Token &t) {
    const auto view = t.view;
    return Str(view.begin()+1, view.end()-1);
}

template <>
const Node Parser::parse<bool>(const Token &t) {
    const auto view = t.view;
    return view == "true";
}

template <>
const Node Parser::parse<double>(const Token &t) {
    return std::stod(t.view);
}

template <>
const Node Parser::parse<Node::Array>(void) {
    Node::Array arr;
    do {
        auto val = parse();
        if (val.is<std::monostate>()) break;
        arr.push_back(val);
    } while(expect(kinds::COMMA, kinds::BRACKETR) != kinds::BRACKETR);
    return arr;
}

template <>
const Node Parser::parse<Node::Object>(void) {
    Node::Object obj;
    do {
        auto key = parse<Str>(expect(JSON_STRING));
        expect(kinds::COLON);
        auto val = parse();
        if (val.is<std::monostate>()) break;
        obj[key] = val;
    } while (expect(kinds::COMMA, kinds::BRACER) != kinds::BRACER);
    return obj;
}

const Node Parser::parse(void) {
    const auto t = eat();
    if (t == Vec<Kind>({kinds::UEOF, kinds::BRACER, kinds::BRACKETR}))
        return {};
    if (t == JSON_STRING)
        return parse<Str>(t);
    if (t == kinds::LITERALNUM)
        return parse<double>(t);
    if (t == kinds::BRACEL)
        return parse<Node::Object>();
    if (t == kinds::BRACKETL)
        return parse<Node::Array>();

    const auto v = t.view;
    if (t == kinds::IDENTIFIER && v == "null")
        return nullptr;
    if (t == kinds::IDENTIFIER && v == "true" || v == "false")
        return parse<bool>(t);

    throw UnknownToken(t.mark, t.view);
}

const Token Parser::expect(const std::initializer_list<Kind> kinds) {
    auto t = eat();
    if (t == kinds) { return t; }

    Vec<Str> names;
    std::for_each(kinds.begin(), kinds.end(), [&names](const auto &k) {
        names.push_back(k.name);
    });
    auto join = names | std::views::join_with(StrV(" or "));
    throw std::runtime_error(std::format("{} Expected {} but got {}", t.mark, Str(join.begin(), join.end()), t.kind->name));
}

struct Alexi {
    private:
    Engine engine;

    public:
    Alexi(void);
    Parser from_file(Path path);
};

Alexi::Alexi(void) : engine() {
    engine = engine.use_kinds(KINDS);
}

Parser Alexi::from_file(Path path) {
    return Parser(engine.from_file(path));
}

int main(int argc, const char *const *argv) {
    auto ast = Alexi().from_file(LANG_FILE).parse();
    return 0;
}
