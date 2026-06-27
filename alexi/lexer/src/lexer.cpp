#include <format>
#include <iterator>
#include <memory>
#include <regex>
#include <stdexcept>
#include <sstream>

#include "alexi/lexer.hpp"
#include "alexi/location.hpp"

namespace alexi::lexer {
    inline constexpr LineNo line_count(const Str &view) {
        const std::regex NEWLINE_REGEX(R"(\r\n|\r|\n)");
        auto begin = std::sregex_iterator(view.begin(), view.end(), NEWLINE_REGEX);
        auto end   = std::sregex_iterator();
        return std::distance(begin, end);
    }

    inline constexpr bool is_action(const Action action, const Action expect) {
        return (action & expect) == expect;
    }

    Token Self::handle_next_token(Token &&token) {
        const auto action = token.kind->action;

        if (is_action(action, Action::MULTILINE))
            marker >> MarkShifter{ .line = line_count(token.view) };
        if (is_action(action, Action::CONSUME)) {
            marker >> MarkShifter{ .position = token.size() };
            return token;
        }
        if (is_action(action, Action::IGNORE)) {
            marker >> MarkShifter{ .position = token.size() };
            return next_token();
        }
        if (is_action(action, Action::UEOF))
            throw std::runtime_error("Unexpected EOF");

        std::stringstream ss;
        ss << "Unknown token " << token;
        throw std::runtime_error(ss.str());
    }

    Token Self::match_next_token(void) {
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
        throw std::runtime_error(std::format("Hit unreachable branch: no token matched '{}'", view));
    }

    Token Self::next_token(void) {
        if (marker.position-1 >= data.size())
            return Token::UEOF(marker);
        auto token = match_next_token();
        return handle_next_token(std::move(token));
    }
}
