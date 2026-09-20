#pragma once
#include "sapling/node.hpp"

namespace sapling {

// Base class for nodes with exactly one child.
//
// In plain words: a decorator wraps ONE node and changes what it reports, or how often
// it runs, without the wrapped node knowing. Think of it as a filter between the tree
// and that node (child_). Inverter, ForceSuccess, Retry and Repeat are decorators.
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
// In plain words: a logical NOT. An Inverter around "IsDoorOpen" reads as "is the door
// closed?" without writing a new condition.
class Inverter : public DecoratorNode {
 public:
  explicit Inverter(NodePtr child, std::string name = "Inverter");

 protected:
  Status onTick() override;
};

// Success and Failure both become Success. Running stays Running.
// In plain words: "try this, but do not let it failing stop the rest of the tree."
class ForceSuccess : public DecoratorNode {
 public:
  explicit ForceSuccess(NodePtr child, std::string name = "ForceSuccess");

 protected:
  Status onTick() override;
};

// Retry: tick the child up to `max_attempts` times IN THE SAME TICK while it keeps
// failing.
//
// In plain words: "if it fails, have another go." In enter_room.cpp, picking the lock
// often fails, so it is wrapped in Retry(5): up to five goes before giving up. "IN THE
// SAME TICK" means the loop happens inside one call to tick(), not one attempt per tick.
//
// The exact rules (the tests check these):
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

// Repeat: tick the child until it has succeeded `times` times, looping within the same
// tick.
//
// In plain words: "do this N times" (e.g. wave three times). Retry repeats while the
// child fails; Repeat repeats while it succeeds.
//
// The exact rules (the tests check these):
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
