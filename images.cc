#include "images.h"

/// Alternate returns true iff. x is in (let i = interval)
/// ... or [-3i, -2i) or [-i, 0) or [i, 2i) or [3i, 4i) or ... .
bool Detail::alternate(double x, double interval) {
    x += 2 * interval * (x < 0 ? (ceil(-x / interval / 2.0)) : 0);
    return (int) (x / interval) % 2;
}

/// Returns a function that scales every point in x axis by v.first and
/// in the y axis by v.second.
Base_image<Point> Detail::scale(const Vector &v) {
    return [=](const Point &p) {
        return (Point(p.first / v.first, p.second / v.second, p.is_polar));
    };
}

/// Returns a function that moves every point in x axis by v.first and
/// in the y axis by v.second.
Base_image<Point> Detail::translate(const Vector &v) {
    return [=](const Point &p) {
        return (Point(p.first - v.first, p.second - v.second, p.is_polar));
    };
}
/// Returns an Image that for points inside region is like this_way, for the
/// rest is like that_way.
Image cond(const Region &region, const Image &this_way, const Image &that_way) {
    return lerp(region, this_way, that_way);
}

/// Returns an Image that is a result of blending (with parameter blend)
/// this_way and that_way.
Image lerp(const Blend &blend, const Image &this_way, const Image &that_way) {
    return [=](const Point &p) {
        return this_way(p).weighted_mean(that_way(p), blend(p));
    };
}

/// Returns an Image that is a result of blending (with parameter blend)
/// image and black color.
Image darken(const Image &image, const Blend &blend) {
    return lerp(blend, image, constant(Colors::black));
}

/// Returns an Image that is a result of blending (with parameter blend)
/// image and white color.
Image lighten(const Image &image, const Blend &blend) {
    return lerp(blend, image, constant(Colors::white));
}