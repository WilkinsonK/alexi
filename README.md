# Alexi
Lexical analysis engine for configuring lexical parsers.
---

This project comprises of a library which enables its users to define lexical
parsers for their specific use-case.

## What are Lexical Parsers?
Commonly known as **lexers**, lexical parsers are utilzed, typically, to break
down source code into individual **tokens**. These tokens, when parsed, are used
as the basic building blocks of language grammar.

The idea is simple, take some input string or byte array and identify each of
its parts. Say for the example below, where we are assuming whitespace needs to
be ignored and punctuation has its own identity, we would break the input down
as such:

```txt
Example input: "Mary had a little lamb."
Example output: ["Mary", "had", "a", "little", "lamb", "."]
```

## How Does Alexi Work?
Alexi works in the same fashion as the previous parsing example. Once a lexer
has been generated from the engine, it then will search through the stream one
token at a time with the `Lexer::next_token()` method. This will return a
`Token` instance as the basic information necessary for downstream **IR**
parsers, consumers and processors.

## Types Overview
Listed below are examples of what the components this library is composed of.
With some luck, their descriptions will assist you in understanding how to best
take advantage of the library's features.

### [Engine][1]
The `Engine` type is perhaps the highest level type in **Alexi**. This type
allows us to preconfigure the anticipated behavior of any `Lexer`s generated
from it. The core intent is to be able define allowed *token types* (We use
`Kind` as a type definition), declare expected keywords and ensures common types
are included before producing a lexer.

#### Member Functions
|**Method Name**|**Variations**|**Return Type**|
|-|-|-|
|**from_data**|2|`Lexer`|
|**from_file**|2|`Lexer`|
|**use_keywords**|3|`Engine`|
|**use_kind**|3|`Engine`|

### Kind
The `Kind` type is an `Alexi` primitive. When declared, it describes the rules
of what a `Token` should look like and how it should be handled. We use this
type throughout the rest of the library as the foundation for all other types.
This allows us to drive analysis according to whatever the expected syntax of
a given input should be.

#### Kind Rules
There are some rules that govern a `Kind`:

- A `Kind` must be unique to other `Kind` definitions.
- A `Kind` must have a unique name.
- A `Kind` pattern may be shared with other `Kind`s, but must use the `Kind::predicate` to be distinguished from other `Kinds`.
- `Kind::pattern` may not capture all possible characters (`R"(.*)"`).
- The `Kind::order` must be a `float` between **0** and **1**.
- The `Kind::action` as a bit map must not contain both `Action::CONSUME` and `Action::IGNORE`. `Action::CONSUME` will override `Action::IGNORE`.
- The `Kind::action` cannot be `Action::NOTHING`. This will produce and error.

#### Example Kind

```cpp
#include "alexi/kind.hpp"

using namespace alexi;

static const Kind my_kind{
    .pattern = R"(\w+)",
    .name    = "WORD",
    // .action    = Action::CONSUME,
    // .order     = 0.1,
    // .natural   = false,
    // .predicate = [](auto _){ return true; }
};
```

#### Kind Attributes
`Kind` member attributes can be found [here][3].

#### Predefined Kinds
Since most syntaxes are often reused or rarely change, `Alexi` provides a series
of predefined `Kind`s that can be found [here][2]. These definitions include
syntaxes for arithmetic operations, unnatural identifiers, strings and many
more.

### Matcher
### Lexer
### Token

[1]: alexi/engine/README.md
[2]: alexi/token/include/alexi/kinds.hpp
[3]: alexi/token/include/alexi/kind.hpp
