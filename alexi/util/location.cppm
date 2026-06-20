export module alexi.util:location;
import :aliases;
import std;

namespace alexi {
    export struct Line {
        LineNo id       = 1;
        Offset position = 0;
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

    export struct Mark {
        #define MARK_DEF const
        MARK_ATTRS
        #undef MARK_DEF
    };

    export struct Marker {
        #define MARK_DEF
        MARK_ATTRS
        #undef MARK_DEF

        operator Mark() {
            return {
                .file     = file,
                .line     = line,
                .offset   = offset,
                .position = position
            };
        }
    };
}
