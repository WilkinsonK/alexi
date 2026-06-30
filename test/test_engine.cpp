#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <filesystem>

#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"
#include "alexi/kind.hpp"

using namespace alexi;

// Helper function to create temporary test files
Path create_temp_file(const Str &content) {
    static int counter = 0;
    Path temp_path = std::filesystem::temp_directory_path() / (Str("alexi_test_") + std::to_string(counter++));
    std::ofstream file(temp_path);
    file << content;
    file.close();
    return temp_path;
}

// Helper function to clean up temp files
void cleanup_temp_file(const Path &path) {
    std::filesystem::remove(path);
}

// Helper function to check if a matcher exists by kind name
bool matcher_exists(const Matchers &matchers, const Str &kind_name) {
    for (const auto &m : matchers) {
        if (m->kind->name == kind_name) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// CONSTRUCTOR AND INITIALIZATION
// ============================================================================

TEST_CASE("Engine constructs with default state", "[engine]") {
    Engine engine;
    REQUIRE(engine.keywords.empty());
}

TEST_CASE("Engine default state contains empty keywords", "[engine]") {
    Engine engine;
    REQUIRE(engine.keywords.size() == 0);
}

// ============================================================================
// FROM_DATA VARIANTS - BASIC STRING
// ============================================================================

TEST_CASE("Engine::from_data with single string parameter", "[engine][engine::from_data]") {
    Engine engine;
    Lexer lexer = engine.from_data("hello world");

    REQUIRE(lexer.data == "hello world");
    REQUIRE(lexer.marker.file == FILE_DEFAULT);
}

TEST_CASE("Engine::from_data with empty string", "[engine][engine::from_data]") {
    Engine engine;
    Lexer lexer = engine.from_data("");

    REQUIRE(lexer.data.empty());
    REQUIRE(lexer.marker.file == FILE_DEFAULT);
}

TEST_CASE("Engine::from_data with multiline string", "[engine][engine::from_data]") {
    Engine engine;
    Str multiline = "line1\nline2\nline3";
    Lexer lexer = engine.from_data(multiline);

    REQUIRE(lexer.data == multiline);
}

// ============================================================================
// FROM_DATA VARIANTS - WITH FILE PATH
// ============================================================================

TEST_CASE("Engine::from_data with file path and string", "[engine][engine::from_data]") {
    Engine engine;
    Path custom_path = "custom_file.txt";
    Lexer lexer = engine.from_data(custom_path, "test data");

    REQUIRE(lexer.data == "test data");
    REQUIRE(lexer.marker.file == custom_path);
}

TEST_CASE("Engine::from_data sets correct file path", "[engine][engine::from_data]") {
    Engine engine;
    Path file_path = "myfile.abc";
    Lexer lexer = engine.from_data(file_path, "content");

    REQUIRE(lexer.marker.file == file_path);
}

// ============================================================================
// FROM_FILE VARIANTS - VALID FILE
// ============================================================================

TEST_CASE("Engine::from_file with valid file", "[engine][engine::from_file]") {
    Path temp_file = create_temp_file("file content here");
    Engine engine;

    Lexer lexer = engine.from_file(temp_file);

    REQUIRE(lexer.data == "file content here");
    REQUIRE(lexer.marker.file == temp_file);

    cleanup_temp_file(temp_file);
}

TEST_CASE("Engine::from_file with multiline file", "[engine][engine::from_file]") {
    Str multiline_content = "first line\nsecond line\nthird line";
    Path temp_file = create_temp_file(multiline_content);
    Engine engine;

    Lexer lexer = engine.from_file(temp_file);

    REQUIRE(lexer.data == multiline_content);

    cleanup_temp_file(temp_file);
}

TEST_CASE("Engine::from_file with empty file", "[engine][engine::from_file]") {
    Path temp_file = create_temp_file("");
    Engine engine;

    Lexer lexer = engine.from_file(temp_file);

    REQUIRE(lexer.data.empty());
    REQUIRE(lexer.marker.file == temp_file);

    cleanup_temp_file(temp_file);
}

// ============================================================================
// FROM_FILE VARIANTS - ERROR CONDITIONS
// ============================================================================

TEST_CASE("Engine::from_file throws on file not found", "[engine][engine::from_file]") {
    Engine engine;
    Path nonexistent_file = std::filesystem::temp_directory_path() / "nonexistent_alexi_test_file_12345.txt";

    REQUIRE_THROWS(engine.from_file(nonexistent_file));
}

TEST_CASE("Engine::from_file error includes file path", "[engine][engine::from_file]") {
    Engine engine;
    Path nonexistent_file = "path/to/nonexistent/file.txt";

    try {
        engine.from_file(nonexistent_file);
        FAIL("Should have thrown");
    } catch (const std::runtime_error &e) {
        Str error_msg(e.what());
        REQUIRE(!error_msg.empty());
    }
}

// ============================================================================
// USE_KEYWORDS VARIANTS - SINGLE STRING
// ============================================================================

TEST_CASE("Engine::use_keywords with single keyword", "[engine][engine::keywords]") {
    Engine engine;
    auto result = engine.use_keywords("if");

    REQUIRE(engine.keywords.size() == 1);
    REQUIRE(engine.keywords[0] == "if");
}

TEST_CASE("Engine::use_keywords returns Self for chaining", "[engine][engine::keywords]") {
    Engine engine;
    Engine &result = engine.use_keywords("let");

    REQUIRE(&result == &engine);
    REQUIRE(engine.keywords.size() == 1);
}

TEST_CASE("Engine::use_keywords with multiple calls adds all", "[engine][engine::keywords]") {
    Engine engine;
    engine.use_keywords("if");
    engine.use_keywords("else");
    engine.use_keywords("while");

    REQUIRE(engine.keywords.size() == 3);
}

// ============================================================================
// USE_KEYWORDS VARIANTS - VECTOR
// ============================================================================

TEST_CASE("Engine::use_keywords with vector of strings", "[engine][engine::keywords]") {
    Engine engine;
    Vec<Str> kwds = {"if", "else", "while", "for"};
    engine.use_keywords(kwds);

    REQUIRE(engine.keywords.size() == 4);
}

TEST_CASE("Engine::use_keywords with empty vector", "[engine][engine::keywords]") {
    Engine engine;
    Vec<Str> empty_kwds;
    engine.use_keywords(empty_kwds);

    REQUIRE(engine.keywords.size() == 0);
}

TEST_CASE("Engine::use_keywords with single-element vector", "[engine][engine::keywords]") {
    Engine engine;
    Vec<Str> kwds = {"return"};
    engine.use_keywords(kwds);

    REQUIRE(engine.keywords.size() == 1);
    REQUIRE(engine.keywords[0] == "return");
}

// ============================================================================
// USE_KEYWORDS VARIANTS - ITERATORS
// ============================================================================

TEST_CASE("Engine::use_keywords with iterators", "[engine][engine::keywords]") {
    Engine engine;
    Vec<Str> kwds = {"break", "continue", "switch", "case"};
    engine.use_keywords(kwds);

    REQUIRE(engine.keywords.size() == 4);
}

TEST_CASE("Engine::use_keywords with multiple keywords from vector", "[engine][engine::keywords]") {
    Engine engine;
    Vec<Str> kwds = {"x", "y", "z"};
    engine.use_keywords(kwds);

    REQUIRE(engine.keywords.size() == 3);
}

// ============================================================================
// USE_KEYWORDS COMBINATIONS AND EDGE CASES
// ============================================================================

TEST_CASE("Engine::use_keywords chained multiple times", "[engine][engine::keywords][engine::builder]") {
    Engine engine;
    Vec<Str> kwds{"while", "for"};
    engine
        .use_keywords("if")
        .use_keywords("else")
        .use_keywords(kwds);

    REQUIRE(engine.keywords.size() == 4);
}

TEST_CASE("Engine::use_keywords with duplicate keywords", "[engine][engine::keywords]") {
    Engine engine;
    engine.use_keywords("if");
    engine.use_keywords("if");

    REQUIRE(engine.keywords.size() == 2);
}

TEST_CASE("Engine::use_keywords preserves order when chaining", "[engine][engine::keywords]") {
    Engine engine;
    engine.use_keywords("first").use_keywords("second").use_keywords("third");

    REQUIRE(engine.keywords.size() == 3);
    REQUIRE(engine.keywords[0] == "third");
    REQUIRE(engine.keywords[1] == "second");
    REQUIRE(engine.keywords[2] == "first");
}

// ============================================================================
// USE_KINDS VARIANTS - SINGLE KIND
// ============================================================================

TEST_CASE("Engine::use_kinds with single Kind", "[engine][engine::kinds]") {
    Engine engine;
    Kind k{.pattern = R"(\d+)", .name = "NUMBER"};
    engine.use_kinds(k);

    REQUIRE(engine.kinds.find("NUMBER") != nullptr);
}

TEST_CASE("Engine::use_kinds returns Self for chaining", "[engine][engine::kinds]") {
    Engine engine;
    Kind k{.pattern = R"(\.)" , .name = "DOT"};
    Engine &result = engine.use_kinds(k);

    REQUIRE(&result == &engine);
    REQUIRE(engine.kinds.find("DOT") != nullptr);
}

// ============================================================================
// USE_KINDS VARIANTS - VECTOR
// ============================================================================

TEST_CASE("Engine::use_kinds with vector of Kinds", "[engine][engine::kinds]") {
    Engine engine;
    Vec<Kind> kinds = {
        Kind{.pattern = R"(\d+)", .name = "NUMBER"},
        Kind{.pattern = R"([a-z]+)", .name = "LOWER"},
        Kind{.pattern = R"([A-Z]+)", .name = "UPPER"}
    };
    engine.use_kinds(kinds);

    REQUIRE(engine.kinds.find("NUMBER") != nullptr);
    REQUIRE(engine.kinds.find("LOWER") != nullptr);
    REQUIRE(engine.kinds.find("UPPER") != nullptr);
}

TEST_CASE("Engine::use_kinds with empty vector", "[engine][engine::kinds]") {
    Engine engine;
    Vec<Kind> empty_kinds;
    engine.use_kinds(empty_kinds);

    REQUIRE(engine.kinds.find("ANYTHING") == nullptr);
}

// ============================================================================
// USE_KINDS VARIANTS - ITERATORS
// ============================================================================

TEST_CASE("Engine::use_kinds with iterators", "[engine][engine::kinds]") {
    Engine engine;
    Vec<Kind> kinds = {
        Kind{.pattern = R"(\+)", .name = "PLUS"},
        Kind{.pattern = R"(-)", .name = "MINUS"},
        Kind{.pattern = R"(\*)", .name = "STAR"},
        Kind{.pattern = R"(/)", .name = "SLASH"}
    };
    engine.use_kinds(kinds);

    REQUIRE(engine.kinds.find("PLUS") != nullptr);
    REQUIRE(engine.kinds.find("MINUS") != nullptr);
    REQUIRE(engine.kinds.find("STAR") != nullptr);
    REQUIRE(engine.kinds.find("SLASH") != nullptr);
}

// ============================================================================
// USE_KINDS COMBINATIONS AND EDGE CASES
// ============================================================================

TEST_CASE("Engine::use_kinds chained multiple times", "[engine][engine::kinds][engine::builder]") {
    Engine engine;
    engine
        .use_kinds(Kind{.pattern = R"(,)", .name = "COMMA"})
        .use_kinds(Kind{.pattern = R"(;)", .name = "SEMICOLON"})
        .use_kinds(Kind{.pattern = R"(:)", .name = "COLON"});

    REQUIRE(engine.kinds.find("COMMA") != nullptr);
    REQUIRE(engine.kinds.find("SEMICOLON") != nullptr);
    REQUIRE(engine.kinds.find("COLON") != nullptr);
}

// ============================================================================
// METHOD CHAINING AND BUILDER PATTERN
// ============================================================================

TEST_CASE("Engine supports fluent builder pattern", "[engine][engine::builder]") {
    Engine engine;
    Lexer lexer = engine
        .use_keywords("if")
        .use_keywords("else")
        .use_kinds(Kind{.pattern = R"(\d+)", .name = "NUM"})
        .from_data("if x else");

    REQUIRE(engine.keywords.size() == 2);
    REQUIRE(engine.kinds.find("NUM") != nullptr);
    REQUIRE(lexer.data == "if x else");
}

TEST_CASE("Engine builder chain with keywords and kinds together", "[engine][engine::builder]") {
    Engine engine;
    engine
        .use_keywords("let")
        .use_kinds(Kind{.pattern = R"(=)", .name = "ASSIGN"})
        .use_keywords("const")
        .use_kinds(Kind{.pattern = R"(;)", .name = "SEMI"});

    REQUIRE(engine.keywords.size() == 2);
    REQUIRE(engine.kinds.find("ASSIGN") != nullptr);
    REQUIRE(engine.kinds.find("SEMI") != nullptr);
}

// ============================================================================
// LEXER GENERATION WITH VARIOUS CONFIGURATIONS
// ============================================================================

TEST_CASE("Engine generates Lexer with no custom configuration", "[engine][engine::lexer]") {
    Engine engine;
    Lexer lexer = engine.from_data("hello");

    REQUIRE(lexer.matchers.begin() != lexer.matchers.end());
}

TEST_CASE("Engine generates Lexer with keywords", "[engine][engine::lexer]") {
    Engine engine;
    engine.use_keywords("if").use_keywords("else");
    Lexer lexer = engine.from_data("if condition else");

    REQUIRE(matcher_exists(lexer.matchers, "KEYWORD"));
}

TEST_CASE("Engine generates Lexer with custom kinds", "[engine][engine::lexer]") {
    Engine engine;
    engine.use_kinds(Kind{.pattern = R"(\d+)", .name = "INTEGER"});
    Lexer lexer = engine.from_data("123 456");

    REQUIRE(matcher_exists(lexer.matchers, "INTEGER"));
}

TEST_CASE("Engine always adds WHITESPACE to generated Lexer", "[engine][engine::lexer]") {
    Engine engine;
    Lexer lexer = engine.from_data("a b c");

    REQUIRE(matcher_exists(lexer.matchers, "WHITESPACE"));
}

TEST_CASE("Engine always adds NEWLINE to generated Lexer", "[engine][engine::lexer]") {
    Engine engine;
    Lexer lexer = engine.from_data("a\nb\nc");

    REQUIRE(matcher_exists(lexer.matchers, "NEWLINE"));
}

TEST_CASE("Engine always adds UNKNOWN to generated Lexer", "[engine][engine::lexer]") {
    Engine engine;
    Lexer lexer = engine.from_data("text");

    REQUIRE(matcher_exists(lexer.matchers, "UNKNOWN"));
}

// ============================================================================
// KEYWORDS AND KINDS INTERACTION
// ============================================================================

TEST_CASE("Engine keywords don't conflict with kinds", "[engine][engine::keywords][engine::kinds]") {
    Engine engine;
    engine
        .use_keywords("if")
        .use_kinds(Kind{.pattern = R"(if)", .name = "KEYWORD_PATTERN"});

    REQUIRE(engine.keywords.size() == 1);
    REQUIRE(engine.kinds.find("KEYWORD_PATTERN") != nullptr);
}

TEST_CASE("Engine with both keywords and kinds generates valid Lexer", "[engine][engine::lexer]") {
    Engine engine;
    engine
        .use_keywords("return")
        .use_kinds(Kind{.pattern = R"([a-z]+)", .name = "IDENT"});

    Lexer lexer = engine.from_data("return x");

    REQUIRE(matcher_exists(lexer.matchers, "KEYWORD"));
    REQUIRE(matcher_exists(lexer.matchers, "IDENT"));
    REQUIRE(matcher_exists(lexer.matchers, "WHITESPACE"));
}

// ============================================================================
// WHITESPACE AND NEWLINE HANDLING
// ============================================================================

TEST_CASE("Engine generated Lexer handles whitespace", "[engine][engine::whitespace]") {
    Engine engine;
    Lexer lexer = engine.from_data("a   b\t\tc");

    REQUIRE(lexer.data == "a   b\t\tc");
    REQUIRE(matcher_exists(lexer.matchers, "WHITESPACE"));
}

TEST_CASE("Engine generated Lexer handles newlines", "[engine][engine::newline]") {
    Engine engine;
    Lexer lexer = engine.from_data("line1\nline2\r\nline3\rline4");

    REQUIRE(matcher_exists(lexer.matchers, "NEWLINE"));
}

TEST_CASE("Engine generated Lexer handles mixed whitespace and newlines", "[engine][engine::whitespace][engine::newline]") {
    Engine engine;
    Lexer lexer = engine.from_data("a\n  b  \n\tc");

    REQUIRE(lexer.data == "a\n  b  \n\tc");
}

// ============================================================================
// EMPTY INPUT HANDLING
// ============================================================================

TEST_CASE("Engine handles empty data string", "[engine][engine::empty]") {
    Engine engine;
    Lexer lexer = engine.from_data("");

    REQUIRE(lexer.data.empty());
}

TEST_CASE("Engine with no configuration handles empty data", "[engine][engine::empty]") {
    Engine engine;
    Lexer lexer = engine.from_data("");

    REQUIRE(lexer.data.empty());
    REQUIRE(lexer.marker.file == FILE_DEFAULT);
}

TEST_CASE("Engine with keywords handles empty data", "[engine][engine::empty]") {
    Engine engine;
    engine.use_keywords("keyword");
    Lexer lexer = engine.from_data("");

    REQUIRE(lexer.data.empty());
}

// ============================================================================
// DEFAULT FILE PATH BEHAVIOR
// ============================================================================

TEST_CASE("Engine::from_data defaults file to __main__", "[engine][engine::from_data]") {
    Engine engine;
    Lexer lexer = engine.from_data("test");

    REQUIRE(lexer.marker.file == FILE_DEFAULT);
    REQUIRE(lexer.marker.file == "__main__");
}

TEST_CASE("Engine::from_data with explicit path overrides default", "[engine][engine::from_data]") {
    Engine engine;
    Path custom = "myfile.txt";
    Lexer lexer = engine.from_data(custom, "data");

    REQUIRE(lexer.marker.file == custom);
    REQUIRE(lexer.marker.file != FILE_DEFAULT);
}

// ============================================================================
// COMPLEX INTEGRATION SCENARIOS
// ============================================================================

TEST_CASE("Engine builds complete configuration", "[engine][engine::builder]") {
    Engine engine;
    engine
        .use_keywords("fn")
        .use_keywords("let")
        .use_keywords("const")
        .use_kinds(Kind{.pattern = R"(\d+)", .name = "NUMBER"})
        .use_kinds(Kind{.pattern = R"(\()", .name = "LPAREN"})
        .use_kinds(Kind{.pattern = R"(\))", .name = "RPAREN"});

    REQUIRE(engine.keywords.size() == 3);
    REQUIRE(engine.kinds.find("NUMBER") != nullptr);
    REQUIRE(engine.kinds.find("LPAREN") != nullptr);
    REQUIRE(engine.kinds.find("RPAREN") != nullptr);
}

TEST_CASE("Engine generates Lexer from complex configuration", "[engine][engine::lexer]") {
    Engine engine;
    engine
        .use_keywords("if")
        .use_keywords("else")
        .use_kinds(Kind{.pattern = R"([0-9]+)", .name = "INT"})
        .use_kinds(Kind{.pattern = R"([a-z_]+)", .name = "IDENT"});

    Lexer lexer = engine.from_data("if x 123 else y");

    REQUIRE(lexer.data == "if x 123 else y");
    REQUIRE(matcher_exists(lexer.matchers, "KEYWORD"));
    REQUIRE(matcher_exists(lexer.matchers, "INT"));
    REQUIRE(matcher_exists(lexer.matchers, "IDENT"));
    REQUIRE(matcher_exists(lexer.matchers, "WHITESPACE"));
}

TEST_CASE("Engine preserves configuration across multiple from_data calls", "[engine][engine::lexer]") {
    Engine engine;
    engine.use_keywords("fn").use_kinds(Kind{.pattern = R"(\{)", .name = "LBRACE"});

    Lexer lex1 = engine.from_data("fn1");
    Lexer lex2 = engine.from_data("fn2");

    REQUIRE(matcher_exists(lex1.matchers, "KEYWORD"));
    REQUIRE(matcher_exists(lex2.matchers, "KEYWORD"));
    REQUIRE(matcher_exists(lex1.matchers, "LBRACE"));
    REQUIRE(matcher_exists(lex2.matchers, "LBRACE"));
}

TEST_CASE("Engine multiple instances are independent", "[engine]") {
    Engine engine1;
    Engine engine2;

    engine1.use_keywords("if");
    engine2.use_keywords("while");

    REQUIRE(engine1.keywords.size() == 1);
    REQUIRE(engine2.keywords.size() == 1);
    REQUIRE(engine1.keywords[0] == "if");
    REQUIRE(engine2.keywords[0] == "while");
}
