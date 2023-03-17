#include "fuzzyborder.h"

#include <iostream>
#include <random>

#include "util.h"

FuzzyBorderFilter::FuzzyBorderFilter(size_t border_size) : border_size_(border_size) {
}

void FuzzyBorderFilter::Apply(Image& image) const {
    static constexpr size_t GranulsPerPixelMultiplier = 600;
    const int granul_radius = static_cast<int>((image.Width() + image.Height()) / 170 + border_size_);

    std::ranlux24 rand;
    std::normal_distribution<> offset(0.0, static_cast<double>(border_size_));

    std::uniform_int_distribution<size_t> gen_y(0, image.Height());
    for (size_t _ = 0; _ < (GranulsPerPixelMultiplier * image.Height()) / (border_size_ * granul_radius); ++_) {
        const auto cx = [&] {
            const double cx = offset(rand);
            return cx >= 0 ? static_cast<size_t>(cx) : static_cast<size_t>(static_cast<double>(image.Width()) + cx);
        }();
        const auto cy = gen_y(rand);
        AddGranul(cx, cy, granul_radius, RGB{1.0}, image);
    }

    std::uniform_int_distribution<size_t> gen_x(0, image.Height());
    for (size_t _ = 0; _ < (GranulsPerPixelMultiplier * image.Width()) / (border_size_ * granul_radius); ++_) {
        const auto cy = [&] {
            const double cy = offset(rand);
            return cy >= 0 ? static_cast<size_t>(cy) : static_cast<size_t>(static_cast<double>(image.Height()) + cy);
        }();
        const auto cx = gen_x(rand);
        AddGranul(cx, cy, granul_radius, RGB{1.0}, image);
    }
}
