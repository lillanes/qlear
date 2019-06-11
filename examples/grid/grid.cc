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

void move_if_possible(Position &position, int dx, int dy) {
    if ((dx < 0 && position.x > 0) || (dx > 0 && position.x < WIDTH - 1)) {
        position.x += dx;
    }
    if ((dy < 0 && position.y > 0) || (dy > 0 && position.y < HEIGHT - 1)) {
        position.y += dy;
    }
}

void up(Position &position) {
    if (test_success()) {
        move_if_possible(position, 0, -1);
    } else {
        switch (choose(rng)) {
        case 0: move_if_possible(position, 1, 0); break;
        case 1: move_if_possible(position, 0, 1); break;
        case 2: move_if_possible(position, -1, 0); break;
        }
    }
}

void right(Position &position) {
    if (test_success()) {
        move_if_possible(position, 1, 0);
    } else {
        switch (choose(rng)) {
        case 0: move_if_possible(position, 0, 1); break;
        case 1: move_if_possible(position, -1, 0); break;
        case 2: move_if_possible(position, 0, -1); break;
        }
    }
}

void down(Position &position) {
    if (test_success()) {
        move_if_possible(position, 0, 1);
    } else {
        switch (choose(rng)) {
        case 0: move_if_possible(position, -1, 0); break;
        case 1: move_if_possible(position, 0, -1); break;
        case 2: move_if_possible(position, 1, 0); break;
        }
    }
}

void left(Position &position) {
    if (test_success()) {
        move_if_possible(position, -1, 0);
    } else {
        switch (choose(rng)) {
        case 0: move_if_possible(position, 0, -1); break;
        case 1: move_if_possible(position, 1, 0); break;
        case 2: move_if_possible(position, 0, 1); break;
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
        case 0: up(pos); break;
        case 1: right(pos); break;
        case 2: down(pos); break;
        case 3: left(pos); break;
        case 4: break; // no-op
        }
        reward += goal_reward(pos, goal);
    }
    if (verbose) {
        std::cout << "reward " << reward << "\n";
    }

    return reward;
}

#if 0
int qlear_agent(Position const &start, Position const &goal,
        unsigned int steps, bool verbose=false) {
    Position pos(start);

    if (verbose) {
        std::cout << "From " << pos << " to " << goal << " -- ";
    }

    qlear::Environment env(pos,
            {up, right, down, left, [](Position &){}},
            [&goal](Position const &, size_t, Position const &s1) {
                return goal_reward(s1, goal);
            }
            );

    qlear::train(env, steps, verbose);
    int eval_reward = qlear::evaluate(env, steps, verbose);
    return eval_reward;
}
#endif

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

    for (unsigned int i = 0; i < experiments; ++i) {
        Position pos(x(rng), y(rng));
        Position goal(x(rng), y(rng));

        random_reward += random_agent(pos, goal, steps);
    }
    std::cout << "Random agent:\n";
    std::cout << "  Total reward accrued: " << random_reward << "\n";
    std::cout << "  Average reward per experiment: "
        << (double) random_reward / experiments << "\n";
    std::cout << "  Average reward per step: "
        << (double) random_reward / (experiments * steps) << "\n";
}
