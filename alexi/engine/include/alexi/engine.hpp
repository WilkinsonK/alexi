#pragma once

#include "alexi/aliases.hpp"
#include "alexi/kind.hpp"
#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"

namespace alexi {
    ALEXI_STRUCT(Engine) {
        Str    data;
        Marker marker;
        Kinds  kinds;

        static Self from_data(const Str&);
        static Self from_file(const Path&);

        Lexer generate(void);
        Self use_data(const Str&);
        Self use_kind(const Kind&);
    };
}
