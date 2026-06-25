#include <fstream>

#include "alexi/engine.hpp"

namespace alexi {
    inline constexpr Str read(std::fstream &fd) {
        using iter = std::istreambuf_iterator<char>;
        return Str((iter(fd)), iter());
    }

    inline constexpr Str read(const Path &path) {
        std::fstream fd(path);
        if (fd.fail())
            return "";
        return read(fd);
    }

    Engine Engine::from_data(const Str &data) {
        return {
            .data = data
        };
    }

    Engine Engine::from_file(const Path &file) {
        Engine me = {
            .data   = read(file),
            .marker = { .file = file },
        };
        return me;
    }

    Lexer Engine::generate(void) {
        return {
            .marker   = marker,
            .matchers = kinds,
            .data     = data
        };
    }

    Engine Engine::use_data(const Str &data) {
        this->data = data;
        return *this;
    }

    Engine Engine::use_kind(const Kind &kind) {
        kinds.add(kind);
        return *this;
    }
}
