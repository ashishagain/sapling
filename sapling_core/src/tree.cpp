#include "sapling/tree.hpp"

#include <sstream>
#include <stdexcept>
#include <thread>
#include <utility>

namespace sapling {

Tree::Tree(NodePtr root, Blackboard::Ptr bb) : root_(std::move(root)), blackboard_(std::move(bb)) {
  if (!root_) throw std::invalid_argument("Tree needs a root node");
  // Milestone 5: give the blackboard to every node.
}

Status Tree::tickOnce() {
  // Milestone 5
  SAPLING_TODO("Tree::tickOnce");
}

Status Tree::tickWhileRunning(std::chrono::milliseconds period) {
  // Milestone 5
  SAPLING_TODO("Tree::tickWhileRunning");
}

void Tree::halt() {
  // Milestone 5
  SAPLING_TODO("Tree::halt");
}

std::string Tree::toString() const {
  // Milestone 5. Hint: a small recursive helper taking (const Node&, depth,
  // std::ostringstream&) makes this short.
  SAPLING_TODO("Tree::toString");
}

}  // namespace sapling
