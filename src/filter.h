#pragma once

#include "image.h"

class Filter {
public:
    virtual ~Filter() = default;

    /*!
     * Applies the filter to the `image`.
     */
    virtual void Apply(Image& image) const = 0;
};
