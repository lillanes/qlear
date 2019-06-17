#pragma once

#include <random>

namespace qlear {

class Rng {
    std::mt19937 engine;
public:
    Rng();

    template <typename S>
    Rng(S seed)
        : engine(seed) {}

    template <typename C, typename S = typename C::size_type>
    S uniform_index(C const &c) {
        std::uniform_int_distribution<> dist(0, c.size() - 1);
        return dist(engine);
    }

    template <typename C, typename E = typename C::value_type>
    E uniform_choice(C const &c) {
        return c[uniform_index(c)];
    }

    double canonical();

};

extern Rng rng;

} // namespace qlear
