#include <cerrno>
#include <cstddef>
#include <cstring>
#include <format>

#include "alexi/engine.hpp"
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

    inline Str read(File &fd) {
        using iter = std::istreambuf_iterator<char>;
        return Str((iter(fd)), iter());
    }

    inline File open(const Path &path) {
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
}
