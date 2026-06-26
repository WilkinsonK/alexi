#include <algorithm>
#include <cerrno>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <initializer_list>
#include <stdexcept>

#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"

namespace alexi {
    inline Matchers generate_matchers(Engine &engine) {
        Kinds matchers = engine.kinds;
        matchers.add(kinds::WHITESPACE);
        matchers.add(kinds::NEWLINE);
        matchers.add(kinds::UNKNOWN);
        matchers.add(kinds::KEYWORD(engine.keywords));
        return matchers;
    };

    inline constexpr Str read(std::fstream &fd) {
        using iter = std::istreambuf_iterator<char>;
        return Str((iter(fd)), iter());
    }

    inline constexpr Str read(const Path &path) {
        std::fstream fd(path);
        if (!fd)
            throw std::runtime_error(std::format(
                R"("{}": {})",
                path.c_str(),
                std::strerror(errno)
            ));
        return read(fd);
    }

    Engine::Engine(void) {}

    Lexer Engine::from_data(const Str &data) {
        return { .matchers = generate_matchers(*this), .data = data };
    }

    Lexer Engine::from_file(const Path &file) {
        Kinds matchers = kinds;
        matchers.add(kinds::KEYWORD(keywords));
        return {
            .marker   = { .file = file },
            .matchers = generate_matchers(*this),
            .data     = read(file),
        };
    }

    Engine Engine::use_keywords(const Str &kwd) {
        return use_keywords(Vec({kwd}));
    }

    Engine Engine::use_keywords(const Vec<Str> &kwds) {
        return use_keywords(kwds.begin(), kwds.end());;
    }

    Engine Engine::use_keywords(const Vec<Str>::const_iterator &first, const Vec<Str>::const_iterator &last) {
        keywords.insert(keywords.begin(), first, last);
        return *this;
    }

    Engine Engine::use_kinds(const Kind &kind) {
        return use_kinds(Vec({kind}));
    }

    Engine Engine::use_kinds(const Vec<Kind> &kinds) {
        return use_kinds(kinds.begin(), kinds.end());
    }

    Engine Engine::use_kinds(const Vec<Kind>::const_iterator &first, const Vec<Kind>::const_iterator &last) {
        std::for_each(first, last, [&](const auto &kind) {
            kinds.add(kind);
        });
        return *this;
    }
}
