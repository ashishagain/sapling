#include "sapling/node.hpp"

#include <utility>

namespace sapling {

Node::Node(std::string name) : name_(std::move(name)) {}

Status Node::tick() {
  // Milestone 1: call onTick(), store the result in status_, return it.
  // WHY: "ticking" is the heartbeat of the whole project. Once per cycle the Tree ticks
  // the root, the root ticks its children, and so on down; each node answers Running,
  // Success or Failure and its parent decides what to do next. Every node type in the
  // library goes through this one function, so it is where "a node was asked to
  // act" is defined. Remembering the answer in status_ lets parents (and tests) ask
  // "what did you last say?".
  // HINT: this is the "template method". Three lines:
  //   status_ = onTick();   // onTick() is virtual: a subclass's version runs here
  //   return status_;       // (it returns Running/Success/Failure, never Idle)
  // Nothing else belongs here. Composites/decorators handle their own children in
  // their onTick().
  SAPLING_TODO("Node::tick");
}

void Node::halt() {
  // Milestone 1: call onHalt() only if Running, then go back to Idle.
  // WHY: halting is how a tree INTERRUPTS work. In the turtle demo, when the battery
  // gets low the tree halts the MoveTo that is mid-drive so the turtle stops. Without
  // halt(), a robot would keep executing a stale action after the tree changed its mind.
  // HINT:
  //   if (status_ == Status::Running) onHalt();   // only a node in progress needs cleanup
  //   status_ = Status::Idle;                     // ALWAYS, even if it was not running
  // Why "only if Running"? A finished/idle node has nothing to stop or cancel.
  // The tests count how many times onHalt() ran, so calling it too often fails them.
  SAPLING_TODO("Node::halt");
}

void Node::setBlackboard(Blackboard::Ptr bb) {
  // Milestone 1
  // WHY: every node needs access to the shared blackboard (see blackboard.hpp) to read
  // and write facts. The Tree gives it to the root once, and it is passed down from
  // parent to child, so no node has to be wired to the blackboard by hand.
  // HINT: just store bb in the blackboard_ member (std::move it). The children are
  // handled by the CompositeNode / DecoratorNode overrides, not here.
  SAPLING_TODO("Node::setBlackboard");
}

}  // namespace sapling
