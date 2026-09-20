# Roadmap

## What you're building

**A small C++ library that lets a robot make decisions, plus a demo robot that uses it.**

A robot program constantly has to decide what to do next: "if the battery is low, go charge;
otherwise keep patrolling." A *behavior tree* is a tidy way to write those decisions as a tree of
small pieces instead of a tangle of `if`/`else`. You are writing the engine that runs such trees.
Real robotics software (Nav2, MoveIt 2) uses the same idea, so you'll be building a mini version of
something used in industry.

There are three pieces, and the milestones build them in this order:

| Piece | What it is | Milestones |
|---|---|---|
| **The engine** (`sapling_core/`) | The library: nodes, the shared notepad (blackboard), Sequence/Fallback/etc., and a `Tree` that runs them. Plain C++, no robot needed. | M1–M4 |
| **The example** (`enter_room.cpp`) | A tiny program that builds a tree with your engine and runs it in the terminal. This is your first proof it works. | M5 |
| **The robot demo** (`sapling_turtle/`) | A simulated turtle (ROS 2 turtlesim) that drives between waypoints, and when its battery runs low, stops, drives to a charger, recharges, and resumes. | M6 |

### What "done" looks like

**After M5**, this program (`./build/enter_room_example`) uses your engine to get a robot through a
locked door: check if the door is open, else try the handle, else pick the lock (retrying up to 5
times), then walk in. It prints something like:

```
Tree:
Root
  GetDoorOpen
    IsDoorOpen
    OpenDoor
    UnlockThenOpen
      Retry(5)
        PickLock
      OpenDoor
  EnterRoom

Running:
  OpenDoor: it's locked
  PickLock: attempt 1 failed
  PickLock: attempt 2 failed
  PickLock: click!
  OpenDoor: opened
  EnterRoom: I'm in!

Result: Success
```

The top half is the tree drawn as text (children indented under their parent); the bottom half is
the robot's decisions, in order. Every line there comes from a function you wrote.

**After M6**, you launch turtlesim and watch the turtle loop around four waypoints. Its battery
drains as it moves. When it drops below 25%, the turtle abandons its patrol mid-drive, goes to the
charger, recharges, and starts patrolling again. The tree behind it:

```
Root (ReactiveSequence)          <- re-checks the battery on EVERY tick
├── EnsureBattery (Fallback)     <- "battery OK? if not, go recharge"
│   ├── BatteryOk
│   └── GoRecharge (Sequence)
│       ├── MoveTo charger
│       └── Recharge
└── Patrol (Sequence)            <- visit the waypoints in order
    ├── MoveTo wp0
    ├── MoveTo wp1
    ├── MoveTo wp2
    └── MoveTo wp3
```

You'll end up with a working library, a passing test suite, and a GIF of the turtle for your README.

## How to work through it

Do the milestones in order. Each one has tests, and your job is to make them pass.

1. Run the tests for the milestone. They fail with `Not implemented yet: <function name>`.
2. Open the file for that function and find the `SAPLING_TODO(...)` line. The comment above it
   says what to write. Replace that line with your code.
3. Re-run the tests. Repeat until the milestone is green, then commit.

```bash
cmake -S sapling_core -B build -DCMAKE_BUILD_TYPE=Debug   # once
cmake --build build -j && ./build/sapling_tests --gtest_filter='M1*'
```

`--gtest_filter='M1*'` runs only the M1 tests. Change it to `M2*`, `M3*`, and so on as you go.

**The hints in the source files get thinner as you go, on purpose.** M1 explains everything,
including the C++ syntax. By M5 and M6 you get a goal and a list of names.

Tip: turn on sanitizers (`-DSAPLING_SANITIZE=ON`) from the start. They catch pointer mistakes
early, with a clear error message.

**Commit after every milestone** with a meaningful message. A clean history looks good on GitHub.

---

## M0: Theory (an evening)

No code for this one, just paper and a pen.

**Why it matters:** the code in M1 to M4 is just these ideas written down. If they're clear, the
code is short and makes sense. If not, you'll be typing it without knowing why it works.

Make sure you can answer these before you start:

- What does "ticking" a tree mean? Why is a tick not the same as a function call that runs until
  it's done?
