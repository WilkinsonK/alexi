#include <catch2/catch_test_macros.hpp>

#include "alexi/exceptions.hpp"
#include "alexi/location.hpp"

using namespace alexi;

// ============================================================================
// EXCEPTION TEMPLATE TESTS
// ============================================================================

TEST_CASE("Exception with Mark location is constructible", "[exception]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 5, .position = 10},
        .position = 20
    };

    LexerException<Mark> ex(mark, "Test error message");
    REQUIRE(ex.what() != nullptr);
}

TEST_CASE("Exception message includes location info", "[exception]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 5, .position = 10},
        .position = 20
    };

    LexerException<Mark> ex(mark, "Test error message");
    std::string message = ex.what();

    REQUIRE(message.find("test.txt") != std::string::npos);
    REQUIRE(message.find("5") != std::string::npos);  // Line ID
    REQUIRE(message.find("Test error message") != std::string::npos);
    // Format: "[file@line:col] message"
    REQUIRE(message.find("[") != std::string::npos);
    REQUIRE(message.find("@") != std::string::npos);
    REQUIRE(message.find(":") != std::string::npos);
}

TEST_CASE("Exception message format includes Mark format", "[exception]") {
    Mark mark{
        .file = "source.txt",
        .line = {.id = 10, .position = 5},
        .position = 25
    };

    LexerException<Mark> ex(mark, "Error details");
    std::string message = ex.what();

    // Should follow pattern: [file@line:col] message
    REQUIRE(message.find("[") != std::string::npos);
    REQUIRE(message.find("]") != std::string::npos);
    REQUIRE(message.find("@") != std::string::npos);
    REQUIRE(message.find(":") != std::string::npos);
}

TEST_CASE("Exception with Marker location is constructible", "[exception]") {
    Marker marker{
        .file = "code.txt",
        .line = {.id = 15, .position = 0},
        .position = 50
    };

    LexerException<Marker> ex(marker, "Error at marker location");
    REQUIRE(ex.what() != nullptr);
}

// ============================================================================
// UNMATCHED EXCEPTION TESTS
// ============================================================================

TEST_CASE("Unmatched exception is constructible", "[exception][unmatched]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 3, .position = 0},
        .position = 15
    };

    Unmatched<Mark, Str> ex(mark, "}");
    REQUIRE(ex.what() != nullptr);
}

TEST_CASE("Unmatched exception message contains location", "[exception][unmatched]") {
    Mark mark{
        .file = "input.txt",
        .line = {.id = 7, .position = 10},
        .position = 42
    };

    Unmatched<Mark, Str> ex(mark, "unexpected_char");
    std::string message = ex.what();

    REQUIRE(message.find("input.txt") != std::string::npos);
    REQUIRE(message.find("7") != std::string::npos);
}

TEST_CASE("Unmatched exception message contains no token type matched", "[exception][unmatched]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 1, .position = 0},
        .position = 5
    };

    Unmatched<Mark, Str> ex(mark, "}");
    std::string message = ex.what();

    // Message format: "location no token type matched \"token\""
    REQUIRE(message.find("no token type matched") != std::string::npos);
    REQUIRE(message.find("}") != std::string::npos);
}

TEST_CASE("Unmatched exception with string value", "[exception][unmatched]") {
    Mark mark;
    Str view = "bad_token";

    Unmatched<Mark, Str> ex(mark, view);
    std::string message = ex.what();

    REQUIRE(message.find("bad_token") != std::string::npos);
}

// ============================================================================
// UNKNOWN TOKEN EXCEPTION TESTS
// ============================================================================

TEST_CASE("UnknownToken exception is constructible", "[exception][unknown]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 2, .position = 0},
        .position = 10
    };

    UnknownToken<Mark, Str> ex(mark, "?");
    REQUIRE(ex.what() != nullptr);
}

TEST_CASE("UnknownToken exception message contains location", "[exception][unknown]") {
    Mark mark{
        .file = "source.txt",
        .line = {.id = 5, .position = 20},
        .position = 35
    };

    UnknownToken<Mark, Str> ex(mark, "?");
    std::string message = ex.what();

    REQUIRE(message.find("source.txt") != std::string::npos);
    REQUIRE(message.find("5") != std::string::npos);
}

TEST_CASE("UnknownToken exception message contains token value", "[exception][unknown]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 1, .position = 0},
        .position = 8
    };

    UnknownToken<Mark, Str> ex(mark, "??");
    std::string message = ex.what();

    // Message format: "location \"token\""
    REQUIRE(message.find("??") != std::string::npos);
}

