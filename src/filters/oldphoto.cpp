#include "oldphoto.h"

#include <random>

#include "fuzzyborder.h"
#include "gaussianblur.h"
#include "serpia.h"
#include "util.h"

OldPhotoFilter::OldPhotoFilter(size_t border_size, double sigma)
    : FilterChain(SerpiaFilter(), FuzzyBorderFilter(border_size), GaussianBlur(sigma)) {
}

void OldPhotoFilter::Apply(Image& image) const {
    static constexpr int MinGranulRadius = 5;
    static constexpr int MaxGranulRadius = 10;
    static constexpr size_t GranulsNumber = 25;

    FilterChain::Apply(image);

    std::default_random_engine rand_eng;
    std::uniform_int_distribution<size_t> x_gen(0, image.Width());
    std::uniform_int_distribution<size_t> y_gen(0, image.Height());
    std::uniform_int_distribution<int> radius_gen(MinGranulRadius, MaxGranulRadius);
    for (size_t _ = 0; _ != GranulsNumber; ++_) {
        AddGranul(x_gen(rand_eng), y_gen(rand_eng), radius_gen(rand_eng), RGB{1.0}, image);
    }
}
