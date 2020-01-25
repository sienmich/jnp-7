#ifndef IMAGES_H
#define IMAGES_H

#include "functional.h"
#include "coordinate.h"
#include "color.h"

template<typename T>
using Base_image = std::function<T(const Point)>;

using Fraction = double;
using Region = Base_image<bool>;
using Image = Base_image<Color>;
using Blend = Base_image<Fraction>;

namespace Detail {
    bool alternate(double x, double interval);
}

template<typename T>
Base_image<T> constant(const T &t) {
    return [=]([[maybe_unused]]    const Point &p) { return t; };
}

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi) {
//    return compose(to_polar, lift(translate, compose(), constant(Vector(phi, 0))), from_polar);
    return [=](const Point &p) {
        Point polar{to_polar(p)};
        Point rotated{polar.first, polar.second - phi, true};
        return image(from_polar(rotated));
    };
}

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v) {
    return [=](const Point &p) {
        return image(Point(p.first - v.first, p.second - v.second));
    };
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s) {
    return [=](const Point &p) {
        return image(Point(p.first / s, p.second / s));
    };
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
    return [=](const Point &p) {
        return distance(p, q) <= r ? inner : outer;
    };
}

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way) {
    return [=](const Point &p) {
        return Detail::alternate(p.first, d) == Detail::alternate(p.second, d)
               ? this_way : that_way;
    };
}

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
    return [=](const Point &p) {
        Point polar{to_polar(p)};
        return checker(d, this_way, that_way)(
                Point(polar.first,
                      (Detail::alternate(
                              polar.second * n, 2 * M_PI)) ? d : 0));
    };
}

template<typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way) {
    return [=](const Point &p) {
        return Detail::alternate(distance(p, q), d) ? that_way : this_way;
    };
}

template<typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
    return [=](const Point &p) {
        return std::abs(p.first) * 2 <= d ? this_way : that_way;
    };
}

/*
template<typename T>
Base_image<T> constant(const T &t);

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi);

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v);

template<typename T>
Base_image<T> scale(Base_image<T> image, double s);

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer);

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way);

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way);

template<typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way);

template<typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way);


*/

Image cond(const Region &region, const Image &this_way, const Image &that_way);

Image lerp(const Blend &blend, const Image &this_way, const Image &that_way);

Image darken(const Image &image, const Blend &blend);

Image lighten(const Image &image, const Blend &blend);

#endif //IMAGES_H
