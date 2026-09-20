# Roadmap

Work through the milestones in order. Each one has a test suite. Your job is to make it pass.

Every unimplemented function throws `Not implemented yet: <Function>`, so running the tests for a
milestone tells you exactly which function to write next — run them, read the failure, open the
file, implement that one function, and re-run. Once a function is done, delete its
`SAPLING_TODO(...)` line (that's how you and the test runner both know it's finished).

```bash
cmake -S sapling_core -B build -DCMAKE_BUILD_TYPE=Debug   # once
cmake --build build -j && ./build/sapling_tests --gtest_filter='M1*'
```

The `--gtest_filter='M1*'` part runs only the Milestone 1 tests (swap `M1` for `M2`, `M3`, etc. as
you go). Building with `Debug` keeps assertions on and makes crashes easier to debug.

Tip: turn on sanitizers (`-DSAPLING_SANITIZE=ON`) from the start. This project uses raw and shared
pointers to link tree nodes together, and it's easy to accidentally use a pointer after its object
is gone. AddressSanitizer (ASan) catches that kind of bug immediately, with a clear error, instead
of letting it silently corrupt memory or crash somewhere unrelated ten lines later.

**Commit after every milestone** with a meaningful message. A clean history is part of what makes
this look good on GitHub.

---

## M0: Theory (an evening)

Do this milestone with no code, no IDE — just paper and a pen. It exists because the next six
milestones only make sense once you can picture what a behavior tree *is*.

The short version: a **behavior tree (BT)** is a way of describing decision-making as a tree of
nodes, where the leaves do actual work ("check the battery", "drive to a point") and the branches
(the non-leaf nodes) decide *which* leaves run and in what order. You re-evaluate the whole tree
from the root at a fixed rate — for example, ten times a second — rather than running it once to
completion. Every one of those evaluations is called a **tick**.

That single idea — re-evaluating repeatedly instead of running once — is the crux of the whole
project, so make sure these are not just words to you before moving on:

- What does "ticking" a tree mean? Why is a tick not the same as a function call that runs until
  done? (Concretely: if a node is in the middle of driving a robot somewhere, what does it mean for
  that node to be ticked *again* before it's finished?)
- What do `Running`, `Success`, `Failure` mean? Every node returns one of these three when ticked.
  Who looks at that return value, and what do they do with it?
- What's the difference between a Sequence and a Fallback? A Sequence is like a chain of `&&`s (do
  A, and if that works do B, and if that works do C); a Fallback is like a chain of `||`s (try A,
  and if that fails try B). Draw the "enter the room" tree from
  `sapling_core/examples/enter_room.cpp` on paper, and trace through it by hand for a locked door
  and an already-open door.
- Why do robots use BTs instead of finite state machines (FSMs)? (Hint: in an FSM you have a fixed
  set of states and you hand-wire a transition between every pair of states that can reach each
  other. Imagine a 20-state FSM for a robot, and now imagine adding one new behavior — say, "pause
  for a human" — that should be able to interrupt *any* of those 20 states. How many new transitions
  does that take in an FSM? How many nodes does it take in a BT?)

