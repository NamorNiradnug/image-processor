#include "rgb.h"

#include <algorithm>
#include <numeric>

RGB::RGB(uint8_t r, uint8_t g, uint8_t b)
    : r(static_cast<double>(r) / std::numeric_limits<uint8_t>::max()),
      g(static_cast<double>(g) / std::numeric_limits<uint8_t>::max()),
      b(static_cast<double>(b) / std::numeric_limits<uint8_t>::max()) {
}

void RGB::Normalize() {
    r = std::clamp(r, 0.0, 1.0), g = std::clamp(g, 0.0, 1.0), b = std::clamp(b, 0.0, 1.0);
}

double RGB::Luminance() const {
    static constexpr double RedCoef = 0.299;
    static constexpr double GreenCoef = 0.587;
    static constexpr double BlueCoef = 0.114;
    return RedCoef * r + GreenCoef * g + BlueCoef * b;
}
