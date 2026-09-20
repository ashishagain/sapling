#include "sapling/leaves.hpp"

#include <utility>

namespace sapling {

Condition::Condition(std::string name, Fn fn) : Node(std::move(name)), fn_(std::move(fn)) {}

Status Condition::onTick() {
  // Milestone 1
  SAPLING_TODO("Condition::onTick");
}

Action::Action(std::string name, Fn fn) : Node(std::move(name)), fn_(std::move(fn)) {}

Status Action::onTick() {
  // Milestone 1
  SAPLING_TODO("Action::onTick");
}

}  // namespace sapling
