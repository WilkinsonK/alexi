# Alexi
## Engine
The `Engine` type is perhaps the highest level type in **Alexi**. This type
allows us to preconfigure the anticipated behavior of any `Lexer`s generated
from it. The core intent is to be able define allowed *token types* (We use
`Kind` as a type definition), declare expected keywords and ensures common types
are included before producing a lexer.

```cpp
#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"

using namespace alexi;

void example(void) {
    Engine engine;
    engine = engine.use_kinds(kinds::IDENTIFIER).use_kinds(kinds::ELLIPSIS);
    auto lexer = engine.from_data("lorem ipsum ...");
}
```

### From Data
`Engine::from_data()` generates a `Lexer` from anonymous data.

```cpp
#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"

using namespace alexi;

void example(void) {
    Engine engine;
    engine = engine.use_kinds(kinds::IDENTIFIER).use_kinds(kinds::ELLIPSIS);
    auto lexer = engine.from_data("lorem ipsum ...");
}
```

### From File
`Engine::from_file()` generates a `Lexer` from some file path or file
descriptor. It will try to read the file and generate the lexer from the data
contained at that file location.

```cpp
#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"

using namespace alexi;

void example(void) {
    Engine engine;
    engine = engine.use_kinds(kinds::IDENTIFIER).use_kinds(kinds::ELLIPSIS);
    auto lexer = engine.from_file("/path/to/file"); // Can error if file not found.
}
```

### Use Kinds
`Engine::use_kind()` adds one or many `Kind`s to the engine's internal state.
These `Kinds` will be transformed into `Matcher`s once a `Lexer` needs to be
generated.

```cpp
#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"

using namespace alexi;

void example(void) {
    Engine engine;
    engine = engine.use_kinds({
        kinds::IDENTIFIER,
        kinds::ELLIPSIS,
        ...
    });
}
```

### Use Keywords
`Engine::use_keywords()` adds one or many **strings** as special identifiers.
These identifiers will be used during *token matching* to determine if some
unknown identifier is a **keyword** or not. This will be transformed into a
`Kind` definition and added to the lexer matchers when a `Lexer` needs to be
generated.

```cpp
#include "alexi/engine.hpp"
#include "alexi/kinds.hpp"

using namespace alexi;

void example(void) {
    Engine engine;
    engine = engine.use_keywords({
        "let",
        "const",
        "struct",
        ...
    });
}
```
