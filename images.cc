#include "images.h"

bool Detail::alternate(double x, double interval) {
    x += 2 * interval * (x < 0 ? (ceil(-x / interval / 2.0)) : 0);
    return (int) (x / interval) % 2;
}

Image cond(const Region &region, const Image &this_way, const Image &that_way) {
    return [=](const Point &p) {
        return region(p) ? this_way(p) : that_way(p);
    };
}

Image lerp(const Blend &blend, const Image &this_way, const Image &that_way) {
    return [=](const Point &p) {
        return this_way(p).weighted_mean(that_way(p), blend(p));
    };
}

Image darken(const Image &image, const Blend &blend) {
    return lerp(blend, image, constant(Colors::black));
}

Image lighten(const Image &image, const Blend &blend) {
    return lerp(blend, image, constant(Colors::white));
}


