#pragma once
#include <memory>
#include <string>
#include <vector>

#include "sapling/blackboard.hpp"
#include "sapling/status.hpp"

namespace sapling {

class Node;
using NodePtr = std::shared_ptr<Node>;

// Milestone 1.
// Base class of everything in the tree. Every leaf, composite and decorator is a Node.
//
// In plain words: a node is one box in the tree. It can be ticked ("do your thing
// once and tell me how it went": Running / Success / Failure), halted ("stop, you have
// been interrupted"), and it can read and write the shared Blackboard. The tree's
// shape comes from nodes holding other nodes (composites and decorators).
//
// It uses the "template method" pattern:
// the public tick()/halt() handle bookkeeping that is identical for every node,
// and call the protected virtual onTick()/onHalt() that subclasses implement.
class Node {
 public:
  explicit Node(std::string name);
  virtual ~Node() = default;

  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;

  // Tick this node once: call onTick(), remember the returned status
  // (so status() reports it) and return it.
  Status tick();

  // Interrupt this node. If it is currently Running, call onHalt() so it can
  // clean up (stop motors, cancel goals, halt children...).
  // Afterwards the node must always be back to Status::Idle.
  void halt();

  Status status() const { return status_; }
  const std::string& name() const { return name_; }

  // Attach the shared blackboard. Nodes with children must override this to
  // pass the blackboard down to them as well.
  virtual void setBlackboard(Blackboard::Ptr bb);
  Blackboard::Ptr blackboard() const { return blackboard_; }

  // Non-owning view of this node's children, used by Tree::toString().
  // Leaves have no children, so the default returns an empty vector.
  virtual std::vector<const Node*> childNodes() const { return {}; }

 protected:
  // Do the actual work. Must return Running, Success or Failure (never Idle).
  virtual Status onTick() = 0;

  // Called by halt() only when the node was Running. Default: nothing to do.
  virtual void onHalt() {}

 private:
  std::string name_;
  Status status_ = Status::Idle;
  Blackboard::Ptr blackboard_;
};

}  // namespace sapling
