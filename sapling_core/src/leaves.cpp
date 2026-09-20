#include "sapling/leaves.hpp"

#include <utility>

namespace sapling {

Condition::Condition(std::string name, Fn fn) : Node(std::move(name)), fn_(std::move(fn)) {}

Status Condition::onTick() {
  // Milestone 1
  // WHY: a Condition is a yes/no question the tree asks the world, e.g. "is the door
  // open?" (M5 example) or "is the battery above 25%?" (M6 turtle). Conditions are
  // what let a tree branch: a Sequence carries on only if the condition says Success,
  // a Fallback moves to its next option if it says Failure. This function converts the
  // user's plain true/false lambda into the tree's Success/Failure language.
  // HINT:
  //  1. blackboard() returns a shared_ptr. If it is null, no Tree/parent attached one:
  //     throw std::logic_error("...") (test LeafWithoutBlackboardThrows needs this;
  //     you will need #include <stdexcept>).
  //  2. Call fn_ with *blackboard() (fn_ takes Blackboard&, so dereference the pointer).
  //  3. fn_ returns bool: true -> Status::Success, false -> Status::Failure.
  //     A Condition never returns Running.
  SAPLING_TODO("Condition::onTick");
}

Action::Action(std::string name, Fn fn) : Node(std::move(name)), fn_(std::move(fn)) {}

Status Action::onTick() {
  // Milestone 1
  // WHY: an Action is where the tree actually DOES something: "open the door", "pick
  // the lock", "enter the room". Together with Condition these are the leaves, the only
  // nodes that touch the outside world; everything else in the tree just decides which
  // leaf runs next. The user supplies the behavior as a lambda and this function runs it.
  // HINT: same as Condition::onTick, but simpler on step 3: fn_ already returns a
  // Status, so return it as-is (that is how an Action can report Running).
  SAPLING_TODO("Action::onTick");
}

}  // namespace sapling
