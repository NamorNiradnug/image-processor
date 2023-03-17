#pragma once

#include "../filter.h"

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(double sigma);

    void Apply(Image& image) const final;

private:
    std::vector<double> coeffs_;
};
