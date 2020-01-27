#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <functional>

/// Compose() returns identity function.
constexpr auto compose() {
    return [](auto x) { return x; };
}

/// Compose(f1, f2, f3, ...) composes all its arguments, which means it returns
/// function that for x returns ...f3(f2(f1(x))).
constexpr auto compose(auto f, auto ...g) {
    return [=](auto x) { return compose(g...)(f(x)); };
}

/// Lift(h) returns function that for every p (any type; value ignored)
/// returns h().
constexpr auto lift(auto h) {
    return [=]([[maybe_unused]] auto p) { return h(); };
}

/// Lift(h, f1, f2, ...) returns function that for every p
/// returns h(f1(p), f2(p), ...).
constexpr auto lift(auto h, auto f1, auto ...f2) {
    return [=](auto p) {
        return lift([=](auto ...a2) { return h(f1(p), a2...); }, f2...)(p);
    };
}

#endif //FUNCTIONAL_H
