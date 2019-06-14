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

    template <typename T>
    T uniform_choice(std::vector<T> const &v) {
        std::uniform_int_distribution<> dist(0, v.size() - 1);
        return v[dist(engine)];
    }

    double canonical();

    size_t index(size_t length);

};

extern Rng rng;

} // namespace qlear
