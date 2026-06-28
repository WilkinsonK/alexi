#pragma once

#include <concepts>
#include <exception>

#include "alexi/aliases.hpp"
#include "alexi/location.hpp"

namespace alexi {
    template <Location L = Mark>
    struct Exception : public std::exception {
        Str message;

        Exception(const L loc, const Str message) :
            message(std::format("{} {}", loc, message))
            {}

        const char *what(void) const noexcept override {
            return message.c_str();
        }
    };

    template <Location L, std::convertible_to<Str> R>
    struct Unreachable : public Exception<L> {
        Unreachable(const L loc, const R &reason) :
            Exception<L>(loc, std::format("unreachable branch: {}", reason))
            {}
    };

    template <Location L, std::convertible_to<Str> V>
    struct Unmatched : public Exception<L> {
        Unmatched(const L loc, const V view) :
            Exception<L>(loc, std::format("no token type matched \"{}\"", view))
            {}
    };

    template <Location L, std::convertible_to<Str> V>
    struct UnknownToken : public Exception<L> {
        UnknownToken(const L loc, const V view) :
            Exception<L>(loc, std::format("unknown token \"{}\"", view))
            {}
    };

    template <Location L>
    struct UnexpectedEOF : public Exception<L> {
        UnexpectedEOF(const L loc) :
            Exception<L>(loc, "unexpected EOF")
            {}
    };
}
