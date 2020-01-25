#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <functional>

inline auto compose() {
    return [](auto x) { return x; };
}

inline auto compose(auto f, auto ...g) {
    return [f, g...](auto x) { return compose(g...)(f(x)); };
}



auto lift(auto h) {
    return [h]([[maybe_unused]] auto p) { return h(); };
}

auto lift(auto h, auto f) {
    return [h, f](auto p) {
        return h(f(p));
    };
}

auto lift(auto h, auto f, auto g) {
    return [h, f, g](auto p) {
        return h(f(p), g(p));
    };
}

/*

    auto lift(H h, Fs... fs) – realizuje „pointwise lifting” w następujący spo-
    sób:
      lift(h)
        a -> (p -> a)
        h
        Uwaga: h zwraca tutaj a.
      lift(h, f1)
        (a -> b) -> (p -> a) -> (p -> b)
           h           f1
      lift(h, f1, f2)
        (a -> b -> g) -> (p -> a) -> (p -> b) -> (p -> g)
              h             f1          f2
      ...
      lift(h, f1, ..., fn)
        (a1 -> ... -> an -> d) -> (p -> a1) -> ... -> (p -> an) -> (p -> d)
                  h                  f1                  fn
      ...

      Celem  wyjaśnienia   bardziej  szczegółowo  zostanie   opisany  przypadek
      lift(h, f1, f2)
        (a -> b -> g) -> (p -> a) -> (p -> b) -> (p -> g)
              h             f1          f2
      - Funkcja h przyjmuje argumenty a oraz b i zwraca wartość g.
      - Funkcja f1 przyjmuje argument p i zwraca wartość a,  którą lift umiesz-
        cza jako pierwszy argument faktyczny funkcji h.
      - Funkcja f2  przyjmuje argument p i zwraca wartość b, którą lift umiesz-
        cza jako drugi argument faktyczny funkcji h.
      - Funkcja lift przyjmuje argumenty h, f1 oraz f2 i zwraca funkcję,  która
        przyjmuje argument p i zwraca wartość g.
      Nazwa „pointwise lifting” powinna w tym momencie wydawać się już bardziej
      naturalna.

 */



#endif //FUNCTIONAL_H
