#pragma once

#include <format>

#define ALEXI_PARSE(N) \
template <typename ParseContext> \
constexpr auto parse(ParseContext &ctx) { \
    return common_parse<ParseContext>(ctx, N); \
}

#define ALEXI_FORMAT(T, F, ...) \
template <typename FormatContext> \
auto format(const T& t, FormatContext& ctx) const { \
    return format_to(ctx.out(), F, __VA_ARGS__); \
}

#define ALEXI_FORMATTER(T, F, ...) \
template <> struct formatter<T> { \
    ALEXI_PARSE(#T) \
    ALEXI_FORMAT(T, F, __VA_ARGS__) \
}

namespace alexi {
    template <typename ParseContext, typename N>
    constexpr auto common_parse(ParseContext &ctx, N name) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw std::format_error(std::format("Invalid format specifier for {}.", name));
        }
        return it;
    }
}
