export module alexi.token:kinds;
export import :kind;
import std;

namespace alexi {
    export struct Kinds {
        std::set<std::shared_ptr<Kind>> inner;
        // Adds an `alexi::token::Kind` into this container
        // if the new entry is unique.
        // returns a `boolean` if the add was successful.
        bool add(const Kind kind);
        // Has an `alexi::token::Kind` removed from this
        // container.
        std::shared_ptr<Kind> drop(const Str);
        // Gets a reference to an `alexi::token::Kind` from
        // this container.
        const std::shared_ptr<Kind> find(const Str) const;
        // Gets a reference to an `alexi::token::Kind` from
        // this container.
        constexpr const std::shared_ptr<Kind> operator[](const Str) const;
        // Has an `alexi::token::Kind` removed from this
        // container.
        constexpr std::shared_ptr<Kind> operator()(const Str);
        // Adds an `alexi::token::Kind` into this container
        // if the new entry is unique.
        // returns a `boolean` if the add was successful.
        constexpr bool operator|=(const Kind);
    };

    inline bool Kinds::add(const Kind kind) {
        return inner.insert(std::make_shared<Kind>(kind)).second;
    }

    inline const std::shared_ptr<Kind> Kinds::find(const Str name) const {
        auto response = inner | std::views::filter([name](const auto &k) { return k->name == name; });
        if (response.begin() != response.end())
            return *response.begin();
        return nullptr;
    }

    inline std::shared_ptr<Kind> Kinds::drop(const Str name) {
        return inner.extract(find(name)).value();
    }

    constexpr inline const std::shared_ptr<Kind> Kinds::operator[](const Str name) const {
        return find(name);
    }

    constexpr inline std::shared_ptr<Kind> Kinds::operator()(const Str name) {
        return drop(name);
    }

    constexpr inline bool Kinds::operator|=(const Kind kind) {
        return add(kind);
    }
}
