#pragma once

#define ALEXI_STRUCT(NAME, ...) struct NAME : public AlexiObject<NAME __VA_OPT__(<__VA_ARGS__>)>

namespace alexi {
    template <typename S>
    struct AlexiObject {
        using Self = S;
    };
}
