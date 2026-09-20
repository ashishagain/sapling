#pragma once
#include <cstddef>
#include <vector>

#include "sapling/node.hpp"

namespace sapling {

// Base class for control-flow nodes with any number of children.
//
// In plain words: a composite is a node that has children and decides WHICH of them
// to run, and in what order. It does no real work itself (the leaves do that). You
// never create a plain CompositeNode; it just holds what Sequence, Fallback, etc.
// share: the list of children (children_), addChild(), and haltChildren().
class CompositeNode : public Node {
 public:
  using Node::Node;

  // Returns *this so calls can be chained: seq->addChild(a).addChild(b);
  CompositeNode& addChild(NodePtr child);
  const std::vector<NodePtr>& children() const { return children_; }

  void setBlackboard(Blackboard::Ptr bb) override;       // Milestone 1
  std::vector<const Node*> childNodes() const override;  // Milestone 5

 protected:
  // Halt children with index >= `from`.
  void haltChildren(std::size_t from = 0);
  void onHalt() override { haltChildren(); }

  std::vector<NodePtr> children_;
};

// ---------------------------------------------------------------------------
// Milestone 2.

// Sequence ("AND", drawn as ->). Ticks children left to right.
//
// In plain words: "do these steps in order; if any step fails, the whole thing fails."
// Example (enter_room.cpp): the root is a Sequence of [get the door open, enter the
// room]. Entering only makes sense if the door got opened.
//
// Words used below: to "tick" a child is to call child->tick() and get back its Status
// (Running / Success / Failure). A child is "Running" when it needs more ticks to
// finish (a robot still driving). A Sequence remembers, between ticks, which child it
// is up to; that memory is the member current_ in composite.cpp.
//
// The exact rules (the tests check these):
//  - A child returns Failure  -> reset, return Failure.
//  - A child returns Running  -> return Running. On the NEXT tick, resume from
//                                that child (earlier children are NOT re-ticked;
//                                this is a "sequence with memory").
//  - All children Success     -> reset, return Success.
//  - No children              -> Success.
// "Reset" means: halt all children and start from the first child next time.
// (halt = tell a node to stop what it is doing; see Node::halt in node.hpp.)
class Sequence : public CompositeNode {
 public:
  explicit Sequence(std::string name = "Sequence");

 protected:
  Status onTick() override;
  void onHalt() override;

 private:
  std::size_t current_ = 0;
};

// Fallback / Selector ("OR", drawn as ?). The mirror image of Sequence.
//
// In plain words: "try these options in order until one works." Example: to get the
// door open, first check whether it is already open, else try the handle, else pick the
// lock. The first option that succeeds ends the search; if every option fails, the
// Fallback fails. The turtle demo uses one for "battery OK? else go recharge".
//
// The exact rules (the tests check these): returns Success as soon as one child
// succeeds, Failure if all fail, and resumes from a Running child on the next tick.
// No children -> Failure.
class Fallback : public CompositeNode {
 public:
  explicit Fallback(std::string name = "Fallback");

 protected:
  Status onTick() override;
  void onHalt() override;

 private:
  std::size_t current_ = 0;
};

// ---------------------------------------------------------------------------
// Milestone 4.

// ReactiveSequence. Like Sequence, but WITHOUT memory: every tick starts from the first
// child again. That lets conditions placed before a long-running action be re-checked
// continuously.
//
// In plain words: this is the important idea of the whole project. A plain Sequence
// remembers where it was and never looks at its earlier children again. A
// ReactiveSequence forgets, so it re-checks its first children on every tick. Put "is the
// battery OK?" first and a long "drive to the waypoint" second: the moment the battery
// check fails, the driving is halted. (In the turtle demo, this is what makes the turtle
// stop and go recharge mid-drive.)
//
// The exact rules (the tests check these):
//  - Child i returns Running -> halt children after i, return Running.
//  - Child i returns Failure -> halt ALL children (this is how a still-running
//                               action gets interrupted), return Failure.
//  - All Success             -> halt all children, return Success.
class ReactiveSequence : public CompositeNode {
 public:
  explicit ReactiveSequence(std::string name = "ReactiveSequence");

 protected:
  Status onTick() override;
};

// ReactiveFallback. The mirror image of ReactiveSequence: no memory, re-checks its
// earlier children on every tick, with the roles of Success and Failure swapped.
class ReactiveFallback : public CompositeNode {
 public:
  explicit ReactiveFallback(std::string name = "ReactiveFallback");

 protected:
  Status onTick() override;
};

// Parallel. Each tick, ticks every child that has not completed yet (in order, ALL of
// them), THEN looks at the results.
//
// In plain words: run several children "at the same time" (really one after another
// within the same tick, none waiting for the others), and decide the result by counting
// how many succeeded. success_threshold_ is how many must succeed for the Parallel to
// succeed. Example: "drive to the door AND play a sound" with a threshold of 2.
//
// The exact rules (the tests check these):
//  - #Success >= success_threshold                 -> halt running children,
//                                                     reset, return Success.
//  - success_threshold can no longer be reached    -> halt running children,
//    (#Failure > #children - success_threshold)       reset, return Failure.
//  - otherwise                                     -> Running.
// Children that completed are not ticked again until the Parallel resets.
class Parallel : public CompositeNode {
 public:
  Parallel(std::size_t success_threshold, std::string name = "Parallel");

 protected:
  Status onTick() override;
  void onHalt() override;

 private:
  std::size_t success_threshold_;
  std::vector<Status> results_;  // Hint: remember completed children here.
};

}  // namespace sapling
