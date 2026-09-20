#include "sapling/composite.hpp"

#include <utility>

namespace sapling {

// Functions with a SAPLING_TODO line inside are yours to write. Everything else in this file is
// written for you: read it if you're curious, but you don't need to change it.

// ----------------------------------------------------------------- Composite

CompositeNode& CompositeNode::addChild(NodePtr child) {
  children_.push_back(std::move(child));
  return *this;
}

void CompositeNode::setBlackboard(Blackboard::Ptr bb) {
  // Milestone 1: set our own blackboard AND every child's.
  // WHY: a Sequence/Fallback has children, and those need the blackboard too. This is
  // what makes "give the tree one blackboard" (Tree constructor, M5) reach every node.
  // TODO (M1): give the blackboard to this node AND to every child. Replace the SAPLING_TODO line
  //     below with your code.
  //
  //     Two jobs. First, store the blackboard in this node. Node::setBlackboard already does that,
  //     so call it, with the class name in front. A bare setBlackboard(bb) here would call THIS
  //     function again, forever.
  //
  //     Second, children_ is a vector of shared_ptrs to the child nodes. Loop over it and call
  //     setBlackboard(bb) on each one. Children can be composites themselves, and their own
  //     setBlackboard passes it further down, which is how one call reaches the whole tree.
  //
  //         Node::setBlackboard(bb);
  //         for (auto& child : children_) {
  //             // give the blackboard to child
  //         }
  //
  SAPLING_TODO("CompositeNode::setBlackboard");
}

std::vector<const Node*> CompositeNode::childNodes() const {
  // Milestone 5
  // WHY: lets code look inside a tree without owning it. Tree::toString() uses it to
  // print the indented tree you see in the enter_room example's output.
  // TODO (M5): return pointers to all the children, in order. Replace the SAPLING_TODO line below
  //     with your code.
  //
  //     Tree::toString uses this to look inside the tree without owning it. The return type is a
  //     std::vector of plain `const Node*` pointers. Make an empty one and add one pointer per
  //     child. children_ holds shared_ptrs, and .get() on a shared_ptr gives you its plain pointer.
  //
  SAPLING_TODO("CompositeNode::childNodes");
}

void CompositeNode::haltChildren(std::size_t from) {
  // Milestone 2
  // WHY: shared helper used by Sequence, Fallback, and the M4 nodes. Whenever a control
  // node finishes or is interrupted, its children must be told to stop and reset so
  // nothing keeps running (or keeps stale state) behind the scenes.
  // TODO (M2): stop the children from position `from` onwards. Replace the SAPLING_TODO line below
  //     with your code.
  //
  //     children_ is the vector of child nodes. `from` is the first position to stop; its default
  //     (0, see composite.hpp) means stop all of them. Loop over the positions from `from` to the
  //     end of children_ and call halt() on each child. halt() does nothing on a child that isn't
  //     running, so don't check first.
  //
  //         for (std::size_t i = from; i < children_.size(); ++i) {
  //             // halt child i
  //         }
  //
  SAPLING_TODO("CompositeNode::haltChildren");
}

// ------------------------------------------------------------------ Sequence

Sequence::Sequence(std::string name) : CompositeNode(std::move(name)) {}

Status Sequence::onTick() {
  // Milestone 2
  // WHY: a Sequence is "do these steps IN ORDER, stop if one fails". In the enter_room
  // example the root is a Sequence: first get the door open, THEN enter. In the turtle
  // demo, "Patrol" is a Sequence: go to waypoint 0, then 1, then 2, then 3.
  // TODO (M2): tick the children one after another and stop at the first failure. Replace the
  //     SAPLING_TODO line below with your code.
  //
  //     current_ is the position of the child we are up to. It is a member, so it keeps its value
  //     between ticks: that is how the Sequence remembers where it was when a child said Running
  //     last time. It starts at 0.
  //
  //     Each tick, work through the children starting at current_. Ticking one
  //     (children_[current_]->tick()) gives you its Status, and what happens next depends on which
  //     of the three it is:
  //
  //       - Running: the Sequence is Running too. Return it straight away and do NOT change
  //         current_, so the next tick resumes at this same child.
  //       - Failure: the whole Sequence has failed. Stop all the children with haltChildren(),
  //         reset current_ to 0 so the next run starts from scratch, and return Failure.
  //       - Success: that step is done. Move current_ on to the next child and carry on.
  //
  //     If you get past the last child, every step succeeded: stop the children, reset current_,
  //     and return Success. A Sequence with no children never enters the loop and lands here too,
  //     which is what test EmptyIsSuccess expects.
  //
  //     A while loop that runs while current_ is a valid position fits well. Tests
  //     StopsAtFirstFailure, ResumesFromRunningChild and ChildrenAreIdleAfterCompleting show what
  //     is checked.
  //
  SAPLING_TODO("Sequence::onTick");
}

void Sequence::onHalt() {
  // Milestone 2: forget where we were, and halt the children.
  // WHY: if something interrupts the Sequence, it must not resume half-way through later.
  // TODO (M2): get ready for a fresh start after an interruption. Replace the SAPLING_TODO line
  //     below with your code.
  //
  //     Two lines. Put current_ back to 0 so the next run begins at the first child, and stop all
  //     the children with haltChildren().
  //
  SAPLING_TODO("Sequence::onHalt");
}

// ------------------------------------------------------------------ Fallback

Fallback::Fallback(std::string name) : CompositeNode(std::move(name)) {}

Status Fallback::onTick() {
  // Milestone 2
  // WHY: a Fallback is "try options IN ORDER until one works". enter_room uses one for
  // "get the door open": is it already open? else just open it? else pick the lock
  // first. The turtle uses one for "EnsureBattery": battery fine? else go recharge.
  // TODO (M2): try the children in order until one works. Replace the SAPLING_TODO line below with
  //     your code.
  //
  //     This is Sequence::onTick with the roles of Success and Failure swapped: a Sequence needs
  //     every child to succeed, a Fallback only needs the first one that does. Same members
  //     (current_, children_), same loop, same haltChildren() calls, same Running rule.
  //
  //     Work out for yourself what each of the three answers now means, using the comment above
  //     class Fallback in composite.hpp. When you get to the end, they all failed. A Fallback with
  //     no children lands there too, which test EmptyIsFailure expects.
  //
  SAPLING_TODO("Fallback::onTick");
}

void Fallback::onHalt() {
  // Milestone 2
  // WHY: same reason as Sequence::onHalt: start fresh after an interruption.
  // TODO (M2): same as Sequence::onHalt. Replace the SAPLING_TODO line below with your code.
  //
  //     Reset current_ and stop all the children.
  //
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
  // TODO (M4): like Sequence, but with no memory: every tick starts again from the first child.
  //     Replace the SAPLING_TODO line below with your code.
  //
  //     This is the idea the whole project is built around. A plain Sequence remembers where it got
  //     to (current_) and skips its earlier children. A ReactiveSequence has no current_: each tick
  //     it re-runs the children from the top, so a condition placed first (like 'is the battery
  //     OK?') is re-checked all the time, even while a later child is busy driving.
  //
  //     Go through the children in order from position 0. For each child's answer, decide what to
  //     do with the Running case, the Failure case and the Success case. The extra work is stopping
  //     the right children: haltChildren(i + 1) stops everything after position i, and
  //     haltChildren() stops everything.
  //
  //     Think about what should be stopped when an early child suddenly fails while a later one is
  //     mid-action (test FailingConditionInterruptsRunningAction), and when an early child is
  //     Running so the later ones shouldn't be (test RunningChildHaltsLaterChildren). The comment
  //     above ReactiveSequence in composite.hpp spells out the three rules.
  //
  SAPLING_TODO("ReactiveSequence::onTick");
}

// ---------------------------------------------------------- ReactiveFallback

ReactiveFallback::ReactiveFallback(std::string name) : CompositeNode(std::move(name)) {}

Status ReactiveFallback::onTick() {
  // Milestone 4
  // WHY: the mirror of ReactiveSequence: "keep trying option 2 only while option 1 is
  // still failing". Not used by the turtle demo, but it completes the set and shows
  // the same re-check-every-tick idea from the other direction.
  // TODO (M4): the mirror image of ReactiveSequence. Replace the SAPLING_TODO line below with your
  //     code.
  //
  //     No memory, start at the first child every tick, with Success and Failure swapped as they
  //     were between Sequence and Fallback in M2. Test SuccessOfEarlierChildInterruptsRunningAction
  //     shows the interruption case.
  //
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
  // TODO (M4): tick all the unfinished children every tick and decide the result from the counts.
  //     Replace the SAPLING_TODO line below with your code.
  //
  //     results_ holds one Status per child and remembers them between ticks. If its size doesn't
  //     match children_ (first tick, or after a reset), set it up with one Status::Idle per child,
  //     which here means 'not finished yet'.
  //
  //     Tick every child that hasn't finished. isCompleted(status) (in status.hpp) is true for
  //     Success and Failure, so use it to skip finished children. Don't stop early: the tests check
  //     that all pending children are ticked before deciding. Store each answer in results_.
  //
  //     Then count the successes and the failures in results_. success_threshold_ is how many
  //     children must succeed. If successes reach it, the Parallel succeeds. If it can no longer be
  //     reached (failures + success_threshold_ is more than children_.size()) it fails. Otherwise
  //     it is Running.
  //
  //     When it succeeds or fails, stop the children that are still running and clear results_ so
  //     the next run starts fresh. The rules are in the comment above class Parallel in
  //     composite.hpp.
  //
  SAPLING_TODO("Parallel::onTick");
}

void Parallel::onHalt() {
  // Milestone 4
  // WHY: interrupting a Parallel must stop ALL of its running children.
  // TODO (M4): stop everything and forget the results. Replace the SAPLING_TODO line below with
  //     your code.
  //
  //     Empty results_ and stop all the children.
  //
  SAPLING_TODO("Parallel::onHalt");
}

}  // namespace sapling
