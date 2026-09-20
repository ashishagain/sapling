#include "sapling/decorators.hpp"

#include <stdexcept>
#include <utility>

namespace sapling {

// Functions with a SAPLING_TODO line inside are yours to write. Everything else in this file is
// written for you: read it if you're curious, but you don't need to change it.

// ----------------------------------------------------------------- Decorator

DecoratorNode::DecoratorNode(std::string name, NodePtr child)
    : Node(std::move(name)), child_(std::move(child)) {
  if (!child_) throw std::invalid_argument("DecoratorNode '" + this->name() + "' needs a child");
}

void DecoratorNode::setBlackboard(Blackboard::Ptr bb) {
  // Milestone 3
  // WHY: same as CompositeNode::setBlackboard (M1): the decorator's child needs the
  // blackboard too, or the Tree could not hand it to every node.
  // TODO (M3): give the blackboard to this decorator and to the node it wraps. Replace the
  //     SAPLING_TODO line below with your code.
  //
  //     A decorator wraps exactly one node, child_. The job is CompositeNode::setBlackboard from M1
  //     with no loop: store the blackboard in this node (through the Node:: version), then give it
  //     to child_. Look at your M1 version for the shape.
  //
  SAPLING_TODO("DecoratorNode::setBlackboard");
}

std::vector<const Node*> DecoratorNode::childNodes() const {
  // Milestone 5
  // WHY: same purpose as CompositeNode::childNodes, so Tree::toString can print the
  // decorator's child indented under it.
  // TODO (M5): return the single child in the same kind of list. Replace the SAPLING_TODO line
  //     below with your code.
  //
  //     Same idea as CompositeNode::childNodes, but a decorator has just one child, child_. The
  //     vector contains one plain pointer.
  //
  SAPLING_TODO("DecoratorNode::childNodes");
}

void DecoratorNode::onHalt() {
  // Milestone 3
  // WHY: interrupting a decorator must interrupt what it wraps, or the child would keep
  // running unsupervised.
  // TODO (M3): when this decorator is interrupted, interrupt what it wraps. Replace the
  //     SAPLING_TODO line below with your code.
  //
  //     If the decorator is halted but child_ isn't, the child would carry on running with nobody
  //     in charge. Call halt() on child_. One line.
  //
  SAPLING_TODO("DecoratorNode::onHalt");
}

// ------------------------------------------------------------------ Inverter

Inverter::Inverter(NodePtr child, std::string name)
    : DecoratorNode(std::move(name), std::move(child)) {}

Status Inverter::onTick() {
  // Milestone 3
  // WHY: a decorator is a small adapter around ONE child that changes its answer. An
  // Inverter is a logical NOT: "IsDoorOpen" wrapped in an Inverter becomes "IsDoorClosed"
  // without writing a new condition.
  // TODO (M3): run the child once and hand back the opposite answer. Replace the SAPLING_TODO line
  //     below with your code.
  //
  //     A decorator changes the answer of the node it wraps. Inverter is a logical NOT. Tick child_
  //     once (child_->tick() gives you its Status) and keep the result in a variable.
  //
  //     Success and Failure swap. Running has nothing to swap, so pass it through unchanged. Test
  //     M3Inverter.Inverts lists the three cases. It is useful because an Inverter around
  //     IsDoorOpen gives you 'is the door closed' without writing a new condition.
  //
  SAPLING_TODO("Inverter::onTick");
}

// -------------------------------------------------------------- ForceSuccess

ForceSuccess::ForceSuccess(NodePtr child, std::string name)
    : DecoratorNode(std::move(name), std::move(child)) {}

Status ForceSuccess::onTick() {
  // Milestone 3
  // WHY: for optional steps. "Try to wave at the person, but if waving fails that must not
  // stop the rest of the sequence." Wrapping the wave action in ForceSuccess does that.
  // TODO (M3): run the child once and report Success whenever it has finished. Replace the
  //     SAPLING_TODO line below with your code.
  //
  //     Tick child_ once. While the child is still Running, you are Running too. Once it has
  //     finished, however it went, you report Success. This is for optional steps: 'try to wave,
  //     but if waving fails don't let it stop the rest of the Sequence'.
  //
  SAPLING_TODO("ForceSuccess::onTick");
}

// --------------------------------------------------------------------- Retry

Retry::Retry(NodePtr child, int max_attempts, std::string name)
    : DecoratorNode(std::move(name), std::move(child)), max_attempts_(max_attempts) {}

Status Retry::onTick() {
  // Milestone 3
  // WHY: this is the "Retry(5)" around PickLock in the enter_room example. Picking a
  // lock often fails the first few times; Retry keeps trying until it works or the limit
  // is reached, so the tree does not give up on a door because of one bad attempt.
  // TODO (M3): give the child several goes, inside ONE tick, before giving up. Replace the
  //     SAPLING_TODO line below with your code.
  //
  //     max_attempts_ is the limit. attempts_ counts the failures so far. It is a member, so it
  //     survives between ticks, which matters when the child returns Running.
  //
  //     Things to decide: what loop keeps going while there are attempts left; what you do for each
  //     of the three answers from child_->tick(); and when attempts_ goes back to 0. It must, every
  //     time Retry finishes, whether it succeeded or gave up, so the next run gets a fresh set of
  //     attempts (test ResetsBetweenRuns).
  //
  //     Be careful with Running: it must return Running immediately, without counting as a failure
  //     and without losing attempts_. Test RunningKeepsAttemptCount walks through exactly this;
  //     read it before you start.
  //
  SAPLING_TODO("Retry::onTick");
}

void Retry::onHalt() {
  // Milestone 3: reset the attempt counter and halt the child.
  // TODO (M3): forget the failures so far and stop the child. Replace the SAPLING_TODO line below
  //     with your code.
  //
  //     Set attempts_ back to 0. Stopping the child is what DecoratorNode::onHalt() already does,
  //     so call that rather than rewriting it. Write the class name in front
  //     (DecoratorNode::onHalt()), because Retry has its own onHalt().
  //
  SAPLING_TODO("Retry::onHalt");
}

// -------------------------------------------------------------------- Repeat

Repeat::Repeat(NodePtr child, int times, std::string name)
    : DecoratorNode(std::move(name), std::move(child)), times_(times) {}

Status Repeat::onTick() {
  // Milestone 3
  // WHY: "do this N times", e.g. wave three times. Retry's counterpart: Retry repeats
  // while the child FAILS, Repeat repeats while it SUCCEEDS.
  // TODO (M3): run the child until it has succeeded times_ times, inside ONE tick. Replace the
  //     SAPLING_TODO line below with your code.
  //
  //     This is Retry with the roles swapped. count_ counts successes so far (it survives between
  //     ticks) and times_ is how many you need.
  //
  //     Decide what changes compared with Retry: what you count, what makes you stop early, and
  //     what you do on Running. Tests M3Repeat.StopsOnFailure and RunningKeepsCount cover the two
  //     tricky cases.
  //
  SAPLING_TODO("Repeat::onTick");
}

void Repeat::onHalt() {
  // Milestone 3
  // TODO (M3): forget the count so far and stop the child. Replace the SAPLING_TODO line below with
  //     your code.
  //
  //     Same idea as Retry::onHalt: reset count_, then call DecoratorNode::onHalt().
  //
  SAPLING_TODO("Repeat::onHalt");
}

}  // namespace sapling
