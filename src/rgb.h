#pragma once

#include <cstdint>

struct RGB {
    double r = 0;
    double g = 0;
    double b = 0;

    /*!
     * Constructs RGB filled with zeros, that is, representing black color.
     */
    RGB() = default;

    /*!
     * Constructs RGB with all three component equal to `gray`. Represents a shade of gray.
     */
    inline explicit RGB(double gray);

    /*!
     * Do what you think it does.
     */
    inline RGB(double r, double g, double b);

    /*!
     * Constructs RGB from its discrete representation, normalizing components from `0-255` to `0.0-1.0`
     */
    RGB(uint8_t r, uint8_t g, uint8_t b);

    /*!
     * "Fixes" RGB by clamping all its components into `[0, 1]` interval.
     */
    void Normalize();

    [[nodiscard]] double Luminance() const;

    /*!
     * Multiplies all the components by `coef`.
     */
    inline RGB operator*(double coef) const;
    inline RGB& operator*=(double coef);
    inline RGB operator+(const RGB& other) const;
    inline RGB& operator+=(const RGB& other);

    inline static RGB Lerp(const RGB& left, const RGB& right, double t);
};

RGB::RGB(double gray) : RGB(gray, gray, gray) {
}

RGB::RGB(double r, double g, double b) : r(r), g(g), b(b) {
}

RGB RGB::operator*(double coef) const {
    return {r * coef, g * coef, b * coef};
}

RGB& RGB::operator*=(double coef) {
    r *= coef, g *= coef, b *= coef;
    return *this;
}

RGB RGB::operator+(const RGB& other) const {
    return {r + other.r, g + other.g, b + other.b};
}

RGB& RGB::operator+=(const RGB& other) {
    r += other.r, g += other.g, b += other.b;
    return *this;
}

inline RGB RGB::Lerp(const RGB& left, const RGB& right, double t) {
    return left * (1 - t) + right * t;
}

inline RGB operator*(double coef, const RGB& rgb) {
    return rgb * coef;
}
