#include "image.h"

#include <cassert>
#include <limits>

Image::Image(size_t width, size_t height) : width_(width), height_(height), pixels_(width, std::vector(height, RGB{})) {
}

size_t Image::Width() const {
    return width_;
}

size_t Image::Height() const {
    return height_;
}

const RGB& Image::Get(size_t x, size_t y) const {
    if (x >= width_) [[unlikely]] {
        x = x > std::numeric_limits<size_t>::max() / 2 ? 0 : (width_ - 1);
    }
    if (y >= height_) [[unlikely]] {
        y = y > std::numeric_limits<size_t>::max() / 2 ? 0 : (height_ - 1);
    }
    return pixels_[x][y];
}

RGB& Image::At(size_t x, size_t y) {
    return pixels_[x][y];
}

void Image::Resize(size_t width, size_t height) {
    pixels_.resize(width);
    for (auto& coloumn : pixels_) {
        coloumn.resize(height);
    }
    width_ = width;
    height_ = height;
}
