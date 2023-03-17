#pragma once

#include "chain.h"
#include "matrixfilter.h"

class EdgeDetection : public FilterChain {
public:
    explicit EdgeDetection(double threshold);

private:
    static constexpr Matrix3x3 EdgeDetectionMatrix =
        Matrix3x3{std::array{0.0, -1.0, 0.0}, {-1.0, 4.0, -1.0}, {0.0, -1.0, 0.0}};
};
