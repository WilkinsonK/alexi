#pragma once

#include <set>

#include "alexi/kind.hpp"

namespace alexi {
    struct Kinds {
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
        const std::shared_ptr<Kind> operator[](const Str) const;
        // Has an `alexi::token::Kind` removed from this
        // container.
        std::shared_ptr<Kind> operator()(const Str);
        // Adds an `alexi::token::Kind` into this container
        // if the new entry is unique.
        // returns a `boolean` if the add was successful.
        bool operator|=(const Kind);
    };
}
