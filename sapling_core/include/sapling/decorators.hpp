#pragma once
#include "sapling/node.hpp"

namespace sapling {

// Base class for nodes with exactly one child.
class DecoratorNode : public Node {
 public:
  DecoratorNode(std::string name, NodePtr child);

  Node& child() { return *child_; }

  void setBlackboard(Blackboard::Ptr bb) override;       // Milestone 3
  std::vector<const Node*> childNodes() const override;  // Milestone 5

 protected:
  void onHalt() override;  // Milestone 3: halt the child

  NodePtr child_;
};

// ---------------------------------------------------------------------------
// Milestone 3.

// Success <-> Failure. Running stays Running.
class Inverter : public DecoratorNode {
 public:
  explicit Inverter(NodePtr child, std::string name = "Inverter");

 protected:
  Status onTick() override;
};

// Success and Failure both become Success. Running stays Running.
class ForceSuccess : public DecoratorNode {
 public:
  explicit ForceSuccess(NodePtr child, std::string name = "ForceSuccess");

 protected:
  Status onTick() override;
};

// Retry: tick the child up to `max_attempts` times IN THE SAME TICK while it
// keeps failing.
//  - Child Success           -> reset attempts, return Success.
//  - Child Running           -> return Running (keep the attempt count).
//  - Child failed max times  -> reset attempts, return Failure.
class Retry : public DecoratorNode {
 public:
  Retry(NodePtr child, int max_attempts, std::string name = "Retry");

 protected:
  Status onTick() override;
  void onHalt() override;

 private:
  int max_attempts_;
  int attempts_ = 0;
};

// Repeat: tick the child until it has succeeded `times` times, looping within
// the same tick.
//  - Child Failure           -> reset count, return Failure.
//  - Child Running           -> return Running (keep the count).
//  - `times` successes       -> reset count, return Success.
class Repeat : public DecoratorNode {
 public:
  Repeat(NodePtr child, int times, std::string name = "Repeat");

 protected:
  Status onTick() override;
  void onHalt() override;

 private:
  int times_;
  int count_ = 0;
};

}  // namespace sapling
