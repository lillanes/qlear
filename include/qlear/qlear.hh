#pragma once

#include <qlear/qtable.hh>

#include <functional>
#include <vector>

namespace qlear {

using State = unsigned int;
using Action = std::function<State(State)>;
using Reward = std::function<double(State, Action, State)>;

struct Rate {
    double const value;

    Rate(double value) : value(value) {
        if (value < 0.0 || value > 1.0) {
            throw "Rate must be in [0, 1] range.";
        }
    }

    Rate operator=(double value) { return Rate(value); }
};

struct Environment {
    State init;
    std::vector<Action> actions;
    Reward reward;
};

class Agent {
    Environment const &env;
    Rate learning;
    Rate discount;
    Rate epsilon;
    QTable qtable;

public:
    Agent(Environment const &env, double learning_rate = 1.0,
          double discount_rate = 1.0, double epsilon = 0.0)
            : env(env), learning(learning_rate), discount(discount_rate),
              epsilon(epsilon) {}

    double train(unsigned int steps);
    double evaluate(unsigned int steps, bool discounted = false) const;

private:
    double estimate_q(State state) const;
};

} // namespace qlear
