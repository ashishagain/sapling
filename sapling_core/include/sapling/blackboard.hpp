#pragma once
#include <any>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "sapling/todo.hpp"

namespace sapling {

// Milestone 1.
// A blackboard is a typed key/value store shared by every node in a tree.
// Nodes use it to pass data around (e.g. "target_pose", "battery_level")
// without knowing about each other.
//
// Hint: std::any can hold a value of any type, and std::any_cast<T> gets it
// back out (throwing std::bad_any_cast if the type is wrong). The pointer
// overload, std::any_cast<T>(&some_any), returns nullptr instead of throwing.
class Blackboard {
 public:
  using Ptr = std::shared_ptr<Blackboard>;

  // Store `value` under `key`, replacing anything already there
  // (even if the old value had a different type).
  template <typename T>
  void set(const std::string& key, T value) {
    SAPLING_TODO("Blackboard::set");
  }

  // Return the value stored under `key`, or std::nullopt if the key is
  // missing OR holds a value of a different type.
  template <typename T>
  std::optional<T> get(const std::string& key) const {
    SAPLING_TODO("Blackboard::get");
  }

  // True if anything is stored under `key`.
  bool has(const std::string& key) const {
    SAPLING_TODO("Blackboard::has");
  }

  void erase(const std::string& key) {
    SAPLING_TODO("Blackboard::erase");
  }

 private:
  std::unordered_map<std::string, std::any> storage_;
};

}  // namespace sapling
