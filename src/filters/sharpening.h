#pragma once

#include "matrixfilter.h"

static constexpr auto SharpeningMatrix = Matrix3x3{std::array{0.0, -1.0, 0.0}, {-1, 5, -1}, {0, -1, 0}};

using SharpeningFilter = Matrix3x3Filter<SharpeningMatrix>;
