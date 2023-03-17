#pragma once

#include "../filter.h"

class FuzzyBorderFilter : public Filter {
public:
    explicit FuzzyBorderFilter(size_t border_size);

    void Apply(Image& image) const override;

private:
    size_t border_size_;
};
