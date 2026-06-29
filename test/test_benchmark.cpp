#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"
#include "alexi/matcher.hpp"

using namespace alexi;

// ============================================================================
// BENCHMARK UTILITIES
// ============================================================================

// Generate test data of various sizes
Str generate_identifier_sequence(size_t count) {
    Str result;
    for (size_t i = 0; i < count; ++i) {
        result += "identifier";
        if (i < count - 1) result += " ";
    }
    return result;
}

Str generate_mixed_tokens(size_t count) {
    Str result;
    for (size_t i = 0; i < count; ++i) {
        result += "var_";
        result += std::to_string(i);
        result += " = ";
        result += std::to_string(i * 42);
        result += "; ";
    }
    return result;
}

Str generate_string_literals(size_t count) {
    Str result;
    for (size_t i = 0; i < count; ++i) {
        result += "\"string ";
        result += std::to_string(i);
        result += "\" ";
    }
    return result;
}

Str generate_multiline_content(size_t lines) {
    Str result;
    for (size_t i = 0; i < lines; ++i) {
        result += "line_";
        result += std::to_string(i);
        result += " = value_";
        result += std::to_string(i * 10);
        result += "\n";
    }
    return result;
}

// ============================================================================
// MATCHER PERFORMANCE BENCHMARKS
// ============================================================================

TEST_CASE("BENCHMARK: Single Matcher - IDENTIFIER pattern", "[benchmark][matcher]") {
    Matcher m = Matcher::from({
        .pattern = R"([A-Za-z_]\w*)",
        .name = "IDENTIFIER"
    });

    Str input = "hello";
    Mark mark;

    BENCHMARK("Match identifier against 'hello'") {
        return m.match(std::string_view(input), mark);
    };
}

TEST_CASE("BENCHMARK: Single Matcher - NUMBER pattern", "[benchmark][matcher]") {
    Matcher m = Matcher::from({
        .pattern = R"(\d+)",
        .name = "NUMBER"
    });

    Str input = "12345";
    Mark mark;

    BENCHMARK("Match number against '12345'") {
        return m.match(std::string_view(input), mark);
    };
}

TEST_CASE("BENCHMARK: Complex regex - STRING pattern", "[benchmark][matcher]") {
    Matcher m = Matcher::from({
        .pattern = R"("([^"\\]|\\.)*"|'([^'\\]|\\.)*'|""".*""")",
        .name = "LITERALSTR"
    });

    Str input = "\"complex string with \\\"escaped\\\" quotes\"";
    Mark mark;

    BENCHMARK("Match complex string pattern") {
        return m.match(std::string_view(input), mark);
    };
}

TEST_CASE("BENCHMARK: Failed match - pattern mismatch", "[benchmark][matcher]") {
    Matcher m = Matcher::from({
        .pattern = R"([0-9]+)",
        .name = "NUMBER"
    });

    Str input = "hello";  // Won't match
    Mark mark;

    BENCHMARK("Failed match attempt (no regex match)") {
        return m.match(std::string_view(input), mark);
    };
}

// ============================================================================
// LEXER INITIALIZATION BENCHMARKS
// ============================================================================

TEST_CASE("BENCHMARK: Engine initialization", "[benchmark][engine]") {
    BENCHMARK("Create new Engine") {
        return Engine();
    };
}

TEST_CASE("BENCHMARK: Lexer creation with builtin kinds", "[benchmark][lexer]") {
    Engine engine;
    Str data = "hello world";

    BENCHMARK("Engine::from_data() with builtin kinds") {
        return engine.from_data(data);
    };
}

TEST_CASE("BENCHMARK: Lexer creation with many custom kinds", "[benchmark][lexer]") {
    Engine engine;
    
    // Add multiple custom kinds (simulates real usage)
    for (int i = 0; i < 10; ++i) {
        engine.use_kinds({
            {.pattern = R"(\w+)", .name = "IDENT_" + std::to_string(i)},
        });
    }

    Str data = "hello world";

    BENCHMARK("Lexer creation with 10+ custom kinds") {
        return engine.from_data(data);
    };
}

