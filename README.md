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
|**use_kind**|3|`Kind`|

### Kind
### Matcher
### Lexer
### Token

[1]: alexi/engine/README.md
