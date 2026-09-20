#include "sapling/leaves.hpp"

#include <utility>

namespace sapling {

// Functions with a SAPLING_TODO line inside are yours to write. Everything else in this file is
// written for you: read it if you're curious, but you don't need to change it.

Condition::Condition(std::string name, Fn fn) : Node(std::move(name)), fn_(std::move(fn)) {}

Status Condition::onTick() {
  // Milestone 1
  // WHY: a Condition is a yes/no question the tree asks the world, e.g. "is the door
  // open?" (M5 example) or "is the battery above 25%?" (M6 turtle). Conditions are
  // what let a tree branch: a Sequence carries on only if the condition says Success,
  // a Fallback moves to its next option if it says Failure. This function converts the
  // user's plain true/false lambda into the tree's Success/Failure language.
  // TODO (M1): run the user's yes/no question and turn the answer into Success or Failure. Replace
  //     the SAPLING_TODO line below with your code.
  //
  //     fn_ is the function the user gave when creating the Condition. In enter_room.cpp it is
  //     `[](Blackboard& b) { return b.get<bool>("door_open").value_or(false); }`: it takes the
  //     blackboard and returns true or false.
  //
  //     First check a blackboard is attached. blackboard() returns a std::shared_ptr, which is null
  //     if nobody has called setBlackboard yet. In that case throw a std::logic_error (test
  //     LeafWithoutBlackboardThrows checks this). Add #include <stdexcept> at the top of this file.
  //
  //         if (!blackboard()) {
  //             throw std::logic_error("Condition '" + name() + "' has no blackboard");
  //         }
  //
  //     To call fn_ you pass it the blackboard itself, not the pointer, so write *blackboard().
  //     Then true means Status::Success and false means Status::Failure. A Condition never returns
  //     Running.
  //
  SAPLING_TODO("Condition::onTick");
}

Action::Action(std::string name, Fn fn) : Node(std::move(name)), fn_(std::move(fn)) {}

Status Action::onTick() {
  // Milestone 1
  // WHY: an Action is where the tree actually DOES something: "open the door", "pick
  // the lock", "enter the room". Together with Condition these are the leaves, the only
  // nodes that touch the outside world; everything else in the tree just decides which
  // leaf runs next. The user supplies the behavior as a lambda and this function runs it.
  // TODO (M1): run the user's action and return its Status. Replace the SAPLING_TODO line below
  //     with your code.
  //
  //     This is Condition::onTick again with one simplification. Do the same null-blackboard check
  //     first and throw the same kind of std::logic_error.
  //
  //     Then call fn_ with *blackboard(). Unlike a Condition's, an Action's lambda already returns
  //     a Status, so return it as it is. That is how an action gets to say Running.
  //
  SAPLING_TODO("Action::onTick");
}

}  // namespace sapling
