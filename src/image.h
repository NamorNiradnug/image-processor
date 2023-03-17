#pragma once

#include <cstddef>
#include <vector>

#include "rgb.h"

class Image {
public:
    Image(size_t width, size_t height);

    [[nodiscard]] size_t Width() const;
    [[nodiscard]] size_t Height() const;
    void Resize(size_t width, size_t height);

    /*!
     * Returns RGB of pixel at `(x, y)` if `(x, y)` are valid coordinates of pixel
     * That is, `x` is in the interval `[0, width)` and `y` is in interval `[0, height)`
     *
     * Otherwise returns the RGB of the closest pixel.
     * Coordinate values larger than `std::numeric_limits<size_t>::max() / 2` assumed as negative.
     *
     * If image is empty (that is, at least one of `width` or `height` is zero) the behaviour is undefined.
     */
    [[nodiscard]] const RGB& Get(size_t x, size_t y) const;

    /*!
     * Reference to the RGB of pixel at `(x, y)`
     *
     * If `x` is not in the interval `[0, width)` or `y` is not in the interval `[0, height)` the behaviour is
     * undefined.
     */
    RGB& At(size_t x, size_t y);

private:
    size_t width_;
    size_t height_;
    std::vector<std::vector<RGB>> pixels_;
};
