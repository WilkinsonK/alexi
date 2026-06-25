#pragma once

#include <filesystem>
#include <iostream>
#include <optional>
#include <string_view>

#define ALEXI_STRUCT(NAME, ...) struct NAME : public AlexiObject<NAME __VA_OPT__(<__VA_ARGS__>)>

namespace alexi {
    using Len    = std::size_t;
    using LineNo = std::size_t;
    using Str    = std::string;
    using StrV   = std::string_view;
    using Ord    = double;
    using Offset = std::size_t;
    using Path   = std::filesystem::path;

    template <typename T>
    using Opt = std::optional<T>;

    template <typename T>
    std::ostream &operator<<(std::ostream &os, const Opt<T> opt) {
        if (opt.has_value()) return std::cout << "Some(" << opt.value() << ")";
        else return std::cout << "None";
    }

    template <typename S>
    struct AlexiObject {
        using Self = S;
    };
}
