export module alexi.token:token;
import :kind;
import std;

namespace alexi {
    struct Token {
        const Kind kind;
        const Str view;
    };
}
