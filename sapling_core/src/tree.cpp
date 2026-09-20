#include "sapling/tree.hpp"

#include <sstream>
#include <stdexcept>
#include <thread>
#include <utility>

namespace sapling {

Tree::Tree(NodePtr root, Blackboard::Ptr bb) : root_(std::move(root)), blackboard_(std::move(bb)) {
  if (!root_) throw std::invalid_argument("Tree needs a root node");
  // Milestone 5: give the blackboard to every node.
  // WHY: after M1-M4 you can build nodes and tick them by hand. Tree is the piece a real
  // program uses: it owns the root and the blackboard, so you write `Tree tree(root, bb)`
  // and get a runnable robot brain. This constructor is the "plug the blackboard in" step.
  // HINT: root_->setBlackboard(blackboard_); (M1/M3 made this recurse to every node).
  // No SAPLING_TODO here, so nothing will tell you it is missing except test
  // AttachesBlackboardToAllNodes.
}

Status Tree::tickOnce() {
  // Milestone 5
  // WHY: one tick = one "decision cycle". The turtle's ROS node calls this on a timer
  // (about 10 times a second), and each call re-evaluates the whole tree from the top.
  // HINT: one line: return root_->tick();
  SAPLING_TODO("Tree::tickOnce");
}

Status Tree::tickWhileRunning(std::chrono::milliseconds period) {
  // Milestone 5
  // WHY: the enter_room example is a program that starts, runs to completion and exits, so
  // it needs "keep ticking until the tree finishes". This is that loop, and it is
  // what produces the Running/Result output you see in the finished example.
  // HINT: loop: Status s = tickOnce(); if s != Running, return s; otherwise sleep for
  // `period` and go round again. Sleeping: std::this_thread::sleep_for(period)
  // (<thread> is already included). Default period is 0ms, so tests do not wait.
  SAPLING_TODO("Tree::tickWhileRunning");
}

void Tree::halt() {
  // Milestone 5
  // WHY: lets a program stop the whole tree cleanly (e.g. when the robot node shuts
  // down), so every running action gets its onHalt() and the motors stop.
  // HINT: root_->halt(). Node::halt() and each onHalt() already cascade down.
  SAPLING_TODO("Tree::halt");
}

std::string Tree::toString() const {
  // Milestone 5. WHY: this is the indented tree printout at the top of the enter_room
  // example's output. It is how you SEE the tree you built, invaluable for debugging.
  // Hint: a small recursive helper taking (const Node&, depth,
  // std::ostringstream&) makes this short.
  // HINT: helper(node, depth, out): write 2*depth spaces, then node.name(), then '\n';
  // then for each pointer in node.childNodes() call helper(*child, depth + 1, out).
  // Start with helper(*root_, 0, out) and return out.str(). A static free function
  // above this one, or a lambda, both work. Test ToString shows the exact format.
  SAPLING_TODO("Tree::toString");
}

}  // namespace sapling
