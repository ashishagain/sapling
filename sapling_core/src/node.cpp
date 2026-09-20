#include "sapling/node.hpp"

#include <utility>

namespace sapling {

// Functions with a SAPLING_TODO line inside are yours to write. Everything else in this file is
// written for you: read it if you're curious, but you don't need to change it.

Node::Node(std::string name) : name_(std::move(name)) {}

Status Node::tick() {
  // Milestone 1: call onTick(), store the result in status_, return it.
  // WHY: "ticking" is the heartbeat of the whole project. Once per cycle the Tree ticks
  // the root, the root ticks its children, and so on down; each node answers Running,
  // Success or Failure and its parent decides what to do next. Every node type in the
  // library goes through this one function, so it is where "a node was asked to
  // act" is defined. Remembering the answer in status_ lets parents (and tests) ask
  // "what did you last say?".
  // TODO (M1): run this node's logic once, remember the answer, and return it. Replace the
  //     SAPLING_TODO line below with your code.
  //
  //     onTick() is the function every node type fills in with its own behaviour: a Sequence's
  //     onTick() loops over its children, an Action's onTick() runs its lambda, and so on. It is
  //     virtual, so calling it here runs the version belonging to whichever kind of node you are
  //     actually inside. It returns a Status.
  //
  //     status_ is the member that remembers the node's last answer (status() reads it). So: call
  //     onTick(), store what it returns in status_, and return it.
  //
  //         status_ = onTick();
  //         return status_;
  //
  SAPLING_TODO("Node::tick");
}

void Node::halt() {
  // Milestone 1: call onHalt() only if Running, then go back to Idle.
  // WHY: halting is how a tree INTERRUPTS work. In the turtle demo, when the battery
  // gets low the tree halts the MoveTo that is mid-drive so the turtle stops. Without
  // halt(), a robot would keep executing a stale action after the tree changed its mind.
  // TODO (M1): interrupt this node: clean up if it was running, and end up Idle. Replace the
  //     SAPLING_TODO line below with your code.
  //
  //     onHalt() is the cleanup hook (stop the motors, cancel a goal). It should only run when
  //     there is something to clean up, which means only when status_ is Status::Running. A node
  //     that is Idle, Success or Failure isn't in the middle of anything. The tests count how many
  //     times onHalt() ran, so calling it too often fails them.
  //
  //     Whether or not it was running, the node must be Status::Idle afterwards. So you need an
  //     `if` around the onHalt() call, and an assignment to status_ that is NOT inside the `if`.
  //
  //         if (status_ == Status::Running) {
  //             // call onHalt()
  //         }
  //         // then set status_ back to Idle
  //
  SAPLING_TODO("Node::halt");
}

void Node::setBlackboard(Blackboard::Ptr bb) {
  // Milestone 1
  // WHY: every node needs access to the shared blackboard (see blackboard.hpp) to read
  // and write facts. The Tree gives it to the root once, and it is passed down from
  // parent to child, so no node has to be wired to the blackboard by hand.
  // TODO (M1): remember the blackboard this node was given. Replace the SAPLING_TODO line below
  //     with your code.
  //
  //     blackboard_ is the member holding this node's pointer to the shared blackboard (a
  //     std::shared_ptr). Store the one you were given in it. std::move(bb) hands it over without
  //     an extra copy; plain assignment also works.
  //
  //         blackboard_ = std::move(bb);
  //
  SAPLING_TODO("Node::setBlackboard");
}

}  // namespace sapling
