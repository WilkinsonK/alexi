#pragma once

#include "alexi/aliases.hpp"
#include "alexi/kind.hpp"
#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"

namespace alexi {
    ALEXI_STRUCT(Engine) {
        Vec<Str> keywords = {};
        Kinds    kinds;

        Engine(void);

        Lexer from_data(const Str&);
        Lexer from_file(const Path&);
        Self use_keywords(const Str&);
        Self use_keywords(const Vec<Str>&);
        Self use_keywords(const Vec<Str>::const_iterator&, const Vec<Str>::const_iterator&);
        Self use_kinds(const Kind&);
        Self use_kinds(const Vec<Kind>&);
        Self use_kinds(const Vec<Kind>::const_iterator&, const Vec<Kind>::const_iterator&);
    };
}
