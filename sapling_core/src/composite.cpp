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
  SAPLING_TODO("CompositeNode::setBlackboard");
}

std::vector<const Node*> CompositeNode::childNodes() const {
  // Milestone 5
  SAPLING_TODO("CompositeNode::childNodes");
}

void CompositeNode::haltChildren(std::size_t from) {
  // Milestone 2
  SAPLING_TODO("CompositeNode::haltChildren");
}

// ------------------------------------------------------------------ Sequence

Sequence::Sequence(std::string name) : CompositeNode(std::move(name)) {}

Status Sequence::onTick() {
  // Milestone 2
  SAPLING_TODO("Sequence::onTick");
}

void Sequence::onHalt() {
  // Milestone 2: forget where we were, and halt the children.
  SAPLING_TODO("Sequence::onHalt");
}

// ------------------------------------------------------------------ Fallback

Fallback::Fallback(std::string name) : CompositeNode(std::move(name)) {}

Status Fallback::onTick() {
  // Milestone 2
  SAPLING_TODO("Fallback::onTick");
}

void Fallback::onHalt() {
  // Milestone 2
  SAPLING_TODO("Fallback::onHalt");
}

// ---------------------------------------------------------- ReactiveSequence

ReactiveSequence::ReactiveSequence(std::string name) : CompositeNode(std::move(name)) {}

Status ReactiveSequence::onTick() {
  // Milestone 4
  SAPLING_TODO("ReactiveSequence::onTick");
}

// ---------------------------------------------------------- ReactiveFallback

ReactiveFallback::ReactiveFallback(std::string name) : CompositeNode(std::move(name)) {}

Status ReactiveFallback::onTick() {
  // Milestone 4
  SAPLING_TODO("ReactiveFallback::onTick");
}

// ------------------------------------------------------------------ Parallel

Parallel::Parallel(std::size_t success_threshold, std::string name)
    : CompositeNode(std::move(name)), success_threshold_(success_threshold) {}

Status Parallel::onTick() {
  // Milestone 4
  SAPLING_TODO("Parallel::onTick");
}

void Parallel::onHalt() {
  // Milestone 4
  SAPLING_TODO("Parallel::onHalt");
}

}  // namespace sapling
