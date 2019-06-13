#include <cstdlib>
#include <iostream>
#include <random>

#include <qlear/qlear.hh>

#define WIDTH 32
#define HEIGHT 32
#define SUCCESS_RATE 0.8

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_real_distribution<> dist(0.0, 1.0);
std::uniform_int_distribution<> choose(0,2);

struct Position {
    unsigned int x, y;

    Position() = default;
    Position(unsigned int x, unsigned int y)
        : x(x), y(y) {}
    ~Position() = default;

    // Default move constructor and assignment
    Position(Position &&) = default;
    Position & operator=(Position &&) = default;

    // Default copy constructor and assignment
    Position(Position const &) = default;
    Position & operator=(Position const &) = default;

    Position(qlear::State const &id)
        : x(id % WIDTH), y(id / WIDTH) {}

    qlear::State id() const {
        return WIDTH * y + x;
    }

    bool operator==(Position const &other) const {
        return x == other.x && y == other.y;
    }

    friend std::ostream& operator<<(std::ostream &stream,
            Position const &position) {
        stream << '(' << position.x << ", " << position.y << ')';
        return stream;
    }
};

bool test_success() {
    return (dist(rng) <= SUCCESS_RATE);
}

Position move_if_possible(Position position, int dx, int dy) {
    if ((dx < 0 && position.x > 0) || (dx > 0 && position.x < WIDTH - 1)) {
        position.x += dx;
    }
    if ((dy < 0 && position.y > 0) || (dy > 0 && position.y < HEIGHT - 1)) {
        position.y += dy;
    }
    return position;
}

Position up(Position const &position) {
    if (test_success()) {
        return move_if_possible(position, 0, -1);
    } else {
        switch (choose(rng)) {
        case 0: return move_if_possible(position, 1, 0);
        case 1: return move_if_possible(position, 0, 1);
        case 2: return move_if_possible(position, -1, 0);
        default: return position;
        }
    }
}

Position right(Position const &position) {
    if (test_success()) {
        return move_if_possible(position, 1, 0);
    } else {
        switch (choose(rng)) {
        case 0: return move_if_possible(position, 0, 1);
        case 1: return move_if_possible(position, -1, 0);
        case 2: return move_if_possible(position, 0, -1);
        default: return position;
        }
    }
}

Position down(Position const &position) {
    if (test_success()) {
        return move_if_possible(position, 0, 1);
    } else {
        switch (choose(rng)) {
        case 0: return move_if_possible(position, -1, 0);
        case 1: return move_if_possible(position, 0, -1);
        case 2: return move_if_possible(position, 1, 0);
        default: return position;
        }
    }
}

Position left(Position const &position) {
    if (test_success()) {
        return move_if_possible(position, -1, 0);
    } else {
        switch (choose(rng)) {
        case 0: return move_if_possible(position, 0, -1);
        case 1: return move_if_possible(position, 1, 0);
        case 2: return move_if_possible(position, 0, 1);
        default: return position;
        }
    }
}

int goal_reward(Position const &position, Position const &goal) {
    if (position == goal) {
        return 1;
    }
    return 0;
}

std::uniform_int_distribution<> uniform_action(0, 4);
int random_agent(Position const &start, Position const &goal,
        unsigned int steps, bool verbose=false) {
    Position pos(start);

    if (verbose) {
        std::cout << "From " << pos << " to " << goal << " -- ";
    }

    int reward = 0;
    for (unsigned int i = 0 ; i < steps; ++i) {
        switch (uniform_action(rng)) {
        case 0: pos = up(pos); break;
        case 1: pos = right(pos); break;
        case 2: pos = down(pos); break;
        case 3: pos = left(pos); break;
        case 4: break; // no-op
        }
        reward += goal_reward(pos, goal);
    }
    if (verbose) {
        std::cout << "reward " << reward << "\n";
    }

    return reward;
}

int qlear_agent(Position const &start, Position const &goal,
        unsigned int steps, bool verbose=false) {
    if (verbose) {
        std::cout << "From " << start << " to " << goal << "\n";
    }

    auto a_up = [&](qlear::State s){ return up(Position(s)).id(); };
    auto a_right = [&](qlear::State s){ return right(Position(s)).id(); };
    auto a_down = [&](qlear::State s){ return down(Position(s)).id(); };
    auto a_left = [&](qlear::State s){ return left(Position(s)).id(); };
    auto a_noop = [](qlear::State s){ return s; };

    std::vector<qlear::Action> actions({a_up, a_right, a_down, a_left, a_noop});

    auto reward = [&](qlear::State, qlear::Action, qlear::State s1) -> double {
        return goal_reward(Position(s1), goal);
    };

    qlear::Environment env(start.id(), actions, reward);

    env.train(steps, 0.9, 0.9, verbose);

    int eval_reward = env.evaluate(steps, verbose);

    return eval_reward;
}

int main(int const argc, char const **argv) {
    unsigned int experiments = 1000;
    unsigned int steps = 1000;

    if (argc == 2) {
        int seed = atoi(argv[1]);
        std::cout << "Using provided seed: " << seed << "\n";
        rng.seed(seed);
    } else if (argc == 3) {
        experiments = atoi(argv[1]);
        steps = atoi(argv[2]);
    } else if (argc == 4) {
        experiments = atoi(argv[1]);
        steps = atoi(argv[2]);

        int seed = atoi(argv[3]);
        std::cout << "Using provided seed: " << seed << "\n";
        rng.seed(seed);
    }
    std::cout << "Will run " << experiments << " independent experiments, with "
        << steps << " training steps each.\n";

    std::uniform_int_distribution<> x(0, WIDTH - 1);
    std::uniform_int_distribution<> y(0, HEIGHT - 1);

    int random_reward = 0;
    int qlear_reward = 0;

    for (unsigned int i = 0; i < experiments; ++i) {
        Position pos(x(rng), y(rng));
        Position goal(x(rng), y(rng));

        random_reward += random_agent(pos, goal, steps);
        qlear_reward += qlear_agent(pos, goal, steps, true);
    }
    std::cout << "Random agent:\n";
    std::cout << "  Total reward accrued: " << random_reward << "\n";
    std::cout << "  Average reward per experiment: "
        << (double) random_reward / experiments << "\n";
    std::cout << "  Average reward per step: "
        << (double) random_reward / (experiments * steps) << "\n";
    std::cout << "Qlear agent:\n";
    std::cout << "  Total reward accrued: " << qlear_reward << "\n";
    std::cout << "  Average reward per experiment: "
        << (double) qlear_reward / experiments << "\n";
    std::cout << "  Average reward per step: "
        << (double) qlear_reward / (experiments * steps) << "\n";
}
