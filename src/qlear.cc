#include <qlear/qlear.hh>

#include <iostream>
#include <random>

#include <qlear/utils.hh>

namespace qlear {

bool test_epsilon(Rate const &epsilon) {
    if (epsilon.value > 0.0) {
        return epsilon.value > rng.canonical();
    }
    return false;
}

double Agent::train(unsigned int steps) {
#ifdef VERBOSE
    std::cout << "Begin training!\n";
    std::cout << "  Starting at state " << init << ".\n";
#endif

    double total_reward = 0.0;
    State s0 = env.init;

    for (unsigned int step = 0; step < steps; ++step) {


        double r = 0.0;
        size_t selected_action_index;

        if (test_epsilon(epsilon)) {
            selected_action_index = rng.uniform_index(env.actions);
        } else {
            double max_q = qtable[{s0, 0}];
            std::vector<size_t> best_actions = {0};
            for (size_t index = 1; index < env.actions.size(); ++index) {
                double q = qtable[{s0, index}];
                if (q > max_q) {
                    max_q = q;
                    best_actions = {index};
                } else if (q == max_q) {
                    best_actions.push_back(index);
                }
            }
            selected_action_index = rng.uniform_choice(best_actions);
        }

        Action selected_action = env.actions[selected_action_index];
        State s1 = selected_action(s0);
        r = env.reward(s0, selected_action, s1);
        total_reward += r;

#ifdef VERBOSE
        std::cout << "  Applied action " << selected_action_index << "\n";
        std::cout << "   Reached state " << s1 << "\n";
        std::cout << "   Obtained reward of " << r << "\n";
#endif

        double q = qtable[{s0, selected_action_index}];
        qtable[{s0, selected_action_index}] = (1.0 - learning.value) * q
            + learning.value * (r + discount.value * estimate_q(s1));

#ifdef VERBOSE
        std::cout << "   New q-value for (" << s0 << ", "
            << selected_action_index << "): "
            << qtable[{s0, selected_action_index}] << " (was " << q << ")\n";
#endif

        s0 = s1;
    }

    return total_reward;
}

double Agent::evaluate(unsigned int steps, bool discounted) const {
#ifdef VERBOSE
    std::cout << "Begin evaluation!\n";
    std::cout << "  Starting at state " << init << ".\n";
#endif

    double total_reward = 0.0;
    double current_discount = 1.0;
    State s0 = env.init;

    for (unsigned int step = 0; step < steps; ++step) {
        auto it = qtable.find({s0, 0});
        double max_q = (it == qtable.end()) ? 0 : it->second;
        std::vector<size_t> best_actions = {0};
        for (size_t index = 1; index < env.actions.size(); ++index) {
            it = qtable.find({s0, index});
            double q = (it == qtable.end()) ? 0 : it->second;
            if (q > max_q) {
                max_q = q;
                best_actions = {index};
            } else if (q == max_q) {
                best_actions.push_back(index);
            }
        }
        size_t best_action_index = rng.uniform_choice(best_actions);
        Action best_action = env.actions[best_action_index];
        State s1 = best_action(s0);
        double r = env.reward(s0, best_action, s1);
        s0 = s1;

#ifdef VERBOSE
        std::cout << "  Applied action " << best_action_index << "\n";
        std::cout << "   Reached state " << s1 << "\n";
        std::cout << "   Obtained reward of " << r << "\n";
#endif

        total_reward += current_discount * r;
        if (discounted) {
            current_discount *= discount.value;
        }
    }

    return total_reward;
}

double Agent::estimate_q(State state) const {
    auto it = qtable.find({state, 0});
    double max_q = (it == qtable.end()) ? 0 : it->second;
    for (size_t index = 1; index < env.actions.size(); ++index) {
        it = qtable.find({state, index});
        double q = (it == qtable.end()) ? 0 : it->second;
        if (q > max_q) {
            max_q = q;
        }
    }
    return max_q;
}

} // namespace qlear