// ============================================================================
// LEXING THROUGHPUT BENCHMARKS
// ============================================================================

TEST_CASE("BENCHMARK: Lex small input (10 identifiers)", "[benchmark][throughput]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Str data = generate_identifier_sequence(10);

    BENCHMARK("Lex 10 identifiers (~60 chars)") {
        Lexer l{.marker = {}, .matchers = ks, .data = data};
        Vec<Token> tokens;
        while (1) {
            auto t = l.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
        return tokens.size();
    };
}

TEST_CASE("BENCHMARK: Lex medium input (100 identifiers)", "[benchmark][throughput]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Str data = generate_identifier_sequence(100);

    BENCHMARK("Lex 100 identifiers (~600 chars)") {
        Lexer l{.marker = {}, .matchers = ks, .data = data};
        Vec<Token> tokens;
        while (1) {
            auto t = l.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
        return tokens.size();
    };
}

TEST_CASE("BENCHMARK: Lex large input (1000 identifiers)", "[benchmark][throughput]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Str data = generate_identifier_sequence(1000);

    BENCHMARK("Lex 1000 identifiers (~6KB)") {
        Lexer l{.marker = {}, .matchers = ks, .data = data};
        Vec<Token> tokens;
        while (1) {
            auto t = l.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
        return tokens.size();
    };
}

// ============================================================================
// MIXED TOKEN BENCHMARKS
// ============================================================================

TEST_CASE("BENCHMARK: Lex mixed tokens (assignments)", "[benchmark][scenario]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::ASSIGN);
    ks.add(kinds::LITERALNUM);
    ks.add(kinds::SEMICOLON);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Str data = generate_mixed_tokens(50);  // 50 assignments

    BENCHMARK("Lex 50 assignment statements (~900 chars)") {
        Lexer l{.marker = {}, .matchers = ks, .data = data};
        Vec<Token> tokens;
        while (1) {
            auto t = l.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
        return tokens.size();
    };
}

// ============================================================================
// STRING LITERAL BENCHMARKS
// ============================================================================

TEST_CASE("BENCHMARK: Lex string literals", "[benchmark][scenario]") {
    Kinds ks;
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Str data = generate_string_literals(50);

    BENCHMARK("Lex 50 string literals") {
        Lexer l{.marker = {}, .matchers = ks, .data = data};
        Vec<Token> tokens;
        while (1) {
            auto t = l.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
        return tokens.size();
    };
}

// ============================================================================
// MULTILINE BENCHMARKS
// ============================================================================

TEST_CASE("BENCHMARK: Lex multiline content (100 lines)", "[benchmark][scenario]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::ASSIGN);
    ks.add(kinds::LITERALNUM);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::UNKNOWN);

    Str data = generate_multiline_content(100);

    BENCHMARK("Lex 100 lines of assignments") {
        Lexer l{.marker = {}, .matchers = ks, .data = data};
        Vec<Token> tokens;
        while (1) {
            auto t = l.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
        return tokens.size();
    };
}

TEST_CASE("BENCHMARK: Lex multiline with many kinds", "[benchmark][scenario]") {
    Kinds ks;
    ks.add(kinds::IDENTIFIER);
    ks.add(kinds::ASSIGN);
    ks.add(kinds::LITERALNUM);
    ks.add(kinds::LITERALSTR);
    ks.add(kinds::SEMICOLON);
    ks.add(kinds::BRACEL);
    ks.add(kinds::BRACER);
    ks.add(kinds::PARENL);
    ks.add(kinds::PARENR);
    ks.add(kinds::COMMA);
    ks.add(kinds::DOT);
    ks.add(kinds::NEWLINE);
    ks.add(kinds::WHITESPACE);
    ks.add(kinds::COMMENT);
    ks.add(kinds::UNKNOWN);

    Str data = generate_multiline_content(50);

    BENCHMARK("Lex 50 lines with 15 different token kinds") {
        Lexer l{.marker = {}, .matchers = ks, .data = data};
        Vec<Token> tokens;
        while (1) {
            auto t = l.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
        return tokens.size();
    };
}

// ============================================================================
// PATTERN COMPILATION BENCHMARKS
// ============================================================================

TEST_CASE("BENCHMARK: Matcher creation - simple pattern", "[benchmark][compilation]") {
    BENCHMARK("Create Matcher with simple pattern [A-Za-z_]\\w*") {
        return Matcher::from({
            .pattern = R"([A-Za-z_]\w*)",
            .name = "IDENTIFIER"
        });
    };
}

TEST_CASE("BENCHMARK: Matcher creation - complex pattern", "[benchmark][compilation]") {
    BENCHMARK("Create Matcher with complex pattern (string literal)") {
        return Matcher::from({
            .pattern = R"("([^"\\]|\\.)*"|'([^'\\]|\\.)*'|""".*""")",
            .name = "LITERALSTR"
        });
    };
}

TEST_CASE("BENCHMARK: Matcher creation from Kind", "[benchmark][compilation]") {
    Kind k{.pattern = R"(\d+)", .name = "NUMBER"};
    
    BENCHMARK("Create Matcher from Kind") {
        return Matcher::from(k);
    };
}

// ============================================================================
// END-TO-END BENCHMARKS
// ============================================================================

TEST_CASE("BENCHMARK: Full pipeline - Engine → Lexer → Tokens (small)", "[benchmark][e2e]") {
    BENCHMARK("Full pipeline: small input (10 identifiers)") {
        Kinds ks;
        ks.add(kinds::IDENTIFIER);
        ks.add(kinds::WHITESPACE);
        ks.add(kinds::UNKNOWN);
        
        Str data = generate_identifier_sequence(10);
        Lexer lexer{.marker = {}, .matchers = ks, .data = data};
        
        Vec<Token> tokens;
        while (1) {
            auto t = lexer.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
        return tokens.size();
    };
}

TEST_CASE("BENCHMARK: Full pipeline - Engine → Lexer → Tokens (medium)", "[benchmark][e2e]") {
    BENCHMARK("Full pipeline: medium input (100 identifiers)") {
        Kinds ks;
        ks.add(kinds::IDENTIFIER);
        ks.add(kinds::WHITESPACE);
        ks.add(kinds::UNKNOWN);
        
        Str data = generate_identifier_sequence(100);
        Lexer lexer{.marker = {}, .matchers = ks, .data = data};
        
        Vec<Token> tokens;
        while (1) {
            auto t = lexer.next_token();
            tokens.push_back(t);
            if (t == kinds::UEOF) break;
        }
        return tokens.size();
    };
}

// ============================================================================
// PREDICATE VALIDATION BENCHMARKS
// ============================================================================

TEST_CASE("BENCHMARK: Matching with predicate validation", "[benchmark][predicate]") {
    Kind keyword_kind{
        .pattern = R"([A-Za-z_]\w*)",
        .name = "KEYWORD",
        .natural = false,
        .predicate = [](const std::string_view s) {
            return s == "if" || s == "else" || s == "while" || s == "for";
        }
    };
    
    Matcher m = Matcher::from(keyword_kind);
    Str input = "while";
    Mark mark;

    BENCHMARK("Match with predicate validation (match succeeds)") {
        return m.match(std::string_view(input), mark);
    };
}

TEST_CASE("BENCHMARK: Matching with predicate validation (no match)", "[benchmark][predicate]") {
    Kind keyword_kind{
        .pattern = R"([A-Za-z_]\w*)",
        .name = "KEYWORD",
        .natural = false,
        .predicate = [](const std::string_view s) {
            return s == "if" || s == "else" || s == "while" || s == "for";
        }
    };
    
    Matcher m = Matcher::from(keyword_kind);
    Str input = "myvar";  // Not a keyword
    Mark mark;

    BENCHMARK("Match with predicate validation (predicate fails)") {
        return m.match(std::string_view(input), mark);
    };
}
