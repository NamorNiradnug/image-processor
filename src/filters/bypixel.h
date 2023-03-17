#pragma once

#include "../filter.h"

// clang-format off
// i use clang-format 15 locally, and RequiresExpressionIndentation: OuterScope
// only available from clang-format 16
template <class T>
concept RGBTransformation = requires(T transform, const RGB& rgb) {
    { transform(rgb) } -> std::convertible_to<RGB>;
};
// clang-format on

/*!
 * Applies `transform` on every pixel.
 */
template <RGBTransformation Transform>
class ByPixelFilter : public Filter {
public:
    explicit ByPixelFilter(const Transform& transform = Transform()) : transform_(transform) {
    }

    void Apply(Image& image) const final {
        for (size_t x = 0; x < image.Width(); ++x) {
            for (size_t y = 0; y < image.Height(); ++y) {
                image.At(x, y) = transform_(image.Get(x, y));
            }
        }
    }

private:
    const Transform transform_;
};
