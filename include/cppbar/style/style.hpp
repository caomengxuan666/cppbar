#pragma once

#include <string>

namespace cppbar {
namespace style {

struct StyleConfig {
    std::string left_bracket;
    std::string right_bracket;
    std::string filled_char;
    std::string empty_char;
    std::string tip_char;

    StyleConfig(const std::string& lb, const std::string& rb, const std::string& fc,
                const std::string& ec, const std::string& tc)
        : left_bracket(lb), right_bracket(rb), filled_char(fc), empty_char(ec), tip_char(tc) {}
};

class Style {
public:
    enum class Type { Unicode, Ascii, Braille };

    Style() : type_(Type::Unicode) {}

    explicit Style(Type type) : type_(type) {}

    StyleConfig get_config() const;

    void set_type(Type type) { type_ = type; }
    Type get_type() const { return type_; }

    static Style unicode() { return Style(Type::Unicode); }
    static Style ascii() { return Style(Type::Ascii); }
    static Style braille() { return Style(Type::Braille); }

private:
    Type type_;
};

struct PresetStyles {
    static StyleConfig classic();
    static StyleConfig modern();
    static StyleConfig minimal();
    static StyleConfig blocks();
    static StyleConfig dots();
    static StyleConfig squares();
};

}  // namespace style
}  // namespace cppbar