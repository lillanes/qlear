#include <qlear/utils.hh>

#include <algorithm>
#include <functional>
#include <limits>

#include <qlear/qlear.hh>

namespace qlear {

Rng rng;

Rng::Rng() {
    std::mt19937::result_type entropy[std::mt19937::state_size];

    std::random_device rd;
    std::generate(std::begin(entropy), std::end(entropy), std::ref(rd));
    std::seed_seq seed(std::begin(entropy), std::end(entropy));

    engine.seed(seed);
}

double Rng::canonical() {
    return std::generate_canonical<double,
                                   std::numeric_limits<double>::digits>(engine);
}

size_t Rng::index(size_t length) {
    std::uniform_int_distribution<> dist(0, length - 1);
    return dist(engine);
}

} //namespace qlear
