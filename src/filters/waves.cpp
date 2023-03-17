#include "waves.h"

#include <cmath>
#include <iostream>

WavesFilter::WavesFilter(double amplitude, double period)
    : amplitude_(amplitude), frequency_(2 * std::numbers::pi / period) {
}

void WavesFilter::Apply(Image& image) const {
    const Image copy = image;
    for (size_t x = 0; x < image.Width(); ++x) {
        for (size_t y = 0; y < image.Height(); ++y) {
            const auto center_offset_x = static_cast<int>(x) - static_cast<int>(image.Width() / 2);
            const auto center_offset_y = static_cast<int>(y) - static_cast<int>(image.Height() / 2);
            const auto center_offset = std::hypot(center_offset_x, center_offset_y);
            const auto distort = amplitude_ * (std::sin(center_offset * frequency_) + 1.1) * amplitude_ * frequency_ *
                                 std::cos(center_offset * frequency_);
            image.At(x, y) =
                copy.Get(x + static_cast<int>(distort * static_cast<double>(center_offset_x) / center_offset),
                         y + static_cast<int>(distort * static_cast<double>(center_offset_y) / center_offset));
        }
    }
}