Reading: the introduction pages of the [BehaviorTree.CPP docs](https://www.behaviortree.dev/) and
chapter 1 of Colledanchise & Ögren (free preprint on arXiv: search "Behavior Trees in Robotics and
AI: An Introduction"). Both are written for people who have never seen a BT before — don't skip them
to get to the code sooner, they'll save you time in M1–M4.

## M1: Node, Blackboard, leaves

**What you're building:** the one base class every node in the tree inherits from (`Node`), and the
shared "notepad" nodes use to communicate (`Blackboard`).

Files: `blackboard.hpp`, `node.cpp`, `leaves.cpp`, `CompositeNode::setBlackboard` in `composite.cpp`

Tests: `M1*`

Concepts you'll need:

- **The template-method pattern.** `Node::tick()` is a normal public function that every node
  shares — it's already implemented for you. What you implement is `onTick()`, a `protected virtual`
  function that subclasses override with their actual behavior. `tick()` calls `onTick()`, then does
  the bookkeeping that's identical for *every* node type: remembering the returned status so
  `status()` can report it later. This split means you never have to re-write "remember my status"
  in every single node — you write the interesting 5 lines, the base class handles the rest.
- **`std::any`.** The blackboard needs to store values of different types (a `bool` here, a `double`
  there, a custom `Pose2D` struct elsewhere) in one container, keyed by string name. `std::any` is a
  box that can hold a value of *any* type and remembers what type it holds, so you can put a `bool`
  in one slot and an `int` in the next.
- **`std::optional`.** When you ask the blackboard "give me the value at key `battery`", that key
  might not exist yet, or might hold the wrong type. Rather than crashing or returning a made-up
  default, `Blackboard::get<T>()` returns `std::optional<T>` — a wrapper that either contains a value
  or is empty. The caller checks with `.has_value()` or unwraps it with `.value_or(fallback)`. This
  is how the codebase avoids null pointers and exceptions for "the value might not be there."
- **`std::function`.** Leaves (`Action`, `Condition`) don't have their own subclasses for every
  behavior — instead they're built by handing them a callable (a lambda, most often) that gets run on
  tick. `std::function<Status(Blackboard&)>` is a type that can hold *any* callable with that
  signature — a lambda, a free function, a functor — so `Action`/`Condition` can wrap whatever you
  pass them. Look at how `enter_room.cpp` builds `OpenDoor` with a lambda to see this in action.

Check yourself: why does `halt()` only call `onHalt()` when the node is `Running`? (Think about what
it would mean to "interrupt" a node that already finished, or one that never started.)

## M2: Sequence and Fallback

**What you're building:** the two most common ways of combining children — "do these in order, stop
if one fails" (Sequence) and "try these in order, stop as soon as one succeeds" (Fallback).

Files: `composite.cpp` (`haltChildren`, `Sequence`, `Fallback`)

Tests: `M2*`

Both of these need to handle a child that returns `Running` — i.e., a child that needs more than one
tick to finish (like a robot still driving somewhere). When that happens, the Sequence/Fallback
itself returns `Running` too, and remembers *which* child was running so that on the *next* tick it
resumes from that child instead of starting over from the first one. This is what the comments in
`composite.hpp` mean by "a sequence with memory" — it remembers its position between ticks.

Check yourself: in `ResumesFromRunningChild`, the first child is not re-ticked. When is that what
you want (imagine child 1 was a one-off "open the claw" action that already succeeded — re-running it
would be pointless or even harmful), and when is it dangerous (imagine child 1 was a safety check
like "is the path still clear?" — should that really only be checked once?)? You'll get the answer in
M4.

## M3: Decorators

**What you're building:** nodes that wrap a *single* child and modify its result or how it's ticked,
rather than combining multiple children like Sequence/Fallback do. Think of a decorator as a filter
sitting between the tree and one child: the child does the real work, the decorator changes what
comes back out.

Files: `decorators.cpp`

Tests: `M3*`

The four you're implementing:
- `Inverter` — flips Success into Failure and vice versa (like a logical NOT). `Running` passes
  through unchanged, because there's nothing to invert yet.
- `ForceSuccess` — turns Failure into Success too (useful for "try this, but don't let it block the
  rest of the tree if it doesn't work").
- `Retry` — re-ticks its child up to `max_attempts` times *within a single tick* while the child keeps
  failing, only giving up (and returning `Failure`) after the limit.
- `Repeat` — the mirror image: keeps re-ticking the child until it has *succeeded* `times` times in a
  row, all within a single tick.

Check yourself: `Retry` loops *inside a single tick*. What would happen if the child were a
`Condition` that always fails and `max_attempts` were 1,000,000? Since a real robot's control loop
needs to tick at a fixed rate (say, every 100ms) to stay responsive, what does a decorator that can
block for 1,000,000 iterations *inside one tick* do to that timing? Is that a problem for a real-time
robot?

## M4: Reactive nodes and Parallel

**What you're building:** versions of Sequence/Fallback that *forget* their position instead of
remembering it, plus a node that runs several children at once.

Files: `composite.cpp` (`ReactiveSequence`, `ReactiveFallback`, `Parallel`)

Tests: `M4*`

This is the most important milestone conceptually, so slow down here even if the code ends up short.
Reactivity is the reason robots use BTs instead of, say, a simple script. Recall from M2 that
`Sequence` remembers which child was running and skips straight back to it. `ReactiveSequence` does
the opposite: **every single tick, it starts back at the first child.** That sounds wasteful, but it
means a condition placed before a long-running action gets *re-checked on every tick* — if that
condition is now false, the reactive sequence halts the running action immediately instead of letting
it keep going. This is exactly the pattern the turtlesim patrol robot in M6 uses: "is the battery OK?"
is checked before "drive to the next waypoint" on every tick, so a dying battery can interrupt a
drive-in-progress instead of waiting for it to finish first.

`Parallel` is different from both: instead of picking one child to run at a time, it ticks *every*
unfinished child on every tick, then decides Success/Failure/Running by counting how many of them
succeeded or failed so far, against a `success_threshold` you configure.

Check yourself: write down (on paper, in the PR description, wherever) one concrete situation where a
`ReactiveSequence` is correct and a plain `Sequence` would be a bug, and one situation where it's the
other way around. (Hint for the "reactive is a bug" case: think about what happens to `PickLock`'s
attempt counter in `enter_room.cpp` if the condition before it were re-checked, and thus the whole
subtree restarted, on every single tick.)

## M5: Tree runner and the example

**What you're building:** the `Tree` class that owns the root node and the blackboard together, and
the plumbing (`childNodes()`) that lets it walk the tree to print it.

Files: `tree.cpp`, `childNodes()` in `composite.cpp` and `decorators.cpp`

Tests: `M5*` (and now **everything** should be green: `ctest --test-dir build`)

Up to now you've been ticking individual nodes directly in tests. `Tree` is the thing a real program
actually uses: it wires the blackboard into the root once, and `tickWhileRunning()` repeatedly ticks
the root until it returns `Success` or `Failure` (instead of `Running`) — simulating what a robot's
control loop does when it calls tick() over and over at a fixed rate. `toString()` uses the
`childNodes()` you implement here to walk the whole tree and print it, which is invaluable for
debugging once trees get bigger than a page.

Once this milestone's tests pass, run `./build/enter_room_example` and read the printed trace
alongside the tree diagram in the comment at the top of `enter_room.cpp` — you should be able to
predict each line of output before it prints.

## M6: ROS 2 turtlesim patrol

**What you're building:** a real (simulated) robot that uses everything from M1–M5 to patrol between
waypoints and interrupt itself to recharge when its battery gets low. This is where the project stops
being an exercise and starts being a robot.

Files: `sapling_turtle/src/behaviors.cpp`. The ROS node (`patrol_node.cpp`) is already written for
you — read it first. It subscribes to the turtle's pose and publishes velocity commands; its whole
job is translating those ROS topics into blackboard keys (`"pose"`, `"battery"`) and back into
velocity commands, so that the tree logic in `behaviors.cpp` never has to know ROS exists. That
separation is deliberate: it's why `MoveTo` and `Recharge` can be unit-tested (see the experiments
below) without a simulator running at all.

1. Implement `Recharge` first (it's the simpler of the two: each tick, add `per_tick` to the
   `"battery"` blackboard value; return `Running` until it reaches 100, then `Success`). Then
   implement `MoveTo`: a **proportional controller**, meaning the further the turtle is from the goal
   or the more its heading is off, the harder it steers — turn towards the goal point, then drive
   towards it, and stop (returning `Success`) once within `tolerance`.
2. Build the tree in `buildPatrolTree` following the diagram in `behaviors.hpp`. Notice the root is a
   `ReactiveSequence` — that's M4's lesson, applied for real: it re-checks `BatteryOk` before every
   single patrol step, so a low battery can interrupt a `MoveTo` that's halfway to a waypoint.
3. `colcon build`, launch, and watch the turtle patrol, run low, recharge and resume.
4. Answer the three questions in `behaviors.hpp` in `docs/DESIGN.md` — they're designed to make sure
   you understand *why* the tree is shaped the way it is, not just that you copied the diagram.
5. Record a GIF and put it in the README.

Experiments (small, but worth doing — they build intuition no amount of reading will):
- Make the root a plain `Sequence` instead of `ReactiveSequence`. What happens when the battery runs
  out mid-patrol? Why?
- Delete the body of `MoveTo::onHalt()`. Launch it, let something interrupt a `MoveTo` mid-drive
  (e.g. a low battery), and watch what the turtle does. Why does skipping `onHalt()` cause that?
- Add unit tests for `MoveTo` with a fake `TurtleIO` that records velocities instead of publishing to
  ROS. This is the payoff of keeping the leaves ROS-free: you can test the *steering logic* — does it
  turn the right way, does it stop at the right distance — with a plain GoogleTest binary and no
  simulator, in milliseconds instead of minutes.

## M7: Stretch goals (pick any, each is a good CV talking point)

These are optional and unordered — pick whichever sounds most interesting. Each is a self-contained
extension to the engine you've already built, not a new milestone you're expected to fully finish.

- **XML trees.** Right now trees are built in C++ code (like `enter_room.cpp` does). Load them from
  an XML file instead, using tinyxml2 (pulled in via CMake's `FetchContent`) and a *node factory* — a
  registry mapping string names to constructors, e.g. `factory.registerNode<MoveTo>("MoveTo")`, so the
  XML can say `<MoveTo .../>` and the factory knows what C++ type to build. The payoff: you can change
  the robot's behavior by editing a text file, no recompile needed.
- **Ports.** Instead of every node reaching into hard-coded blackboard keys (like `"battery"`), give
  nodes named input/output *ports* that get mapped to blackboard keys from XML — this is what lets
  BehaviorTree.CPP write `{target}` in XML and have it mean "whatever key the tree assigns to this
  port," making nodes reusable across different trees without editing their C++.
- **Observer / logger.** Hook into `tick()` to record every status transition (e.g. "MoveTo went from
  Running to Success at time T"). Use that to either print a live, colored tree in the terminal as it
  runs, or publish the same information on a ROS topic so another tool can visualize it.
- **Timeout decorator**, structured like `Retry`/`Repeat` from M3, but time-based: fail the child if
  it's been `Running` for too long. Take the clock as an injected dependency (a function you can swap
  out in tests) rather than calling a real system clock directly, so tests can simulate time passing
  instantly instead of actually sleeping.
- **Async actions.** Wrap a ROS 2 *action client* (e.g. `turtlesim/action/RotateAbsolute` — ROS
  "actions" are its term for long-running, cancelable goals, distinct from this project's `Action`
  leaf) in a leaf that sends the goal once, returns `Running` while it's in flight, and cancels it
  from `onHalt()` if the tree moves on before it finishes.
- **Benchmark.** Use Google Benchmark to measure how long it takes to tick a 1,000-node tree, then
  try to make that faster (e.g. avoiding unnecessary allocation or `std::any` overhead on the hot
  path).
- **Compare.** Re-implement the patrol tree in BehaviorTree.CPP (the library this project deliberately
  avoided depending on) and write up the differences you hit in `docs/DESIGN.md`. This is a great
  bridge to Nav2, whose navigator is itself a BehaviorTree.CPP tree — understanding the differences
  first-hand makes reading Nav2's trees much easier.
