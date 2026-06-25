#include <iterator>
#include <memory>
#include <regex>
#include <stdexcept>
#include <sstream>

#include "alexi/lexer.hpp"
#include "alexi/location.hpp"

namespace alexi {
    Token Lexer::eof(void) {
        Kind e{
            .pattern = R"()",
            .name    = "EOF",
            .action  = Action::UEOF,
            .natural = true,
        };
        return {
            .kind = std::make_shared<Kind>(e),
            .view = "",
            .mark = marker
        };
    }

    inline constexpr LineNo line_count(const Str &view) {
        const std::regex NEWLINE_REGEX(R"(\r\n|\r|\n)");
        auto begin = std::sregex_iterator(view.begin(), view.end(), NEWLINE_REGEX);
        auto end   = std::sregex_iterator();
        return std::distance(begin, end);
    }

    Token Lexer::handle_next_token(Token &&token) {
        const auto action = token.kind->action;

        if (action == Action::CONSUME) {
            marker >> MarkShifter{ .position = token.size() };
            return token;
        }

        if (action == (Action::CONSUME | Action::MULTILINE)) {
            marker >> MarkShifter{
                .position = token.size(),
                .line     = line_count(token.view)
            };
            return token;
        }

        if (action == Action::UEOF) {
            throw std::runtime_error("Unexpected EOF");
        }

        if (action == Action::IGNORE) {
            marker >> MarkShifter{ .position = token.size() };
            return next_token();
        }

        if (action == (Action::IGNORE | Action::MULTILINE)) {
            marker >> MarkShifter{
                .position = token.size(),
                .line     = line_count(token.view)
            };
            return next_token();
        }

        if (action == Action::MULTILINE) {
            marker >> MarkShifter{
                .position = token.size(),
                .line     = line_count(token.view)
            };
            return next_token();
        }

        std::stringstream ss;
        ss << "Unknown token " << token;
        throw std::runtime_error(ss.str());

        return std::move(token);
    }

    Token Lexer::match_next_token(void) {
        // We have to sub the marker position as it starts
        // at 1 when the marker is initialized, by default.
        // This helps us avoid an off-by-one error when
        // making cursor comparisons with the buffer and
        // error messages.
        StrV view(data.begin() + marker.position-1, data.end());
        for (auto const &m : matchers) {
            auto t = m->match(view, marker);
            if (!t.has_value()) continue;
            return t.value();
        }
        throw std::runtime_error("Hit unreachable branch: no token matched");
    }

    Token Lexer::next_token(void) {
        if (marker.position >= data.size())
            return eof();
        auto token = match_next_token();
        return handle_next_token(std::move(token));
    }
}
