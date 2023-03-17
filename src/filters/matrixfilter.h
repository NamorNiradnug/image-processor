#pragma once

#include <array>

#include "../filter.h"

using Matrix3x3 = std::array<std::array<double, 3>, 3>;

/*!
 * Convolution of an image and 3 by 3 matrix.
 *
 * See https://en.wikipedia.org/wiki/Kernel_(image_processing) for more information
 */
template <Matrix3x3 Matrix>
class Matrix3x3Filter : public Filter {
public:
    void Apply(Image& image) const final {
        const Image copy = image;
        for (size_t x = 0; x < image.Width(); ++x) {
            for (size_t y = 0; y < image.Height(); ++y) {
                image.At(x, y) = {};
                for (size_t dx = -1; dx != 2; ++dx) {
                    for (size_t dy = -1; dy != 2; ++dy) {
                        image.At(x, y) += copy.Get(x + dx, y + dy) * Matrix[dx + 1][dy + 1];
                    }
                }
                image.At(x, y).Normalize();
            }
        }
    }
};
