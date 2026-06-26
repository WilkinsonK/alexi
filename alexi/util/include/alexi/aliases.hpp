#pragma once

#include <filesystem>
#include <functional>
#include <iostream>
#include <optional>
#include <string_view>
#include <vector>

#define ALEXI_STRUCT(NAME, ...) struct NAME : public AlexiObject<NAME __VA_OPT__(<__VA_ARGS__>)>

namespace alexi {
    using Len            = std::size_t;
    using LineNo         = std::size_t;
    using Ord            = double;
    using Offset         = std::size_t;
    using Str            = std::string;
    using StrV           = std::string_view;
    using Path           = std::filesystem::path;
    using TokenValidator = std::function<bool(const StrV)>;

    template <typename T>
    using Opt = std::optional<T>;

    template <typename T>
    using Vec = std::vector<T>;

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
