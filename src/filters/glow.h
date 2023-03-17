#pragma once

#include "../filter.h"
#include "gaussianblur.h"
#include "grayscale.h"

/*!
 * Makes light objects on image to "emit" light.
 */
class GlowFilter : public Filter {
public:
    explicit GlowFilter(double intensity);

    void Apply(Image& image) const override;

private:
    const GrayscaleFilter grayscale_;
    const GaussianBlur gaussian_blur_;
};
