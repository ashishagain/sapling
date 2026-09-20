# Roadmap

Work through the milestones in order. Each one has a test suite. Your job is to make it pass.

Every unimplemented function throws `Not implemented yet: <Function>`, so failing tests tell you
exactly where to go next. Delete the `SAPLING_TODO(...)` line when you implement a function.

```bash
cmake -S sapling_core -B build -DCMAKE_BUILD_TYPE=Debug   # once
cmake --build build -j && ./build/sapling_tests --gtest_filter='M1*'
```

Tip: turn on sanitizers (`-DSAPLING_SANITIZE=ON`) from the start. Behavior trees are pointer-heavy
and ASan will catch mistakes early.

**Commit after every milestone** with a meaningful message. A clean history is part of what makes
this look good on GitHub.

---

## M0: Theory (an evening)

Before writing code, make sure you can answer these:

- What does "ticking" a tree mean? Why is a tick not the same as a function call that runs until done?
- What do `Running`, `Success`, `Failure` mean? Who decides?
- What's the difference between a Sequence and a Fallback? Draw the "enter the room" tree from
  `sapling_core/examples/enter_room.cpp` on paper.
- Why do robots use BTs instead of finite state machines? (Hint: think about adding one new
  behaviour to a 20-state FSM.)

Reading: the introduction pages of the [BehaviorTree.CPP docs](https://www.behaviortree.dev/) and
chapter 1 of Colledanchise & Ögren (free preprint on arXiv: search "Behavior Trees in Robotics and
AI: An Introduction").

## M1: Node, Blackboard, leaves

Files: `blackboard.hpp`, `node.cpp`, `leaves.cpp`, `CompositeNode::setBlackboard` in `composite.cpp`

Tests: `M1*`

Concepts: the template-method pattern (`tick()` → `onTick()`), `std::any`, `std::optional`,
`std::function`.

Check yourself: why does `halt()` only call `onHalt()` when the node is `Running`?

## M2: Sequence and Fallback

Files: `composite.cpp` (`haltChildren`, `Sequence`, `Fallback`)

Tests: `M2*`

Check yourself: in `ResumesFromRunningChild`, the first child is not re-ticked. When is that what
you want, and when is it dangerous? (You'll get the answer in M4.)

## M3: Decorators

Files: `decorators.cpp`

Tests: `M3*`

Check yourself: `Retry` loops *inside a single tick*. What would happen if the child were a
`Condition` that always fails and `max_attempts` were 1,000,000? Is that a problem for a
real-time robot?

## M4: Reactive nodes and Parallel

Files: `composite.cpp` (`ReactiveSequence`, `ReactiveFallback`, `Parallel`)

Tests: `M4*`

This is the most important milestone conceptually. Reactivity is the reason robots use BTs.

Check yourself: write down a situation where a ReactiveSequence is correct and a Sequence is a bug,
and vice versa.

## M5: Tree runner and the example

Files: `tree.cpp`, `childNodes()` in `composite.cpp` and `decorators.cpp`

Tests: `M5*` (and now **everything** should be green: `ctest --test-dir build`)

Then run `./build/enter_room_example` and check the output makes sense.

## M6: ROS 2 turtlesim patrol

Files: `sapling_turtle/src/behaviors.cpp`. The ROS node (`patrol_node.cpp`) is already written;
read it and understand how it maps topics onto the blackboard.

1. Implement `Recharge`, then `MoveTo` (a proportional controller: turn towards the goal, then drive).
2. Build the tree in `buildPatrolTree` following the diagram in `behaviors.hpp`.
3. `colcon build`, launch, and watch the turtle patrol, run low, recharge and resume.
4. Answer the three questions in `behaviors.hpp` in `docs/DESIGN.md`.
5. Record a GIF and put it in the README.

Experiments:
- Make the root a plain `Sequence`. What happens when the battery runs out?
- Delete the body of `MoveTo::onHalt()`. What happens?
- Add unit tests for `MoveTo` with a fake `TurtleIO` that records velocities, with no ROS required.

## M7: Stretch goals (pick any, each is a good CV talking point)

- **XML trees.** Load trees from XML (tinyxml2 via FetchContent) with a node factory
  (`factory.registerNode<MoveTo>("MoveTo")`). Change the robot's behaviour without recompiling.
- **Ports.** Instead of hard-coded blackboard keys, give nodes input/output ports that are remapped
  to keys in XML, like BehaviorTree.CPP's `{target}` syntax.
- **Observer / logger.** Hook into `tick()` to record status transitions; print a live coloured tree
  in the terminal, or publish it on a ROS topic.
- **Timeout decorator** with an injectable clock so it's testable without sleeping.
- **Async actions.** Wrap a ROS 2 action client (e.g. `turtlesim/action/RotateAbsolute`) in a leaf
  that returns `Running` while the goal is active and cancels it in `onHalt()`.
- **Benchmark.** Google Benchmark the tick cost of a 1,000-node tree and optimise it.
- **Compare.** Re-implement the patrol in BehaviorTree.CPP and write up the differences in
  `docs/DESIGN.md`. This is a great bridge to Nav2, whose navigator is a BehaviorTree.CPP tree.