TEST_CASE("UnknownToken exception with different characters", "[exception][unknown]") {
    Mark mark;

    UnknownToken<Mark, Str> ex1(mark, "~");
    UnknownToken<Mark, Str> ex2(mark, "@");
    UnknownToken<Mark, Str> ex3(mark, "#");

    std::string msg1 = ex1.what();
    std::string msg2 = ex2.what();
    std::string msg3 = ex3.what();

    REQUIRE(msg1.find("~") != std::string::npos);
    REQUIRE(msg2.find("@") != std::string::npos);
    REQUIRE(msg3.find("#") != std::string::npos);
}

// ============================================================================
// UNEXPECTED EOF EXCEPTION TESTS
// ============================================================================

TEST_CASE("UnexpectedEOF exception is constructible", "[exception][eof]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 10, .position = 0},
        .position = 100
    };

    UnexpectedEOF<Mark> ex(mark);
    REQUIRE(ex.what() != nullptr);
}

TEST_CASE("UnexpectedEOF exception message contains location", "[exception][eof]") {
    Mark mark{
        .file = "data.txt",
        .line = {.id = 25, .position = 100},
        .position = 250
    };

    UnexpectedEOF<Mark> ex(mark);
    std::string message = ex.what();

    REQUIRE(message.find("data.txt") != std::string::npos);
    REQUIRE(message.find("25") != std::string::npos);
}

TEST_CASE("UnexpectedEOF exception message format", "[exception][eof]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 1, .position = 0},
        .position = 50
    };

    UnexpectedEOF<Mark> ex(mark);
    std::string message = ex.what();

    // Message format: "location " (with empty trailing space)
    REQUIRE(message.find("test.txt") != std::string::npos);
    REQUIRE(!message.empty());
}

// ============================================================================
// UNREACHABLE EXCEPTION TESTS
// ============================================================================

TEST_CASE("Unreachable exception is constructible", "[exception][unreachable]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 1, .position = 0},
        .position = 1
    };

    Unreachable<Mark, Str> ex(mark, "Should not reach here");
    REQUIRE(ex.what() != nullptr);
}

TEST_CASE("Unreachable exception message contains location", "[exception][unreachable]") {
    Mark mark{
        .file = "impl.txt",
        .line = {.id = 42, .position = 0},
        .position = 500
    };

    Unreachable<Mark, Str> ex(mark, "Debug reason");
    std::string message = ex.what();

    REQUIRE(message.find("impl.txt") != std::string::npos);
    REQUIRE(message.find("42") != std::string::npos);
}

TEST_CASE("Unreachable exception message contains the reason", "[exception][unreachable]") {
    Mark mark;

    Unreachable<Mark, Str> ex(mark, "This path should not execute");
    std::string message = ex.what();

    // Message format: "location reason"
    REQUIRE(message.find("This path should not execute") != std::string::npos);
}

// ============================================================================
// EXCEPTION INHERITANCE AND POLYMORPHISM
// ============================================================================

TEST_CASE("Unmatched inherits from Exception", "[exception][inheritance]") {
    Mark mark;
    Unmatched<Mark, Str> ex(mark, "token");

    LexerException<Mark>& base_ref = ex;
    REQUIRE(base_ref.what() != nullptr);
}

TEST_CASE("UnknownToken inherits from Exception", "[exception][inheritance]") {
    Mark mark;
    UnknownToken<Mark, Str> ex(mark, "token");

    LexerException<Mark>& base_ref = ex;
    REQUIRE(base_ref.what() != nullptr);
}

TEST_CASE("UnexpectedEOF inherits from Exception", "[exception][inheritance]") {
    Mark mark;
    UnexpectedEOF<Mark> ex(mark);

    LexerException<Mark>& base_ref = ex;
    REQUIRE(base_ref.what() != nullptr);
}

TEST_CASE("Unreachable inherits from Exception", "[exception][inheritance]") {
    Mark mark;
    Unreachable<Mark, Str> ex(mark, "reason");

    LexerException<Mark>& base_ref = ex;
    REQUIRE(base_ref.what() != nullptr);
}

// ============================================================================
// EXCEPTION CATCHING AND POLYMORPHIC BEHAVIOR
// ============================================================================

TEST_CASE("Can catch Unmatched as exception", "[exception][catch]") {
    Mark mark;
    bool caught = false;

    try {
        throw Unmatched<Mark, Str>(mark, "token");
    } catch (const LexerException<Mark>& e) {
        caught = true;
        REQUIRE(e.what() != nullptr);
    }

    REQUIRE(caught);
}

