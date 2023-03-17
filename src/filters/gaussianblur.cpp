#include "gaussianblur.h"

#include <cmath>
#include <numbers>

GaussianBlur::GaussianBlur(double sigma) {
    const size_t coeffs_size = static_cast<size_t>(std::ceil(6 * std::abs(sigma))) | 1;
    const size_t coeffs_size_half = coeffs_size / 2;
    coeffs_.resize(coeffs_size);
    for (int dx = 0; dx <= static_cast<int>(coeffs_size_half); ++dx) {
        coeffs_[coeffs_size_half - dx] = coeffs_[coeffs_size_half + dx] =
            std::exp(-static_cast<double>(dx * dx) / (2 * sigma * sigma)) /
            std::sqrt(2 * std::numbers::pi * sigma * sigma);
    }
}

void GaussianBlur::Apply(Image& image) const {
    const auto coeffs_size_half = static_cast<int>(coeffs_.size() / 2);

    Image after_vertical_iter(image.Width(), image.Height());
    for (size_t x = 0; x < image.Width(); ++x) {
        for (size_t y = 0; y < image.Height(); ++y) {
            RGB& pixel = after_vertical_iter.At(x, y);
            for (int dy = -coeffs_size_half; dy <= coeffs_size_half; ++dy) {
                pixel += image.Get(x, y + dy) * coeffs_[dy + coeffs_size_half];
            }
        }
    }

    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            RGB& pixel = image.At(x, y) = {};
            for (int dx = -coeffs_size_half; dx <= coeffs_size_half; ++dx) {
                pixel += after_vertical_iter.Get(x + dx, y) * coeffs_[dx + coeffs_size_half];
            }
        }
    }
}