- What do `Running`, `Success` and `Failure` mean? Who reads them, and what do they do with them?
- What's the difference between a Sequence ("do A, then B, then C") and a Fallback ("try A, else
  B, else C")? Draw the "enter the room" tree from `sapling_core/examples/enter_room.cpp` and trace
  it by hand, once for a locked door and once for an open one.
- Why do robots use behavior trees instead of state machines? (Hint: add one new behavior that
  must be able to interrupt all 20 states of a 20-state machine. How many changes is that?)

Reading: the intro pages of the [BehaviorTree.CPP docs](https://www.behaviortree.dev/) and
chapter 1 of Colledanchise & Ögren (free preprint on arXiv: search "Behavior Trees in Robotics and
AI: An Introduction"). Both assume no prior knowledge and will save you time in M1 to M4.

## M1: Node, Blackboard, leaves

**Goal:** the base class every node inherits from (`Node`) and the shared notepad nodes use to pass
data around (`Blackboard`).

**Why it matters:** Every later milestone builds on this. `Node` decides what "tick" and "halt" mean
for every node in the library. The blackboard is how the tree sees the world: in the turtle demo,
the battery level and the turtle's position reach the tree only through it.

Tests: `M1*`

Where to work, in this order:

1. `blackboard.hpp`: `set`, `get`, `has`, `erase`
2. `node.cpp`: `tick`, `halt`, `setBlackboard`
3. `leaves.cpp`: `Condition::onTick`, `Action::onTick`
4. `composite.cpp`: `CompositeNode::setBlackboard`

(All under `sapling_core/`. If you're not sure where something is, search for its name.)

New C++ you'll meet: `std::any` (a box that holds any type), `std::optional` (a value that might
be missing) and `std::function` (holds a lambda). The hints in the files explain each one.

Check yourself: why does `halt()` only call `onHalt()` when the node is `Running`?

## M2: Sequence and Fallback

**Goal:** the two basic ways of combining children. A Sequence does its children in order and
stops at the first failure. A Fallback tries its children in order and stops at the first success.

**Why it matters:** These two are what make a tree a tree. In `enter_room`, the root is a Sequence
(open the door, *then* enter) and "get the door open" is a Fallback (already open? else use the
handle? else pick the lock?). In the turtle demo, the patrol is a Sequence of waypoints and "make
sure the battery is OK" is a Fallback (fine, or else go recharge).

Tests: `M2*`

Where to work, in `sapling_core/src/composite.cpp`, in this order:

1. `haltChildren`
2. `Sequence::onTick` and `Sequence::onHalt`
3. `Fallback::onTick` and `Fallback::onHalt`

The tricky part is a child that returns `Running`. The Sequence or Fallback must return `Running`
too, and pick up at the same child on the next tick.

Check yourself: in the test `ResumesFromRunningChild`, the first child is not ticked again. When
is that what you want, and when is it dangerous? (You'll find out in M4.)

## M3: Decorators

**Goal:** nodes that wrap one child and change its result. Think of them as a filter between the
tree and that child.

**Why it matters:** Decorators change a node's behavior without writing a new node. The `Retry(5)`
around `PickLock` in `enter_room` is why the robot keeps trying the lock instead of giving up after
one failed attempt. `Inverter` and `ForceSuccess` let you reuse conditions and make steps optional.

Tests: `M3*`

Where to work, in `sapling_core/src/decorators.cpp`, in this order:

1. `DecoratorNode::setBlackboard` and `DecoratorNode::onHalt`
2. `Inverter` (flips Success and Failure)
3. `ForceSuccess` (turns any finished result into Success)
4. `Retry` (tries again if the child fails, up to a limit)
5. `Repeat` (runs the child a fixed number of times)

Check yourself: `Retry` loops *inside one tick*. What if the child always fails and
`max_attempts` is 1,000,000? Is that a problem for a robot that must tick 10 times a second?

## M4: Reactive nodes and Parallel

**Goal:** versions of Sequence and Fallback that forget where they were, so they re-check their
first children on every tick, plus a node that runs several children at once.

**Why it matters:** Without this, the turtle would keep driving on a flat battery.
`ReactiveSequence` is what lets the turtle stop mid-drive to go recharge, and it's the main reason
robots use behavior trees at all. `Parallel` isn't used by the turtle, but it's the standard way to
do two things at once.

Tests: `M4*`

Where to work, in `sapling_core/src/composite.cpp`, in this order:

1. `ReactiveSequence::onTick`
2. `ReactiveFallback::onTick`
3. `Parallel::onTick` and `Parallel::onHalt`

This is the most important idea in the project. Put "is the battery OK?" first in a
`ReactiveSequence` and "drive to the waypoint" second. The battery is checked on every tick, so a
low battery can stop the drive halfway. This is exactly what the turtle does in M6.

Check yourself: write down one situation where a `ReactiveSequence` is right and a plain
`Sequence` is a bug, and one where it's the other way round.

## M5: Tree runner and the example

**Goal:** the `Tree` class that owns the root and the blackboard and runs the tick loop, plus the
`childNodes()` functions that let it print the tree.

**Why it matters:** Until now you ticked nodes by hand in tests. `Tree` is what a real program uses:
it hooks up the blackboard and runs the loop. Finishing it gives you the first proof the whole
engine works (the `enter_room` output), and `toString()` lets you see the tree you built, which
helps a lot when debugging in M6.

Tests: `M5*`. Now **everything** should pass: `ctest --test-dir build`.

Where to work, in this order:

1. `childNodes()` in `composite.cpp` and in `decorators.cpp`
2. `tree.cpp`: the `Tree` constructor (no `SAPLING_TODO` here, just add one line), `tickOnce`,
   `tickWhileRunning`, `halt`, `toString`

Then run `./build/enter_room_example` and compare its output with the one in "What done looks
like" above. Try to predict each line before it prints.

## M6: ROS 2 turtlesim patrol

**Goal:** a simulated turtle that uses everything from M1 to M5 to patrol, and to stop and
recharge when its battery runs low.

**Why it matters:** This is the payoff: your engine driving a robot. It also shows why the details
matter. `MoveTo::onHalt` is what stops the turtle when it's interrupted, and the `ReactiveSequence`
at the root is what causes that interruption. This is the part you'll show in the README GIF.

Where to work, in `sapling_turtle/src/behaviors.cpp`, in this order:

1. `Recharge::onTick`
2. `MoveTo::onTick` and `MoveTo::onHalt`
3. `buildPatrolTree`, following the diagram in `behaviors.hpp`

`patrol_node.cpp` is already written. Read it first: it turns ROS topics into blackboard values
(`"pose"`, `"battery"`), so your tree code never needs to know about ROS.

Then:

1. `colcon build`, launch, and watch the turtle patrol, run low, recharge and resume.
2. Answer the three questions in `behaviors.hpp` in `docs/DESIGN.md`.
3. Record a GIF and put it in the README.

Experiments worth trying:
- Make the root a plain `Sequence`. What happens when the battery runs out? Why?
- Delete the body of `MoveTo::onHalt()`. What does the turtle do when it's interrupted? Why?
- Write unit tests for `MoveTo` using a fake `TurtleIO` that records velocities. No ROS needed.

## M7: Stretch goals (pick any)

Optional, and in no particular order.

- **XML trees.** Load trees from an XML file (tinyxml2 via CMake `FetchContent`) using a node
  factory, so you can change the robot's behavior without recompiling.
- **Ports.** Give nodes named inputs and outputs that map to blackboard keys, instead of
  hard-coded keys like `"battery"` (like BehaviorTree.CPP's `{target}` syntax).
- **Observer / logger.** Record every status change in `tick()`, and print a live coloured tree or
  publish it on a ROS topic.
- **Timeout decorator.** Fail the child if it stays `Running` too long. Pass in the clock so tests
  don't have to sleep.
- **Async actions.** Wrap a ROS 2 action client (e.g. `turtlesim/action/RotateAbsolute`) in a leaf
  that returns `Running` while the goal is active and cancels it in `onHalt()`.
- **Benchmark.** Use Google Benchmark to time a 1,000-node tree and make it faster.
- **Compare.** Rebuild the patrol in BehaviorTree.CPP and write up the differences in
  `docs/DESIGN.md`. This is a good bridge to Nav2, which uses BehaviorTree.CPP.
