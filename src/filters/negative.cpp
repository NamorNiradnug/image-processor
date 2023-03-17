#include "negative.h"

RGB NegativeTransform::operator()(const RGB& rgb) const {
    return {1.0 - rgb.r, 1.0 - rgb.g, 1.0 - rgb.b};
}
