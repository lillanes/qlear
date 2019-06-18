#include <qlear/utils.hh>

#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <random>

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
    return std::generate_canonical<double, std::numeric_limits<double>::digits>(
            engine);
}

} // namespace qlear