TEST_CASE("Can catch UnknownToken as exception", "[exception][catch]") {
    Mark mark;
    bool caught = false;

    try {
        throw UnknownToken<Mark, Str>(mark, "token");
    } catch (const LexerException<Mark>& e) {
        caught = true;
        REQUIRE(e.what() != nullptr);
    }

    REQUIRE(caught);
}

TEST_CASE("Can catch UnexpectedEOF as exception", "[exception][catch]") {
    Mark mark;
    bool caught = false;

    try {
        throw UnexpectedEOF<Mark>(mark);
    } catch (const LexerException<Mark>& e) {
        caught = true;
        REQUIRE(e.what() != nullptr);
    }

    REQUIRE(caught);
}

TEST_CASE("Can catch Unreachable as exception", "[exception][catch]") {
    Mark mark;
    bool caught = false;

    try {
        throw Unreachable<Mark, Str>(mark, "reason");
    } catch (const LexerException<Mark>& e) {
        caught = true;
        REQUIRE(e.what() != nullptr);
    }

    REQUIRE(caught);
}

TEST_CASE("Can catch as std::exception", "[exception][catch]") {
    Mark mark;
    bool caught = false;

    try {
        throw UnknownToken<Mark, Str>(mark, "token");
    } catch (const std::exception& e) {
        caught = true;
        REQUIRE(e.what() != nullptr);
    }

    REQUIRE(caught);
}

// ============================================================================
// REALISTIC LEXER ERROR SCENARIOS
// ============================================================================

TEST_CASE("Exception for unmatched closing brace", "[exception][scenario]") {
    Mark mark{
        .file = "code.txt",
        .line = {.id = 5, .position = 0},
        .position = 25
    };

    Unmatched<Mark, Str> ex(mark, "}");
    std::string message = ex.what();

    // Should have useful debugging info
    REQUIRE(message.find("code.txt") != std::string::npos);
    REQUIRE(message.find("5") != std::string::npos);
    REQUIRE(message.find("}") != std::string::npos);
}

TEST_CASE("Exception for unterminated string", "[exception][scenario]") {
    Mark mark{
        .file = "script.txt",
        .line = {.id = 10, .position = 50},
        .position = 75
    };

    UnexpectedEOF<Mark> ex(mark);
    std::string message = ex.what();

    // When EOF is encountered unexpectedly
    REQUIRE(message.find("script.txt") != std::string::npos);
    REQUIRE(message.find("10") != std::string::npos);
}

TEST_CASE("Exception for unknown character", "[exception][scenario]") {
    Mark mark{
        .file = "config.txt",
        .line = {.id = 3, .position = 0},
        .position = 12
    };

    UnknownToken<Mark, Str> ex(mark, "§");
    std::string message = ex.what();

    REQUIRE(message.find("config.txt") != std::string::npos);
    REQUIRE(message.find("3") != std::string::npos);
    REQUIRE(message.find("§") != std::string::npos);
}

// ============================================================================
// EXCEPTION MESSAGE CONSISTENCY
// ============================================================================

TEST_CASE("All exceptions format Mark the same way", "[exception][consistency]") {
    Mark mark{
        .file = "test.txt",
        .line = {.id = 7, .position = 10},
        .position = 35
    };

    Unmatched<Mark, Str> ex1(mark, "x");
    UnknownToken<Mark, Str> ex2(mark, "x");
    UnexpectedEOF<Mark> ex3(mark);

    std::string msg1 = ex1.what();
    std::string msg2 = ex2.what();
    std::string msg3 = ex3.what();

    // All should contain the Mark info
    REQUIRE(msg1.find("test.txt") != std::string::npos);
    REQUIRE(msg2.find("test.txt") != std::string::npos);
    REQUIRE(msg3.find("test.txt") != std::string::npos);

    REQUIRE(msg1.find("7") != std::string::npos);
    REQUIRE(msg2.find("7") != std::string::npos);
    REQUIRE(msg3.find("7") != std::string::npos);
}

TEST_CASE("Exception with default Mark", "[exception][default]") {
    Mark mark;  // Default: file="__main__", line=1, position=1

    UnknownToken<Mark, Str> ex(mark, "?");
    std::string message = ex.what();

    REQUIRE(message.find("__main__") != std::string::npos);
    REQUIRE(!message.empty());
}
