#pragma once

#include "chain.h"

class OldPhotoFilter : public FilterChain {
public:
    OldPhotoFilter(size_t border_size, double sigma);
    void Apply(Image& image) const override;
};
