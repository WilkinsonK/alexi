#include <catch2/catch_test_macros.hpp>
#include <sstream>

#include "alexi/location.hpp"

using namespace alexi;

// ============================================================================
// LINE STRUCT TESTS
// ============================================================================

TEST_CASE("Line initializes with default values", "[location][line]") {
    Line line;
    REQUIRE(line.id == 1);
    REQUIRE(line.position == 0);
}

TEST_CASE("Line can be constructed with custom values", "[location][line]") {
    Line line{.id = 5, .position = 42};
    REQUIRE(line.id == 5);
    REQUIRE(line.position == 42);
}

TEST_CASE("Line::shift increments id and updates position", "[location][line]") {
    Line line{.id = 1, .position = 0};
    line.shift(2, 10);  // Add 2 lines, start new line at position 10

    REQUIRE(line.id == 3);
    REQUIRE(line.position == 10);
}

TEST_CASE("Line::shift with multiple increments", "[location][line]") {
    Line line{.id = 1, .position = 0};
    line.shift(1, 5);

    REQUIRE(line.id == 2);
    REQUIRE(line.position == 5);

    // Shift again
    line.shift(3, 15);
    REQUIRE(line.id == 5);
    REQUIRE(line.position == 15);
}

TEST_CASE("Line::shift returns self for chaining", "[location][line]") {
    Line line{.id = 1, .position = 0};
    Line& result = line.shift(1, 10);

    REQUIRE(&result == &line);
    REQUIRE(line.id == 2);
    REQUIRE(line.position == 10);
}

TEST_CASE("Line::shift with zero line increment keeps position unchanged", "[location][line]") {
    Line line{.id = 5, .position = 20};
    line.shift(0, 30);

    REQUIRE(line.id == 5);
    REQUIRE(line.position == 30);
}

// ============================================================================
// MARK STRUCT TESTS
// ============================================================================

TEST_CASE("Mark initializes with default values", "[location][mark]") {
    Mark mark;
    REQUIRE(mark.file == FILE_DEFAULT);
    REQUIRE(mark.line.id == 1);
    REQUIRE(mark.position == 1);
}

TEST_CASE("Mark can be constructed with custom values", "[location][mark]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 5, .position = 0},
        .position = 42
    };

    REQUIRE(mark.file == "test.txt");
    REQUIRE(mark.line.id == 5);
    REQUIRE(mark.position == 42);
}

TEST_CASE("Mark::from creates Mark from another Mark", "[location][mark]") {
    Mark original{
        .file = "source.txt",
        .line = {.id = 3, .position = 10},
        .position = 25
    };

    Mark copy = Mark::from(original);

    REQUIRE(copy.file == original.file);
    REQUIRE(copy.line.id == original.line.id);
    REQUIRE(copy.line.position == original.line.position);
    REQUIRE(copy.position == original.position);
}

TEST_CASE("Mark::from with Marker (Location concept)", "[location][mark]") {
    Marker marker{
        .file = "test.txt",
        .line = {.id = 2, .position = 5},
        .position = 15
    };

    Mark mark = Mark::from(marker);

    REQUIRE(mark.file == marker.file);
    REQUIRE(mark.line.id == marker.line.id);
    REQUIRE(mark.position == marker.position);
}

TEST_CASE("Mark has correct default file", "[location][mark]") {
    Mark mark;
    REQUIRE(mark.file == "__main__");
}

TEST_CASE("Mark has correct default line ID", "[location][mark]") {
    Mark mark;
    REQUIRE(mark.line.id == 1);
}

TEST_CASE("Mark has correct default position", "[location][mark]") {
    Mark mark;
    REQUIRE(mark.position == 1);
}

// ============================================================================
// MARKER STRUCT TESTS
// ============================================================================

TEST_CASE("Marker initializes with default values", "[location][marker]") {
    Marker marker;
    REQUIRE(marker.file == FILE_DEFAULT);
    REQUIRE(marker.line.id == 1);
    REQUIRE(marker.position == 1);
}

TEST_CASE("Marker can be constructed with custom values", "[location][marker]") {
    Marker marker{
        .file = "test.txt",
        .line = {.id = 5, .position = 0},
        .position = 42
    };

    REQUIRE(marker.file == "test.txt");
    REQUIRE(marker.line.id == 5);
    REQUIRE(marker.position == 42);
}

// ============================================================================
// MARKER::SHIFT_POSITION TESTS
// ============================================================================

TEST_CASE("Marker::shift_position increments position", "[location][marker][shift]") {
    Marker marker{.position = 10};
    marker.shift_position(5);

    REQUIRE(marker.position == 15);
}

TEST_CASE("Marker::shift_position returns self for chaining", "[location][marker][shift]") {
    Marker marker{.position = 10};
    Marker result = marker.shift_position(5);

    REQUIRE(marker.position == 15);
}

TEST_CASE("Marker::shift_position with zero", "[location][marker][shift]") {
    Marker marker{.position = 10};
    marker.shift_position(0);

    REQUIRE(marker.position == 10);
}

