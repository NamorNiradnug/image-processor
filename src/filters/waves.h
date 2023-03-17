#pragma once

#include "../filter.h"

class WavesFilter : public Filter {
public:
    WavesFilter(double amplitude, double period);

    void Apply(Image& image) const override;

private:
    double amplitude_ = 100.0;
    double frequency_;
};
