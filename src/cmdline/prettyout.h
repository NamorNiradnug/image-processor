#pragma once

#include <ostream>
#include <sstream>

namespace color {
static constexpr char Black[] = "\u001b[30m";
static constexpr char Red[] = "\u001b[31m";
static constexpr char Green[] = "\u001b[32m";
static constexpr char Yellow[] = "\u001b[33m";
static constexpr char Blue[] = "\u001b[34m";
static constexpr char Magenta[] = "\u001b[35m";
static constexpr char Cyan[] = "\u001b[36m";
static constexpr char White[] = "\u001b[37m";
static constexpr char Reset[] = "\u001b[0m";
}  // namespace color

// clang-format off
template <typename T>
concept SupportOStream = requires(T t, std::ostream& out) {
    { out << t } -> std::convertible_to<std::ostream&>;
};
// clang-format on

template <const char*>
struct Colored {
    const char* text;

    explicit operator std::string() {
        return (std::ostringstream() << *this).str();
    }
};

using FilterColor = Colored<color::Green>;
using FileColor = Colored<color::Cyan>;
using ArgColor = Colored<color::Magenta>;

template <const char* Color>
std::ostream& operator<<(std::ostream& out, const Colored<Color>& colored) {
    return out << Color << colored.text << color::Reset;
}
