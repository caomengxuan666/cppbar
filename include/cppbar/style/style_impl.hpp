#pragma once

#include "style.hpp"

namespace cppbar {
namespace style {

inline StyleConfig Style::get_config() const {
    switch (type_) {
    case Type::Ascii:
        return StyleConfig("[", "]", "=", " ", ">");
    case Type::Braille:
        return StyleConfig("│", "│", "⣿", "⣀", "⣶");
    case Type::Unicode:
    default:
        return StyleConfig("[", "]", "█", "░", ">");
    }
}

inline StyleConfig PresetStyles::classic() {
    return StyleConfig("[", "]", "=", " ", ">");
}

inline StyleConfig PresetStyles::modern() {
    return StyleConfig("[", "]", "█", "░", ">");
}

inline StyleConfig PresetStyles::minimal() {
    return StyleConfig("", "", "=", " ", ">");
}

inline StyleConfig PresetStyles::blocks() {
    return StyleConfig("|", "|", "▓", "░", "▒");
}

inline StyleConfig PresetStyles::dots() {
    return StyleConfig("◢", "◣", "●", "○", "◑");
}

inline StyleConfig PresetStyles::squares() {
    return StyleConfig("[", "]", "■", "□", "▣");
}

}  // namespace style
}  // namespace cppbar