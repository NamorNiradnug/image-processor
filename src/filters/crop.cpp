#include "crop.h"

Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
}

void Crop::Apply(Image& image) const {
    image.Resize(width_, height_);
}
