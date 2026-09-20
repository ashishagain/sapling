#include "sapling/tree.hpp"

#include <sstream>
#include <stdexcept>
#include <thread>
#include <utility>

namespace sapling {

// Functions with a SAPLING_TODO line inside are yours to write. Everything else in this file is
// written for you: read it if you're curious, but you don't need to change it.

Tree::Tree(NodePtr root, Blackboard::Ptr bb) : root_(std::move(root)), blackboard_(std::move(bb)) {
  if (!root_) throw std::invalid_argument("Tree needs a root node");
  // Milestone 5: give the blackboard to every node.
  // WHY: after M1-M4 you can build nodes and tick them by hand. Tree is the piece a real
  // program uses: it owns the root and the blackboard, so you write `Tree tree(root, bb)`
  // and get a runnable robot brain. This constructor is the "plug the blackboard in" step.
  // TODO (M5): give the blackboard to the root node so every node in the tree can use it. This
  //     function has no SAPLING_TODO, so add your line right below this comment.
  //
  //     root_ is the top node. Its setBlackboard() passes the blackboard down to everything below
  //     it (you wrote that in M1 and M3). blackboard_ is the blackboard this Tree owns. One line.
  //     Only test M5Tree.AttachesBlackboardToAllNodes will notice if it is missing.
  //
}

Status Tree::tickOnce() {
  // Milestone 5
  // WHY: one tick = one "decision cycle". The turtle's ROS node calls this on a timer
  // (about 10 times a second), and each call re-evaluates the whole tree from the top.
  // TODO (M5): tick the root once and return its Status. Replace the SAPLING_TODO line below with
  //     your code.
  //
  //     root_ is the top node of the tree. One line.
  //
  SAPLING_TODO("Tree::tickOnce");
}

Status Tree::tickWhileRunning(std::chrono::milliseconds period) {
  // Milestone 5
  // WHY: the enter_room example is a program that starts, runs to completion and exits, so
  // it needs "keep ticking until the tree finishes". This is that loop, and it is
  // what produces the Running/Result output you see in the finished example.
  // TODO (M5): keep ticking until the tree finishes, and return the final Status. Replace the
  //     SAPLING_TODO line below with your code.
  //
  //     Use tickOnce() for each tick. If the answer is anything other than Running the tree is
  //     done, so return it. If it is Running, wait for `period` and tick again.
  //     std::this_thread::sleep_for(period) does the waiting (<thread> is already included).
  //
  SAPLING_TODO("Tree::tickWhileRunning");
}

void Tree::halt() {
  // Milestone 5
  // WHY: lets a program stop the whole tree cleanly (e.g. when the robot node shuts
  // down), so every running action gets its onHalt() and the motors stop.
  // TODO (M5): stop the whole tree. Replace the SAPLING_TODO line below with your code.
  //
  //     halt() on root_. It cascades down through the whole tree by itself, thanks to M1 to M4.
  //
  SAPLING_TODO("Tree::halt");
}

std::string Tree::toString() const {
  // Milestone 5. WHY: this is the indented tree printout at the top of the enter_room
  // example's output. It is how you SEE the tree you built, invaluable for debugging.
  // TODO (M5): return the tree as indented text. Replace the SAPLING_TODO line below with your
  //     code.
  //
  //     One node name per line, two spaces of indent per level of depth. The exact format is in the
  //     comment in tree.hpp and in test M5Tree.ToString.
  //
  //     Write a helper function ABOVE Tree::toString() that prints one node and then calls itself
  //     for each of that node's children, one level deeper. Its parameters are the node, the
  //     current depth, and the output stream:
  //
  //         static void print(const Node& node, int depth, std::ostringstream& out)
  //
  //     node.name() is the name and node.childNodes() is the list of children (the functions you
  //     just wrote). std::string(n, ' ') gives n spaces for the indent. In toString() itself, make
  //     a std::ostringstream, call your helper on *root_ at depth 0, and return the text with
  //     out.str().
  //
  SAPLING_TODO("Tree::toString");
}

}  // namespace sapling
