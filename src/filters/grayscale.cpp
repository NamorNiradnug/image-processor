#include "grayscale.h"

RGB GrayscaleTransfrom::operator()(const RGB& rgb) const {
    return RGB{rgb.Luminance()};
}
