#pragma once

#include "bypixel.h"

struct NegativeTransform {
    RGB operator()(const RGB& rgb) const;
};

using NegativeFilter = ByPixelFilter<NegativeTransform>;
