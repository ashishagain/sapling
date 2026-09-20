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
  SAPLING_TODO("DecoratorNode::setBlackboard");
}

std::vector<const Node*> DecoratorNode::childNodes() const {
  // Milestone 5
  SAPLING_TODO("DecoratorNode::childNodes");
}

void DecoratorNode::onHalt() {
  // Milestone 3
  SAPLING_TODO("DecoratorNode::onHalt");
}

// ------------------------------------------------------------------ Inverter

Inverter::Inverter(NodePtr child, std::string name)
    : DecoratorNode(std::move(name), std::move(child)) {}

Status Inverter::onTick() {
  // Milestone 3
  SAPLING_TODO("Inverter::onTick");
}

// -------------------------------------------------------------- ForceSuccess

ForceSuccess::ForceSuccess(NodePtr child, std::string name)
    : DecoratorNode(std::move(name), std::move(child)) {}

Status ForceSuccess::onTick() {
  // Milestone 3
  SAPLING_TODO("ForceSuccess::onTick");
}

// --------------------------------------------------------------------- Retry

Retry::Retry(NodePtr child, int max_attempts, std::string name)
    : DecoratorNode(std::move(name), std::move(child)), max_attempts_(max_attempts) {}

Status Retry::onTick() {
  // Milestone 3
  SAPLING_TODO("Retry::onTick");
}

void Retry::onHalt() {
  // Milestone 3: reset the attempt counter and halt the child.
  SAPLING_TODO("Retry::onHalt");
}

// -------------------------------------------------------------------- Repeat

Repeat::Repeat(NodePtr child, int times, std::string name)
    : DecoratorNode(std::move(name), std::move(child)), times_(times) {}

Status Repeat::onTick() {
  // Milestone 3
  SAPLING_TODO("Repeat::onTick");
}

void Repeat::onHalt() {
  // Milestone 3
  SAPLING_TODO("Repeat::onHalt");
}

}  // namespace sapling
