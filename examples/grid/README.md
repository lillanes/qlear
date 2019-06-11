# Grid

A basic grid pathfinding problem. The agent starts in a random location and must compute a policy that maximizes cumulative reward. Reward of 1 is given whenever the agent reaches (or remains in) a given goal location. The agent can try to move in any of the four cardinal directions, and can fail with low probability. Upon such failure, the agent moves instead in a different direction selected at random. If at any time the movement would lead the agent out of the grid, it will instead remain in the same location it was. In addition, the agent can choose to perform no action.

## Building

CMake handles finding the qlear library if it was installed to the default locations:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

If the library was installed to a custom location (using `CMAKE_INSTALL_PREFIX`) then you need:

```bash
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=<absolute/path/to/installed/library> ..
cmake --build .
```

Note that `CMAKE_PREFIX_PATH` must be an absolute path, even if `CMAKE_INSTALL_PREFIX` was given as a relative path.

## Running

By default, the example runs `1000` experiments with `1000` steps each:

```bash
./grid
```

Alternatively, you can run arbitrary number of experiments and steps:

```bash
./grid <experiments> <steps>
```

In addition, you can fix the RNG seed to replicate experiments exactly:

```bash
./grid <seed>
./grid <experiments> <steps> <seed>
```
