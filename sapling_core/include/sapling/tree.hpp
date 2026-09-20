#pragma once
#include <chrono>
#include <string>

#include "sapling/node.hpp"

namespace sapling {

// Milestone 5.
// Owns the root node and the blackboard, and drives the tick loop.
//
// In plain words: the piece a real program uses. Build your nodes, hand the root (and a
// blackboard) to a Tree, then call tickOnce() every cycle (the turtle does this ~10
// times a second) or tickWhileRunning() to run to completion (the enter_room example).
class Tree {
 public:
  // Attach `bb` to every node of the tree (via root->setBlackboard).
  explicit Tree(NodePtr root, Blackboard::Ptr bb = std::make_shared<Blackboard>());

  // Tick the root once and return its status.
  Status tickOnce();

  // Keep ticking the root while it returns Running, sleeping `period`
  // between ticks. Return the final (completed) status.
  Status tickWhileRunning(std::chrono::milliseconds period = std::chrono::milliseconds(0));

  // Halt the whole tree.
  void halt();

  Node& root() { return *root_; }
  Blackboard& blackboard() { return *blackboard_; }

  // Human-readable dump: one node name per line, indented by two spaces per
  // level of depth, every line ending in '\n'. Example:
  //   Root
  //     A
  //     Sub
  //       B
  std::string toString() const;

 private:
  NodePtr root_;
  Blackboard::Ptr blackboard_;
};

}  // namespace sapling
