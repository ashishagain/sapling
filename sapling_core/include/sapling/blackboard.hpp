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
// WHY IT EXISTS: tree nodes are small, separate pieces that must not know about
// each other, yet they need shared facts. Think of a whiteboard in a room that
// everyone can read and write. In the finished project:
//   - enter_room example (M5): the "PickLock" action writes door_locked=false, and
//     the "OpenDoor" action reads it. Neither knows the other exists.
//   - turtle patrol (M6): the ROS node writes the turtle's "pose" and "battery",
//     and the BatteryOk condition / MoveTo action read them. The whole tree
//     "sees" the robot only through this class.
// Everything in this file is used by every later milestone, so get it solid first.
//
// Hint: std::any can hold a value of any type, and std::any_cast<T> gets it
// back out (throwing std::bad_any_cast if the type is wrong). The pointer
// overload, std::any_cast<T>(&some_any), returns nullptr instead of throwing.
class Blackboard {
 public:
  using Ptr = std::shared_ptr<Blackboard>;

  // Store `value` under `key`, replacing anything already there
  // (even if the old value had a different type).
  // USED BY: actions publishing results, e.g. b.set<bool>("door_open", true).
  template <typename T>
  void set(const std::string& key, T value) {
    // HINT: storage_ maps string -> std::any. Assigning a T into a std::any wraps it
    // (std::any remembers the type). storage_[key] = ... replaces any old value,
    // even one of a different type, so there is nothing else to handle. One line.
    SAPLING_TODO("Blackboard::set");
  }

  // Return the value stored under `key`, or std::nullopt if the key is
  // missing OR holds a value of a different type.
  // USED BY: conditions and actions reading facts, e.g. the BatteryOk check reads
  // "battery". It returns "maybe a value" rather than crashing, because a key may
  // not exist yet (the turtle's first pose message may not have arrived).
  template <typename T>
  std::optional<T> get(const std::string& key) const {
    // HINT, three steps:
    //  1. storage_.find(key). Not found? return std::nullopt. (Don't use operator[]:
    //     it is non-const and would insert an empty entry.)
    //  2. Found: it->second is the std::any. Use the POINTER form
    //     std::any_cast<T>(&it->second): it returns T* or nullptr, never throws.
    //  3. nullptr means the stored type was not T -> return std::nullopt.
    //     Otherwise return the dereferenced value (copy it into the optional).
    SAPLING_TODO("Blackboard::get");
  }

  // True if anything is stored under `key`.
  // USED BY: code that needs to know "has this been set yet?" without caring about the type.
  bool has(const std::string& key) const {
    // HINT: is the key in storage_? (find() != end(), or count(), or contains() in C++20.)
    SAPLING_TODO("Blackboard::has");
  }

  void erase(const std::string& key) {
    // HINT: unordered_map::erase(key) already does nothing if the key is missing.
    SAPLING_TODO("Blackboard::erase");
  }

 private:
  std::unordered_map<std::string, std::any> storage_;
};

}  // namespace sapling
