#pragma once
#include <functional>

#include "sapling/node.hpp"

namespace sapling {

// Milestone 1.
// Leaves are where the tree touches the real world.
//
// In plain words: every other node just decides which leaf runs next. The leaves are
// the ones that ask a question ("is the door open?", Condition) or do something ("open
// the door", Action). Both take a lambda from the user, so you can build leaves without
// writing a new class each time; look at how enter_room.cpp does it.
//
// For quick experiments you can wrap a lambda in Action / Condition.
// For real robot behaviours that take time and need clean-up on halt
// (like "drive to a waypoint"), subclass Node directly and implement
// onTick() and onHalt() yourself - see sapling_turtle for examples.

// A condition asks a question. It returns Success (true) or Failure (false)
// and never Running.
// Throw std::logic_error if no blackboard has been attached.
class Condition : public Node {
 public:
  using Fn = std::function<bool(Blackboard&)>;
  Condition(std::string name, Fn fn);

 protected:
  Status onTick() override;

 private:
  Fn fn_;
};

// An action does something. The lambda returns the status directly, so it
// may return Running to be ticked again later.
// Throw std::logic_error if no blackboard has been attached.
class Action : public Node {
 public:
  using Fn = std::function<Status(Blackboard&)>;
  Action(std::string name, Fn fn);

 protected:
  Status onTick() override;

 private:
  Fn fn_;
};

}  // namespace sapling
