#include "sapling/node.hpp"

#include <utility>

namespace sapling {

Node::Node(std::string name) : name_(std::move(name)) {}

Status Node::tick() {
  // Milestone 1: call onTick(), store the result in status_, return it.
  SAPLING_TODO("Node::tick");
}

void Node::halt() {
  // Milestone 1: call onHalt() only if Running, then go back to Idle.
  SAPLING_TODO("Node::halt");
}

void Node::setBlackboard(Blackboard::Ptr bb) {
  // Milestone 1
  SAPLING_TODO("Node::setBlackboard");
}

}  // namespace sapling
