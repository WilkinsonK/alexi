#pragma once

#include <concepts>
#include <format>
#include <exception>

#include "alexi/aliases.hpp"
#include "alexi/location.hpp"

namespace alexi {
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
            AlexiException(std::format("{} {}", loc, message))
            {}
    };

    template <Location L, std::convertible_to<Str> R>
    struct Unreachable : public LexerException<L> {
        Unreachable(const L loc, const R &reason) :
            LexerException<L>(loc, std::format("unreachable branch: {}", reason))
            {}
    };

    template <Location L, std::convertible_to<Str> V>
    struct Unmatched : public LexerException<L> {
        Unmatched(const L loc, const V view) :
            LexerException<L>(loc, std::format("no token type matched \"{}\"", view))
            {}
    };

    template <Location L, std::convertible_to<Str> V>
    struct UnknownToken : public LexerException<L> {
        UnknownToken(const L loc, const V view) :
            LexerException<L>(loc, std::format("unknown token \"{}\"", view))
            {}
    };

    template <Location L>
    struct UnexpectedEOF : public LexerException<L> {
        UnexpectedEOF(const L loc) :
            LexerException<L>(loc, "unexpected EOF")
            {}
    };
}