TEST_CASE("Marker::shift_position with negative (edge case)", "[location][marker][shift]") {
    Marker marker{.position = 10};
    marker.shift_position(-3);

    REQUIRE(marker.position == 7);
}

// ============================================================================
// MARKER::SHIFT_LINE TESTS (Two Overloads)
// ============================================================================

TEST_CASE("Marker::shift_line(LineNo) increments line ID and resets position", "[location][marker][shift]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 15
    };

    marker.shift_line(2);

    REQUIRE(marker.line.id == 3);  // 1 + 2
    // Position is set to current marker.position (15) per util.cpp line 20
    REQUIRE(marker.line.position == 15);
}

TEST_CASE("Marker::shift_line(LineNo, Offset) increments line and sets position", "[location][marker][shift]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 20
    };

    marker.shift_line(1, 10);

    REQUIRE(marker.line.id == 2);
    REQUIRE(marker.line.position == 10);
}

TEST_CASE("Marker::shift_line returns self for chaining", "[location][marker][shift]") {
    Marker marker{.line = {.id = 1, .position = 0}};
    Marker result = marker.shift_line(1);

    REQUIRE(marker.line.id == 2);
}

TEST_CASE("Marker::shift_line with both overloads", "[location][marker][shift]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 10
    };

    // Single-arg: line position follows current position
    marker.shift_line(1);
    REQUIRE(marker.line.id == 2);
    REQUIRE(marker.line.position == 10);

    // Now position changes
    marker.shift_position(5);
    REQUIRE(marker.position == 15);

    // Two-arg: line position explicit
    marker.shift_line(1, 20);
    REQUIRE(marker.line.id == 3);
    REQUIRE(marker.line.position == 20);
    REQUIRE(marker.position == 15);  // Position unchanged by shift_line
}

// ============================================================================
// MARKER::SHIFT (COMBINED) TESTS
// ============================================================================

TEST_CASE("Marker::shift with only position increments position", "[location][marker][shift]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 10
    };

    marker.shift(MarkShifter{.position = 5, .line = 0, .line_position = 0});

    REQUIRE(marker.position == 15);
    REQUIRE(marker.line.id == 1);  // Line unchanged
    REQUIRE(marker.line.position == 0);
}

TEST_CASE("Marker::shift with line updates both line and position", "[location][marker][shift]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 20
    };

    // util.cpp line 13: if (s.line > 0) shift_line(s.line, ...)
    marker.shift(MarkShifter{.position = 5, .line = 2, .line_position = 10});

    REQUIRE(marker.line.id == 3);
    REQUIRE(marker.line.position == 10);
    REQUIRE(marker.position == 25);  // 20 + 5
}

TEST_CASE("Marker::shift without explicit line_position uses current position", "[location][marker][shift]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 15
    };

    // util.cpp line 13: line_position == 0 ? position : line_position
    marker.shift(MarkShifter{.position = 3, .line = 1, .line_position = 0});

    REQUIRE(marker.line.id == 2);
    REQUIRE(marker.line.position == 15);  // Current position, not reset
    REQUIRE(marker.position == 18);
}

TEST_CASE("Marker::shift with explicit line_position", "[location][marker][shift]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 20
    };

    marker.shift(MarkShifter{.position = 10, .line = 1, .line_position = 5});

    REQUIRE(marker.line.id == 2);
    REQUIRE(marker.line.position == 5);  // Explicit value
    REQUIRE(marker.position == 30);
}

TEST_CASE("Marker::shift returns self for chaining", "[location][marker][shift]") {
    Marker marker{.position = 10};
    Marker result = marker.shift(MarkShifter{.position = 5, .line = 0, .line_position = 0});

    REQUIRE(marker.position == 15);
}

// ============================================================================
// MARKER OPERATOR>> TESTS
// ============================================================================

TEST_CASE("Marker::operator>> (>>) delegates to shift", "[location][marker][operator]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 10
    };

    marker >> MarkShifter{.position = 5, .line = 0, .line_position = 0};

    REQUIRE(marker.position == 15);
}

TEST_CASE("Marker::operator>> shifts position", "[location][marker][operator]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 10
    };

    marker >> MarkShifter{.position = 5, .line = 0, .line_position = 0};

    REQUIRE(marker.position == 15);
}

TEST_CASE("Marker::operator>> with line increments", "[location][marker][operator]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 20
    };

    marker >> MarkShifter{.position = 0, .line = 1, .line_position = 0};

    REQUIRE(marker.line.id == 2);
    REQUIRE(marker.position == 20);
}

// ============================================================================
// MARKER::TO_MARK TESTS
// ============================================================================

TEST_CASE("Marker::to_mark creates immutable snapshot", "[location][marker][convert]") {
    Marker marker{
        .file = "test.txt",
        .line = {.id = 3, .position = 10},
        .position = 25
    };

    Mark mark = marker.to_mark();

    REQUIRE(mark.file == marker.file);
    REQUIRE(mark.line.id == marker.line.id);
    REQUIRE(mark.line.position == marker.line.position);
    REQUIRE(mark.position == marker.position);
}

