module;
#include "util_common.hpp"

export module alexi.util:location;
import :aliases;
import std;

namespace alexi {
    export struct Line {
        LineNo id       = 1;
        Offset position = 0;
    };

    export template<typename T>
    concept Location = requires(T t) {
        { t.file }     -> std::same_as<Path&>;
        { t.line }     -> std::same_as<Line&>;
        { t.offset }   -> std::same_as<Offset&>;
        { t.position } -> std::same_as<Offset&>;
    };

    const Path   FILE_DEFAULT     = "__main__";
    const Line   LINE_DEFAULT     = {};
    const Offset OFFSET_DEFAULT   = 0;
    const Offset POSITION_DEFAULT = 0;

    #define MARK_ATTRS \
        MARK_DEF Path   file     = FILE_DEFAULT; \
        MARK_DEF Line   line     = LINE_DEFAULT; \
        MARK_DEF Offset offset   = OFFSET_DEFAULT; \
        MARK_DEF Offset position = POSITION_DEFAULT;

    export ALEXI_STRUCT(Mark) {
        #define MARK_DEF const
        MARK_ATTRS
        #undef MARK_DEF

        template <Location L>
        constexpr inline static Self from(const L& other) {
            return {
                .file     = other.file,
                .line     = other.line,
                .offset   = other.offset,
                .position = other.position
            };
        }
    };

    export ALEXI_STRUCT(Marker) {
        #define MARK_DEF
        MARK_ATTRS
        #undef MARK_DEF

        inline Mark to_mark(void) {
            return Mark::from(*this);
        }

        operator Mark() const {
            return Mark::from(*this);
        }
    };
}
