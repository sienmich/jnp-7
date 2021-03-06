#ifndef IMAGES_H
#define IMAGES_H

#include "functional.h"
#include "coordinate.h"
#include "color.h"

/// Base_image<T> type is representing functions from Points to T.
template<typename T>
using Base_image = std::function<T(const Point)>;

/// Fraction type is representing numbers in [0, 1].
using Fraction = double;

/// Region type is representing functions from Points to bool,
/// which corresponds to some region of the plane.
using Region = Base_image<bool>;

/// Image type is representing functions from Points to Color,
/// which means it is coloring of the plane - the image itself.
using Image = Base_image<Color>;

/// Blend type is representing functions from Points to Fractions,
/// which could be used to blend two Images.
using Blend = Base_image<Fraction>;

/// Namespace Detail contains some additional helper functions.
namespace Detail {
    /// Alternate returns true iff. x is in (let i = interval)
    /// ... or [-3i, -2i) or [-i, 0) or [i, 2i) or [3i, 4i) or ... .
    constexpr bool alternate(double x, double interval) {
        x += 2 * interval * (x < 0 ? (ceil(-x / interval / 2.0)) : 0);
        return (int) (x / interval) % 2;
    }

    /// Returns a function that scales every point in x axis by v.first and
    /// in the y axis by v.second.
    inline Base_image<Point> scale(const Vector &v) {
        return [=](const Point p) {
            return (Point(p.first / v.first, p.second / v.second, p.is_polar));
        };
    }

    /// Returns a function that moves every point in x axis by v.first and
    /// in the y axis by v.second.
    inline Base_image<Point> translate(const Vector &v) {
        return [=](const Point p) {
            return (Point(p.first - v.first, p.second - v.second, p.is_polar));
        };
    }
}

/// Returns a function that for every point returns t.
template<typename T>
Base_image<T> constant(const T &t) {
    return [=]([[maybe_unused]] const Point &p) { return t; };
}

/// Translates whole image by Vector v.
template<typename T>
Base_image<T> translate(const Base_image<T> &image, const Vector &v) {
    return compose(Detail::translate(v), image);
}

/// Rotates whole image by angle phi.
template<typename T>
Base_image<T> rotate(const Base_image<T> &image, double phi) {
    return compose(to_polar,
                   Detail::translate(Vector(0, phi)),
                   from_polar,
                   image);
}

/// Scales whole image by s.
template<typename T>
Base_image<T> scale(const Base_image<T> &image, double s) {
    return compose(Detail::scale(Vector(s, s)), image);
}

/// Returns function that for every point inside a circle(center: q, radius: r)
/// returns inner; outer for other points.
template<typename T>
Base_image<T> circle(const Point &q, double r, const T &inner, const T &outer) {
    return [=](const Point p) {
        return distance(p, q) <= r ? inner : outer;
    };
}

/// Returns function that for every point inside a black field of a
/// chessboard(field length: d) returns this_way; that_way for other points.
template<typename T>
Base_image<T> checker(double d, const T &this_way, const T &that_way) {
    return [=](const Point p) {
        return Detail::alternate(p.first, d) == Detail::alternate(p.second, d)
               ? this_way : that_way;
    };
}

/// Returns function that for every point inside a black field of a
/// polar_chessboard(field length: d, rays: n) returns this_way;
/// that_way for other points. See "polar_checker.bmp" for better explanation.
template<typename T>
Base_image<T>
polar_checker(double d, int n, const T &this_way, const T &that_way) {
    return compose(to_polar,
                   Detail::scale(Vector(d, 2 * M_PI / n)),
                   checker(1, this_way, that_way));
}

/// Returns function that for every point inside a black field of a
/// ring_pattern(center: q, one ring width: d) returns this_way;
/// that_way for other points. See "rings.bmp" for better explanation.
template<typename T>
Base_image<T> rings(Point q, double d, const T &this_way, const T &that_way) {
    return compose(Detail::translate(Vector(q.first, q.second)),
                   polar_checker(d, 1, this_way, that_way));
}

/// Returns function that for every point inside a
/// vertical stripe (center: (0, 0), width: d) returns this_way;
/// that_way for other points.
template<typename T>
Base_image<T> vertical_stripe(double d, const T &this_way, const T &that_way) {
    return [=](const Point p) {
        return std::abs(p.first) * 2 <= d ? this_way : that_way;
    };
}

/// Returns an Image that is a result of blending (with parameter blend)
/// this_way and that_way.
inline Image
lerp(const Blend &blend, const Image &this_way, const Image &that_way) {
    return [=](const Point p) {
        return this_way(p).weighted_mean(that_way(p), blend(p));
    };
}

/// Returns an Image that for points inside region is like this_way, for the
/// rest is like that_way.
inline Image
cond(const Region &region, const Image &this_way, const Image &that_way) {
    return lerp(region, this_way, that_way);
}

/// Returns an Image that is a result of blending (with parameter blend)
/// image and black color.
inline Image darken(const Image &image, const Blend &blend) {
    return lerp(blend, image, constant(Colors::black));
}

/// Returns an Image that is a result of blending (with parameter blend)
/// image and white color.
inline Image lighten(const Image &image, const Blend &blend) {
    return lerp(blend, image, constant(Colors::white));
}

#endif //IMAGES_H