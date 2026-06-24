export module alexi.util:aliases;
import std;

namespace alexi {
    export using Len    = std::size_t;
    export using LineNo = std::size_t;
    export using Str    = std::string;
    export using StrV   = std::string_view;
    export using Ord    = double;
    export using Offset = std::size_t;
    export using Path   = std::filesystem::path;

    export template <typename T>
    using Opt = std::optional<T>;

    export template <typename T>
    std::ostream &operator<<(std::ostream &os, const Opt<T> opt) {
        if (opt.has_value()) return std::cout << "Some(" << opt.value() << ")";
        else return std::cout << "None";
    }
}
