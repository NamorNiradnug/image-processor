#pragma once

#include <cstddef>

template <size_t N>
struct FixedString {
    char buf_[N + 1]{};
    constexpr FixedString(const char* s) {  // NOLINT
        for (size_t i = 0; i != N; ++i) {
            buf_[i] = s[i];
        }
    }
    constexpr operator const char*() const {  // NOLINT
        return buf_;
    }
};
template <size_t N>
FixedString(char const (&)[N]) -> FixedString<N - 1>;
