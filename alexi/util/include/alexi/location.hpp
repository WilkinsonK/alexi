#pragma once
#include <format>

#include "alexi/aliases.hpp"
#include "alexi/formatting.hpp"

namespace alexi {
    ALEXI_STRUCT(MarkShifter) {
        Offset position;
        LineNo line = 0;
        Offset line_position = 0;
    };

    ALEXI_STRUCT(Line) {
        LineNo id       = 1;
        Offset position = 0;

        Self &shift(Offset, Offset = 0);
    };

    template<typename T>
    concept Location = requires(T t) {
        { t.file }     -> std::same_as<Path&>;
        { t.line }     -> std::same_as<Line&>;
        { t.position } -> std::same_as<Offset&>;
    };

    const Path   FILE_DEFAULT     = "__main__";
    const Line   LINE_DEFAULT     = {};
    const Offset POSITION_DEFAULT = 1;

    #define MARK_ATTRS \
        MARK_DEF Path   file     = FILE_DEFAULT; \
        MARK_DEF Line   line     = LINE_DEFAULT; \
        MARK_DEF Offset position = POSITION_DEFAULT;

    ALEXI_STRUCT(Mark) {
        #define MARK_DEF const
        MARK_ATTRS
        #undef MARK_DEF

        template <Location L>
        static Self from(const L&);
    };

    template <Location L>
    Mark Mark::from(const L& other) {
        return {
            .file     = other.file,
            .line     = other.line,
            .position = other.position
        };
    }

    ALEXI_STRUCT(Marker) {
        #define MARK_DEF
        MARK_ATTRS
        #undef MARK_DEF

        Self shift(const MarkShifter&);
        Self shift_line(LineNo);
        Self shift_line(LineNo, Offset);
        Self shift_position(Offset);
        Mark to_mark(void);
        Self operator>>(const MarkShifter&);
        operator Mark() const;
    };

}

namespace std {
    using namespace alexi;
    ALEXI_FORMATTER(
        Mark,
        "[{}@{}:{}]",
        t.file.string(),
        t.line.id,
        t.position - t.line.position
    );
    ALEXI_FORMATTER(
        Marker,
        "[{}@{}:{}]",
        t.line.id,
        t.position - t.line.position
    );
}
