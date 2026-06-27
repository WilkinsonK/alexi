#pragma once

#include "alexi/aliases.hpp"
#include "alexi/kind.hpp"
#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"

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
        Self use_keywords(const Str&);
        // `Engine::use_keywords()` adds one or many strings
        // as special identifiers. These identifiers will be
        // used during token matching to determine if some
        // unknown identifier is a keyword or not. This will
        // be transformed into a `Kind` definition and added
        // to the lexer matchers when a `Lexer` needs to be
        // generated.
        Self use_keywords(const Vec<Str>&);
        // `Engine::use_keywords()` adds one or many strings
        // as special identifiers. These identifiers will be
        // used during token matching to determine if some
        // unknown identifier is a keyword or not. This will
        // be transformed into a `Kind` definition and added
        // to the lexer matchers when a `Lexer` needs to be
        // generated.
        Self use_keywords(const Vec<Str>::const_iterator&, const Vec<Str>::const_iterator&);
        // `Engine::use_kind()` adds one or many `Kind`s to
        // the engine's internal state. These `Kinds` will
        // be transformed into `Matcher`s once a `Lexer`
        // needs to be generated.
        Self use_kinds(const Kind&);
        // `Engine::use_kind()` adds one or many `Kind`s to
        // the engine's internal state. These `Kinds` will
        // be transformed into `Matcher`s once a `Lexer`
        // needs to be generated.
        Self use_kinds(const Vec<Kind>&);
        // `Engine::use_kind()` adds one or many `Kind`s to
        // the engine's internal state. These `Kinds` will
        // be transformed into `Matcher`s once a `Lexer`
        // needs to be generated.
        Self use_kinds(const Vec<Kind>::const_iterator&, const Vec<Kind>::const_iterator&);
    };
}
