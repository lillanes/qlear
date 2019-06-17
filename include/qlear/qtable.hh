#pragma once

#include <cstddef>
#include <unordered_map>

namespace qlear {

using State = unsigned int;
using ActionIndex = std::size_t;

struct StateActionPair {
    State state;
    ActionIndex action;

    bool operator==(StateActionPair const &other) const {
        return this->action == other.action && this->state == other.state;
    }
};

using QTable = std::unordered_map<StateActionPair, double>;

} // namespace qlear

namespace std {

// hash specialization is needed to use StateActionPair in an unordered_map
template<>
struct hash<qlear::StateActionPair> {
    std::size_t operator()(qlear::StateActionPair const &sap) const noexcept {
        hash<qlear::State> state_hasher;
        hash<qlear::ActionIndex> action_hasher;

        // based on Boost's hash_combine
        std::size_t hash = state_hasher(sap.state) + 0x9e3779b9;
        hash ^= action_hasher(sap.action) + 0x9e3779b9 + (hash<<6) + (hash>>2);
        return hash;
    }
};

} //namespace std
