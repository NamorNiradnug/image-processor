#include "util.h"

#include <cmath>

void AddGranul(size_t cx, size_t cy, int radius, const RGB& color, Image& image) {
    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            size_t x = cx + dx;
            size_t y = cy + dy;
            const double coef = std::hypot(dx, dy) / static_cast<double>(radius);
            if (coef <= 1 && x < image.Width() && y < image.Height()) {
                image.At(x, y) = RGB::Lerp(color, image.Get(x, y), coef);
            }
        }
    }
};
