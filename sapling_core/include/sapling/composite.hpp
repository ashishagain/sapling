#pragma once
#include <cstddef>
#include <vector>

#include "sapling/node.hpp"

namespace sapling {

// Base class for control-flow nodes with any number of children.
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
//  - A child returns Failure  -> reset, return Failure.
//  - A child returns Running  -> return Running. On the NEXT tick, resume from
//                                that child (earlier children are NOT re-ticked;
//                                this is a "sequence with memory").
//  - All children Success     -> reset, return Success.
//  - No children              -> Success.
// "Reset" means: halt all children and start from the first child next time.
class Sequence : public CompositeNode {
 public:
  explicit Sequence(std::string name = "Sequence");

 protected:
  Status onTick() override;
  void onHalt() override;

 private:
  std::size_t current_ = 0;
};

// Fallback / Selector ("OR", drawn as ?). The mirror image of Sequence:
// returns Success as soon as one child succeeds, Failure if all fail,
// and resumes from a Running child on the next tick.
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

// ReactiveSequence. Like Sequence, but WITHOUT memory: every tick starts from
// the first child again. That lets conditions placed before a long-running
// action be re-checked continuously.
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

// ReactiveFallback. The mirror image of ReactiveSequence.
class ReactiveFallback : public CompositeNode {
 public:
  explicit ReactiveFallback(std::string name = "ReactiveFallback");

 protected:
  Status onTick() override;
};

// Parallel. Each tick, ticks every child that has not completed yet
// (in order, ALL of them), THEN looks at the results:
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
