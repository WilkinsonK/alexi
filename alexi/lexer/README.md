# Alexi Lexer
The second stage of our library. The Alexi `Lexer` type is responsible for
pulling all other subtypes together. It's goal is to match against some buffer
the series of `Matcher`s derived from the configured `Kind`s. When a compatible
match is found, the `Lexer` returns the next available token. Once the lexer
has made it to the end of the buffer, `Lexer::next_token()` will return an
`UEOF` token.

```cpp
#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"

using namespace alexi;

int main(void) {
    Lexer lexer{
        // .marker = {},
        // .matchers = {},
        .data = ""
    };
    return 0;
}
```

### Next Token
Retrieve the next token in the data stream. If the lexer has reached the end of
the stream, it will instead return a "UEOF" token.

```cpp
#include "alexi/kinds.hpp"
#include "alexi/lexer.hpp"

using namespace alexi;

int main(void) {
    Lexer lexer{
        // .marker = {},
        // .matchers = {},
        .data = ""
    };
    auto t = lexer.next_token();
    return 0;
}
```
