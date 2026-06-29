#include <algorithm>
#include <cerrno>
#include <cstring>
#include <format>
#include <initializer_list>
#include <stdexcept>

#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"
#include "alexi/location.hpp"

namespace alexi::engine {
    inline Matchers generate_matchers(Engine &engine) {
        Kinds matchers = engine.kinds;
        matchers.add(kinds::WHITESPACE);
        matchers.add(kinds::NEWLINE);
        matchers.add(kinds::UNKNOWN);
        if (engine.keywords.size() > 0)
            matchers.add(kinds::KEYWORD(engine.keywords));
        return matchers;
    };

    inline constexpr Str read(File &fd) {
        using iter = std::istreambuf_iterator<char>;
        return Str((iter(fd)), iter());
    }

    inline constexpr File open(const Path &path) {
        File fd(path);
        if (fd) return fd;
        auto src = path.string();
        auto err = std::string(std::strerror(errno));
        throw std::runtime_error(std::format(R"("{}": {})", src, err));
    }

    Self::Engine(void) {}

    Lexer Self::from_data(const Str &data) {
        return from_data(FILE_DEFAULT, data);
    }

    Lexer Self::from_data(const Path&file, const Str &data) {
        return {
            .marker   = { .file = file },
            .matchers = generate_matchers(*this),
            .data     = data
        };
    }

    Lexer Self::from_file(const Path &file) {
        return from_file(file, open(file));
    }

    Lexer Self::from_file(const Path &file, File fd) {
        return {
            .marker   = { .file = file },
            .matchers = generate_matchers(*this),
            .data     = read(fd)
        };
    }

    Self &Self::use_keywords(const Str &kwd) {
        return use_keywords(Vec({kwd}));
    }

    Self &Self::use_keywords(const Vec<Str> &kwds) {
        return use_keywords(kwds.begin(), kwds.end());;
    }

    Self &Self::use_keywords(const Vec<Str>::const_iterator &first, const Vec<Str>::const_iterator &last) {
        keywords.insert(keywords.begin(), first, last);
        return *this;
    }

    Self &Self::use_kinds(const Kind &kind) {
        return use_kinds(Vec({kind}));
    }

    Self &Self::use_kinds(const Vec<Kind> &kinds) {
        return use_kinds(kinds.begin(), kinds.end());
    }

    Self &Self::use_kinds(const Vec<Kind>::const_iterator &first, const Vec<Kind>::const_iterator &last) {
        std::for_each(first, last, [&](const auto &kind) {
            kinds.add(kind);
        });
        return *this;
    }
}
