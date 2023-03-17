#pragma once

#include "bypixel.h"

struct GrayscaleTransfrom {
    RGB operator()(const RGB& rgb) const;
};

using GrayscaleFilter = ByPixelFilter<GrayscaleTransfrom>;
