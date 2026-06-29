#pragma once

#include <concepts>
#include <format>
#include <exception>
#include <ranges>

#include "alexi/aliases.hpp"
#include "alexi/location.hpp"

namespace alexi::exceptions {
    template <std::ranges::range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, Str>
    inline constexpr auto join_with(const R &items, const StrV delim) {
        auto join = items | std::views::join_with(delim);
        return Str(join.begin(), join.end());
    }

    struct AlexiException : public std::exception {
        Str message;

        AlexiException(void) :
            AlexiException("unspecified exception")
            {}
        template <typename ...Arg>
        AlexiException(std::format_string<Arg...> message, Arg&& ...args) :
            message(std::format(message, std::forward<Arg>(args)...))
            {}
        template <std::convertible_to<Str> R>
        AlexiException(R message) :
            message(message)
            {}

        const char *what(void) const noexcept override {
            return message.c_str();
        }
    };

    struct InvalidKind : public AlexiException {
        using AlexiException::AlexiException;
    };

    template <Location L = Mark>
    struct LexerException : public AlexiException {
        LexerException(const L loc, const Str message) :
            AlexiException("{} {}", loc, message)
            {}
        template <typename ...Arg>
        LexerException(const L loc, std::format_string<Arg...> message, Arg&& ...args) :
            AlexiException("{} {}", loc, std::format(message, std::forward<Arg>(args)...))
            {}
    };

    template <Location L, std::convertible_to<Str> V>
    struct Unmatched : public LexerException<L> {
        Unmatched(const L loc, const V view) :
            LexerException<L>(loc, "no token type matched \"{}\"", view)
            {}
    };

    template <Location L, std::convertible_to<Str> R>
    struct Unreachable : public LexerException<L> {
        Unreachable(const L loc, const R &reason) :
            LexerException<L>(loc, reason)
            {}
    };

    template <Location L, std::convertible_to<Str> G, std::ranges::range E>
    requires std::convertible_to<std::ranges::range_value_t<E>, Str>
    struct UnexpectedKind : public LexerException<L> {
        UnexpectedKind(const L loc, const G got, const E &expected) :
            LexerException<L>(loc, "expected {} but got {}", join_with(expected, " or "), got)
            {}
    };

    template <Location L>
    struct UnexpectedEOF : public LexerException<L> {
        UnexpectedEOF(const L loc) :
            LexerException<L>(loc, "")
            {}
    };

    template <Location L, std::convertible_to<Str> V>
    struct UnknownToken : public LexerException<L> {
        UnknownToken(const L loc, const V view) :
            LexerException<L>(loc, "\"{}\"", view)
            {}
    };
}

namespace alexi {
    using alexi::exceptions::AlexiException;
    using alexi::exceptions::InvalidKind;
    using alexi::exceptions::LexerException;
    using alexi::exceptions::Unreachable;
    using alexi::exceptions::Unmatched;
    using alexi::exceptions::UnexpectedKind;
    using alexi::exceptions::UnexpectedEOF;
    using alexi::exceptions::UnknownToken;
}
