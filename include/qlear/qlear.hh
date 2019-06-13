#pragma once

#include <functional>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <qlear/qtable.hh>

namespace qlear {

using State = unsigned int;
using Action = std::function<State(State)>;
using Reward = std::function<double(State, Action, State)>;

class Environment {
    State init;
    std::vector<Action> actions;
    Reward reward;
    QTable qtable;

public:
    Environment() = delete;
    Environment(State const &init, std::vector<Action> actions, Reward reward)
        : init(init), actions(actions), reward(reward), qtable() {}

    void train(unsigned int steps, double learning_rate, double discount,
            bool verbose);
    int evaluate(unsigned int steps, bool verbose) const;

private:
    double estimate_q(State state) const;
};

} // namespace clear
