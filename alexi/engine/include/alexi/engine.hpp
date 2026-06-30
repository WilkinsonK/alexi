#pragma once

#include <algorithm>
#include <initializer_list>

#include "alexi/aliases.hpp"
#include "alexi/kind.hpp"
#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"
#include <initializer_list>

namespace alexi::engine {
    struct Engine;
    using Self = Engine;
}

namespace alexi {
    using alexi::engine::Engine;
}

namespace alexi::engine {
    ALEXI_STRUCT(Engine) {
        Vec<Str> keywords = {};
        Kinds    kinds;

        Engine(void);

        // `Engine::from_data()` generates a `Lexer` from
        // anonymous data.
        Lexer from_data(const Str&);
        // `Engine::from_data()` generates a `Lexer` from
        // anonymous data.
        Lexer from_data(const Path&, const Str&);
        // `Engine::from_file()` generates a `Lexer` from
        // some file path or file descriptor. It will try to
        // read the file and generate the lexer from the
        // data contained at that file location.
        Lexer from_file(const Path&);
        // `Engine::from_file()` generates a `Lexer` from
        // some file path or file descriptor. It will try to
        // read the file and generate the lexer from the
        // data contained at that file location.
        Lexer from_file(const Path&, File);
        // `Engine::use_keywords()` adds one or many strings
        // as special identifiers. These identifiers will be
        // used during token matching to determine if some
        // unknown identifier is a keyword or not. This will
        // be transformed into a `Kind` definition and added
        // to the lexer matchers when a `Lexer` needs to be
        // generated.
        template <typename ...Arg>
        requires (sizeof...(Arg) > 0) && (!Ranged<Arg, Str> && ...)
        Self &use_keywords(Arg && ...kwds) {
            return use_keywords<std::initializer_list<Str>>({kwds...});
        }
        // `Engine::use_keywords()` adds one or many strings
        // as special identifiers. These identifiers will be
        // used during token matching to determine if some
        // unknown identifier is a keyword or not. This will
        // be transformed into a `Kind` definition and added
        // to the lexer matchers when a `Lexer` needs to be
        // generated.
        template <Ranged<Str> R>
        Self &use_keywords(const R &kwds) {
            keywords.insert(keywords.begin(), kwds.begin(), kwds.end());
            return *this;
        }
        // `Engine::use_kind()` adds one or many `Kind`s to
        // the engine's internal state. These `Kinds` will
        // be transformed into `Matcher`s once a `Lexer`
        // needs to be generated.
        Self &use_kinds(const Kind &kind) {
            kinds.add(kind);
            return *this;
        }
        // `Engine::use_kind()` adds one or many `Kind`s to
        // the engine's internal state. These `Kinds` will
        // be transformed into `Matcher`s once a `Lexer`
        // needs to be generated.
        template <typename ...Arg>
        requires (sizeof...(Arg) > 0) && (!Ranged<Arg, Kind> && ...)
        Self &use_kinds(Arg && ...kinds) {
            return use_kinds<std::initializer_list<Kind>>({kinds...});
        }
        // `Engine::use_kind()` adds one or many `Kind`s to
        // the engine's internal state. These `Kinds` will
        // be transformed into `Matcher`s once a `Lexer`
        // needs to be generated.
        template <Ranged<Kind> R>
        Self &use_kinds(const R &k) {
            std::for_each(k.begin(), k.end(), [&](const auto &kind) {
                kinds.add(kind);
            });
            return *this;
        }
    };
}