TEST_CASE("Marker::to_mark snapshot persists after marker changes", "[location][marker][convert]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 10
    };

    Mark mark = marker.to_mark();
    REQUIRE(mark.position == 10);

    // Change marker
    marker.shift_position(5);

    // Mark unchanged
    REQUIRE(mark.position == 10);
    REQUIRE(marker.position == 15);
}

// ============================================================================
// MARKER CONVERSION OPERATOR (to Mark)
// ============================================================================

TEST_CASE("Marker::operator Mark() implicit conversion", "[location][marker][convert]") {
    Marker marker{
        .file = "test.txt",
        .line = {.id = 2, .position = 5},
        .position = 15
    };

    Mark mark = static_cast<Mark>(marker);

    REQUIRE(mark.file == marker.file);
    REQUIRE(mark.line.id == marker.line.id);
    REQUIRE(mark.position == marker.position);
}

TEST_CASE("Marker conversion creates independent snapshot", "[location][marker][convert]") {
    Marker marker{.position = 10};
    Mark mark = static_cast<Mark>(marker);

    marker.shift_position(5);

    REQUIRE(mark.position == 10);
    REQUIRE(marker.position == 15);
}

// ============================================================================
// MARK/MARKER FORMATTING
// ============================================================================

TEST_CASE("Mark formats with file, line, and column", "[location][format]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 5, .position = 10},
        .position = 15
    };

    std::ostringstream oss;
    oss << std::format("{}", mark);

    std::string output = oss.str();
    REQUIRE(!output.empty());
    REQUIRE(output.find("test.txt") != std::string::npos);
    REQUIRE(output.find("5") != std::string::npos);  // Line ID
}

TEST_CASE("Marker formats with file, line, and column", "[location][format]") {
    Marker marker{
        .file = "source.txt",
        .line = {.id = 3, .position = 20},
        .position = 25
    };

    std::ostringstream oss;
    oss << std::format("{}", marker);

    std::string output = oss.str();
    REQUIRE(!output.empty());
    REQUIRE(output.find("source.txt") != std::string::npos);
    REQUIRE(output.find("3") != std::string::npos);  // Line ID
}

// ============================================================================
// LOCATION CONCEPT TESTS
// ============================================================================

TEST_CASE("Mark satisfies Location concept", "[location][concept]") {
    // This compiles only if Mark satisfies Location concept
    Mark mark;

    // Should have these members
    REQUIRE(&mark.file != nullptr);
    REQUIRE(&mark.line != nullptr);
    REQUIRE(&mark.position != nullptr);
}

TEST_CASE("Marker satisfies Location concept", "[location][concept]") {
    // This compiles only if Marker satisfies Location concept
    Marker marker;

    // Should have these members
    REQUIRE(&marker.file != nullptr);
    REQUIRE(&marker.line != nullptr);
    REQUIRE(&marker.position != nullptr);
}

// ============================================================================
// INTEGRATION: REALISTIC SCENARIOS
// ============================================================================

TEST_CASE("Tracking position through single line tokens", "[location][scenario]") {
    Marker marker{
        .line = {.id = 1, .position = 1},
        .position = 1
    };

    // After first token "hello" (5 chars)
    marker.shift_position(5);
    REQUIRE(marker.position == 6);

    // After whitespace (1 char)
    marker.shift_position(1);
    REQUIRE(marker.position == 7);

    // After "world" (5 chars)
    marker.shift_position(5);
    REQUIRE(marker.position == 12);
}

TEST_CASE("Tracking position through multiline content", "[location][scenario]") {
    Marker marker{
        .line = {.id = 1, .position = 1},
        .position = 1
    };

    // After "hello" (5 chars) on line 1
    marker.shift_position(5);
    Mark mark1 = Mark::from(marker);
    REQUIRE(mark1.line.id == 1);
    REQUIRE(mark1.position == 6);

    // After newline (1 char, but increments line)
    marker.shift(MarkShifter{.position = 1, .line = 1, .line_position = 0 });
    Mark mark2 = Mark::from(marker);
    REQUIRE(mark2.line.id == 2);
    REQUIRE(mark2.position == 7);

    // After "world" (5 chars) on line 2
    marker.shift_position(5);
    Mark mark3 = Mark::from(marker);
    REQUIRE(mark3.line.id == 2);
    REQUIRE(mark3.position == 12);
}

TEST_CASE("Multiple newlines increment line counter correctly", "[location][scenario]") {
    Marker marker{
        .line = {.id = 1, .position = 0},
        .position = 1
    };

    // Process 3 newlines
    marker.shift(MarkShifter{ .position = 3, .line = 3, .line_position = 0 });

    REQUIRE(marker.line.id == 4);
    REQUIRE(marker.position == 4);
}

TEST_CASE("File tracking across lexing", "[location][scenario]") {
    Marker marker{
        .file = "program.txt",
        .line = {.id = 1, .position = 0},
        .position = 1
    };

    Mark initial = Mark::from(marker);
    REQUIRE(initial.file == "program.txt");

    // File doesn't change during lexing
    marker.shift_position(100);
    Mark final = Mark::from(marker);
    REQUIRE(final.file == "program.txt");
}
