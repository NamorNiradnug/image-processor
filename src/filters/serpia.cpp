#include "serpia.h"

RGB SerpiaTransform::operator()(const RGB& rgb) const {
    static const RGB BROWN = RGB{0.5, 0.35, 0.07};
    static const RGB WHITE = RGB{1.0};
    return RGB::Lerp(BROWN, WHITE, rgb.Luminance());
}
