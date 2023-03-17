#pragma once

#include "bypixel.h"

struct SerpiaTransform {
    RGB operator()(const RGB& rgb) const;
};

using SerpiaFilter = ByPixelFilter<SerpiaTransform>;
