#include "sapling/composite.hpp"

#include <utility>

namespace sapling {

// ----------------------------------------------------------------- Composite

CompositeNode& CompositeNode::addChild(NodePtr child) {
  children_.push_back(std::move(child));
  return *this;
}

void CompositeNode::setBlackboard(Blackboard::Ptr bb) {
  // Milestone 1: set our own blackboard AND every child's.
  // WHY: a Sequence/Fallback has children, and those need the blackboard too. This is
  // what makes "give the tree one blackboard" (Tree constructor, M5) reach every node.
  // HINT: two steps. (1) Node::setBlackboard(bb) stores it in this node: call the base
  // version explicitly by its qualified name, since a plain setBlackboard(bb) here
  // would call this function again forever. (2) loop over children_ and call
  // child->setBlackboard(bb) on each. Children can be composites themselves, so
  // this recursion is what reaches the whole tree.
  SAPLING_TODO("CompositeNode::setBlackboard");
}

std::vector<const Node*> CompositeNode::childNodes() const {
  // Milestone 5
  // WHY: lets code look inside a tree without owning it. Tree::toString() uses it to
  // print the indented tree you see in the enter_room example's output.
  // HINT: build a std::vector<const Node*> with one raw pointer per child, in order.
  // child.get() gives the raw pointer from a shared_ptr. Tree::toString uses this.
  SAPLING_TODO("CompositeNode::childNodes");
}

void CompositeNode::haltChildren(std::size_t from) {
  // Milestone 2
  // WHY: shared helper used by Sequence, Fallback, and the M4 nodes. Whenever a control
  // node finishes or is interrupted, its children must be told to stop and reset so
  // nothing keeps running (or keeps stale state) behind the scenes.
  // HINT: for i from `from` to children_.size(), call children_[i]->halt().
  // halt() is safe on any child (it only runs onHalt() if that child was Running),
  // so no status checks needed. Note the default argument: haltChildren() halts ALL.
  SAPLING_TODO("CompositeNode::haltChildren");
}

// ------------------------------------------------------------------ Sequence

Sequence::Sequence(std::string name) : CompositeNode(std::move(name)) {}

Status Sequence::onTick() {
  // Milestone 2
  // WHY: a Sequence is "do these steps IN ORDER, stop if one fails". In the enter_room
  // example the root is a Sequence: first get the door open, THEN enter. In the turtle
  // demo, "Patrol" is a Sequence: go to waypoint 0, then 1, then 2, then 3.
  // HINT: current_ (a member) is the index of the child to tick; it persists across
  // ticks, which is the "memory". Loop from current_ to the end:
  //   Status s = children_[current_]->tick();
  //   - Running: return Running (do NOT advance; next tick resumes at this child).
  //   - Failure: haltChildren(); current_ = 0; return Failure.
  //   - Success: ++current_ and go on to the next child.
  // Fell out of the loop = every child succeeded: haltChildren(); current_ = 0;
  // return Success. (haltChildren() also resets finished children to Idle, which
  // test ChildrenAreIdleAfterCompleting checks.) Zero children skips the loop and
  // lands on Success, as required. Tip: use a `while (current_ < children_.size())`.
  SAPLING_TODO("Sequence::onTick");
}

void Sequence::onHalt() {
  // Milestone 2: forget where we were, and halt the children.
  // WHY: if something interrupts the Sequence, it must not resume half-way through later.
  // HINT: two lines: current_ = 0; and haltChildren();
  SAPLING_TODO("Sequence::onHalt");
}

// ------------------------------------------------------------------ Fallback

Fallback::Fallback(std::string name) : CompositeNode(std::move(name)) {}

Status Fallback::onTick() {
  // Milestone 2
  // WHY: a Fallback is "try options IN ORDER until one works". enter_room uses one for
  // "get the door open": is it already open? else just open it? else pick the lock
  // first. The turtle uses one for "EnsureBattery": battery fine? else go recharge.
  // HINT: the mirror of Sequence::onTick, with Success and Failure swapped.
  //   - Running: return Running (keep current_).
  //   - Success: haltChildren(); current_ = 0; return Success   (first success wins).
  //   - Failure: ++current_ and try the next child.
  // All children failed: haltChildren(); current_ = 0; return Failure.
  // No children -> falls through -> Failure, as required.
  SAPLING_TODO("Fallback::onTick");
}

void Fallback::onHalt() {
  // Milestone 2
  // WHY: same reason as Sequence::onHalt: start fresh after an interruption.
  // HINT: identical to Sequence::onHalt: current_ = 0; haltChildren();
  SAPLING_TODO("Fallback::onHalt");
}

// ---------------------------------------------------------- ReactiveSequence

ReactiveSequence::ReactiveSequence(std::string name) : CompositeNode(std::move(name)) {}

Status ReactiveSequence::onTick() {
  // Milestone 4
  // WHY: THE key idea of the project. The turtle demo's root is a ReactiveSequence:
  // "battery OK?" comes first and is re-checked on EVERY tick, even while the turtle is
  // mid-drive. The moment the battery goes low the check fails, the running MoveTo is
  // halted, and the tree switches to recharging. A plain Sequence would never re-check,
  // and the turtle would drive until it ran flat.
  // HINT: no memory and no current_ member: ALWAYS start at child 0. For each child i:
  //   Status s = children_[i]->tick();
  //   - Running: haltChildren(i + 1) (stop anything after it that was running,
  //     see test RunningChildHaltsLaterChildren); return Running.
  //   - Failure: haltChildren() (ALL, this is what interrupts a running action);
  //     return Failure.
  //   - Success: continue to the next child.
  // All succeeded: haltChildren(); return Success.
  // Compare with Sequence: same shape, but the loop starts at 0 every tick.
  SAPLING_TODO("ReactiveSequence::onTick");
}

// ---------------------------------------------------------- ReactiveFallback

ReactiveFallback::ReactiveFallback(std::string name) : CompositeNode(std::move(name)) {}

Status ReactiveFallback::onTick() {
  // Milestone 4
  // WHY: the mirror of ReactiveSequence: "keep trying option 2 only while option 1 is
  // still failing". Not used by the turtle demo, but it completes the set and shows
  // the same re-check-every-tick idea from the other direction.
  // HINT: mirror of ReactiveSequence. Always start at child 0.
  //   - Running: haltChildren(i + 1); return Running.
  //   - Success: haltChildren(); return Success (interrupts a later running child).
  //   - Failure: continue to the next child.
  // All failed: haltChildren(); return Failure.
  SAPLING_TODO("ReactiveFallback::onTick");
}

// ------------------------------------------------------------------ Parallel

Parallel::Parallel(std::size_t success_threshold, std::string name)
    : CompositeNode(std::move(name)), success_threshold_(success_threshold) {}

Status Parallel::onTick() {
  // Milestone 4
  // WHY: runs several children at the same time (e.g. "drive AND play a sound"),
  // succeeding when enough of them have. Not used by the turtle demo, but it is the
  // standard way a BT does two things at once.
  // HINT: results_ (member vector<Status>) remembers each child's outcome.
  //  1. If results_.size() != children_.size(), resize it, filling with Status::Idle
  //     ("not finished yet"). This handles the first tick after a reset.
  //  2. Tick EVERY child whose results_[i] is not Success/Failure yet (isCompleted()
  //     in status.hpp helps). Store what it returns in results_[i]. Do not stop early:
  //     the tests check all pending children get ticked before deciding.
  //  3. Count successes and failures in results_.
  //  4. successes >= success_threshold_        -> Success.
  //     failures + success_threshold_ > children_.size()   -> Failure (threshold can
  //     no longer be reached). This is "failures > size - threshold" rearranged to avoid
  //     subtracting size_t values, which would wrap around if threshold > size.
  //  5. On Success or Failure: haltChildren() (stops children still Running), clear
  //     results_, and return that status. Otherwise return Running.
  SAPLING_TODO("Parallel::onTick");
}

void Parallel::onHalt() {
  // Milestone 4
  // WHY: interrupting a Parallel must stop ALL of its running children.
  // HINT: results_.clear(); haltChildren();
  SAPLING_TODO("Parallel::onHalt");
}

}  // namespace sapling
