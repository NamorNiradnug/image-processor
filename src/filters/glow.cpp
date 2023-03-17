#include "glow.h"

#include <cmath>

#include "gaussianblur.h"
#include "grayscale.h"

GlowFilter::GlowFilter(double intensity) : gaussian_blur_(intensity) {
}

void GlowFilter::Apply(Image& image) const {
    Image grayscaled = image;
    grayscale_.Apply(grayscaled);
    gaussian_blur_.Apply(image);

    for (size_t x = 0; x < image.Width(); ++x) {
        for (size_t y = 0; y < image.Height(); ++y) {
            image.At(x, y) *= std::max(1.0, grayscaled.Get(x, y).Luminance() / image.Get(x, y).Luminance());
            image.At(x, y).Normalize();
        }
    }
}
