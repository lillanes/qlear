#pragma once

#include <random>

namespace qlear {

namespace {

std::random_device rd;
std::mt19937 rng(rd());

} // namespace

template<typename T>
T uniform_choice(std::vector<T> const &v) {
    std::uniform_int_distribution<> dist(0, v.size() - 1);
    return v[dist(rng)];
}

} // namespace qlear
