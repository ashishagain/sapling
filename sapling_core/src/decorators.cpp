#include "sapling/decorators.hpp"

#include <stdexcept>
#include <utility>

namespace sapling {

// ----------------------------------------------------------------- Decorator

DecoratorNode::DecoratorNode(std::string name, NodePtr child)
    : Node(std::move(name)), child_(std::move(child)) {
  if (!child_) throw std::invalid_argument("DecoratorNode '" + this->name() + "' needs a child");
}

void DecoratorNode::setBlackboard(Blackboard::Ptr bb) {
  // Milestone 3
  // WHY: same as CompositeNode::setBlackboard (M1): the decorator's child needs the
  // blackboard too, or the Tree could not hand it to every node.
  // HINT: same idea as CompositeNode::setBlackboard, with one child: call
  // Node::setBlackboard(bb) for this node, then child_->setBlackboard(bb).
  SAPLING_TODO("DecoratorNode::setBlackboard");
}

std::vector<const Node*> DecoratorNode::childNodes() const {
  // Milestone 5
  // WHY: same purpose as CompositeNode::childNodes, so Tree::toString can print the
  // decorator's child indented under it.
  // HINT: return a vector containing just the one child: {child_.get()}.
  SAPLING_TODO("DecoratorNode::childNodes");
}

void DecoratorNode::onHalt() {
  // Milestone 3
  // WHY: interrupting a decorator must interrupt what it wraps, or the child would keep
  // running unsupervised.
  // HINT: child_->halt(). (Retry/Repeat override this to reset a counter first, then
  // call this base version so the child is still halted.)
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
  // HINT: tick the child once: Status s = child_->tick();
  // Success -> return Failure, Failure -> return Success, anything else (Running)
  // -> return it unchanged.
  SAPLING_TODO("Inverter::onTick");
}

// -------------------------------------------------------------- ForceSuccess

ForceSuccess::ForceSuccess(NodePtr child, std::string name)
    : DecoratorNode(std::move(name), std::move(child)) {}

Status ForceSuccess::onTick() {
  // Milestone 3
  // WHY: for optional steps. "Try to wave at the person, but if waving fails that must not
  // stop the rest of the sequence." Wrapping the wave action in ForceSuccess does that.
  // HINT: tick the child once. Running -> return Running. Success or Failure ->
  // return Success. (isCompleted() from status.hpp is handy.)
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
  // HINT: attempts_ (member) counts failures so far; it survives across ticks so a
  // Running child does not reset it. Loop while attempts_ < max_attempts_:
  //   Status s = child_->tick();
  //   - Success: attempts_ = 0; return Success.
  //   - Running: return Running (keep attempts_; next tick continues).
  //   - Failure: ++attempts_ and loop to try again immediately (same tick).
  // Loop ended = ran out of attempts: attempts_ = 0; return Failure.
  // Check test RunningKeepsAttemptCount for exactly when the counter is incremented.
  SAPLING_TODO("Retry::onTick");
}

void Retry::onHalt() {
  // Milestone 3: reset the attempt counter and halt the child.
  // HINT: attempts_ = 0; then DecoratorNode::onHalt(); (qualified call to the base).
  SAPLING_TODO("Retry::onHalt");
}

// -------------------------------------------------------------------- Repeat

Repeat::Repeat(NodePtr child, int times, std::string name)
    : DecoratorNode(std::move(name), std::move(child)), times_(times) {}

Status Repeat::onTick() {
  // Milestone 3
  // WHY: "do this N times", e.g. wave three times. Retry's counterpart: Retry repeats
  // while the child FAILS, Repeat repeats while it SUCCEEDS.
  // HINT: count_ (member) counts successes so far. Loop while count_ < times_:
  //   Status s = child_->tick();
  //   - Failure: count_ = 0; return Failure.
  //   - Running: return Running (keep count_).
  //   - Success: ++count_ and loop again (same tick).
  // Loop ended = enough successes: count_ = 0; return Success.
  // Mirror image of Retry::onTick.
  SAPLING_TODO("Repeat::onTick");
}

void Repeat::onHalt() {
  // Milestone 3
  // HINT: count_ = 0; then DecoratorNode::onHalt();
  SAPLING_TODO("Repeat::onHalt");
}

}  // namespace sapling
