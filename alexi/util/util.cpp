#include "alexi/location.hpp"

namespace alexi {
    Line &Line::shift(LineNo ln, Offset pn) {
        id += ln;
        position = pn;
        return *this;
    }

    Marker Marker::shift(const MarkShifter &s) {
        if (s.line > 0)
            // Only update line position if line is updated too.
            shift_line(s.line, s.line_position == 0 ? position : s.line_position);
        // Always update marker position regardless.
        shift_position(s.position);
        return *this;
    }

    Marker Marker::shift_line(LineNo ln) {
        line.shift(ln, position);
        return *this;
    }

    Marker Marker::shift_line(LineNo ln, Offset pn) {
        line.shift(ln, pn);
        return *this;
    }

    Marker Marker::shift_position(Offset n) {
        position += n;
        return *this;
    }

    Mark Marker::to_mark(void) {
        return Mark::from(*this);
    }

    Marker Marker::operator>>(const MarkShifter& shifter) {
        return shift(shifter);
    }

    Marker::operator Mark() const {
        return Mark::from(*this);
    }
}
