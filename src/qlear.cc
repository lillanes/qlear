#include <qlear/qlear.hh>

#include <iostream>

namespace qlear {

void Environment::train(unsigned int steps, double learning_rate,
        double discount, bool verbose) {
    if (verbose) {
        std::cout << "Begin training!\n";
        std::cout << "  Starting at state " << init << ".\n";
    }

    State s0 = init;

    for (unsigned int step = 0; step < steps; ++step) {
        double max_q = qtable[{s0, 0}];
        size_t best_action_index = 0;
        for (size_t index = 1; index < actions.size(); ++index) {
            double q = qtable[{s0, index}];
            if (q > max_q) {
                max_q = q;
                best_action_index = index;
            }
        }
        Action best_action = actions[best_action_index];
        State s1 = best_action(s0);
        int r = reward(s0, best_action, s1);

        if (verbose) {
            std::cout << "  Applied action " << best_action_index << "\n";
            std::cout << "   Reached state " << s1 << "\n";
            std::cout << "   Obtained reward of " << r << "\n";
        }

        double q = qtable[{s0, best_action_index}];
        qtable[{s0, best_action_index}] = (1.0 - learning_rate) * q 
            + learning_rate * (r + discount * estimate_q(s1));

        if (verbose) {
            std::cout << "   New q-value for (" << s0 << ", "
                << best_action_index << "): "
                << qtable[{s0, best_action_index}] << " (was " << q << ")\n";
        }

        s0 = s1;
    }
}

int Environment::evaluate(unsigned int steps, bool verbose) const {
    if (verbose) {
        std::cout << "Begin evaluation!\n";
        std::cout << "  Starting at state " << init << ".\n";
    }
    int total_reward = 0;

    State s0 = init;
    for (unsigned int step = 0; step < steps; ++step) {
        auto it = qtable.find({s0, 0});
        double max_q = (it == qtable.end()) ? 0 : it->second;
        size_t best_action_index = 0;
        for (size_t index = 1; index < actions.size(); ++index) {
            it = qtable.find({s0, index});
            double q = (it == qtable.end()) ? 0 : it->second;
            if (q > max_q) {
                max_q = q;
                best_action_index = index;
            }
        }
        Action best_action = actions[best_action_index];
        State s1 = best_action(s0);
        int r = reward(s0, best_action, s1);
        s0 = s1;

        if (verbose) {
            std::cout << "  Applied action " << best_action_index << "\n";
            std::cout << "   Reached state " << s1 << "\n";
            std::cout << "   Obtained reward of " << r << "\n";
        }

        total_reward += r;
    }

    return total_reward;
}

double Environment::estimate_q(State state) const {
    auto it = qtable.find({state, 0});
    double max_q = (it == qtable.end()) ? 0 : it->second;
    for (size_t index = 1; index < actions.size(); ++index) {
        it = qtable.find({state, index});
        double q = (it == qtable.end()) ? 0 : it->second;
        if (q > max_q) {
            max_q = q;
        }
    }
    return max_q;
}

} // namespace qlear
