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
    // TODO (M1): store `value` in the blackboard under the name `key`. Replace the SAPLING_TODO
    //     line below with your code.
    //
    //     storage_ (at the bottom of this class) is the shelf of labelled boxes. It maps a name to
    //     a std::any, a box that can hold a value of any type. `key` is the label and `value` is
    //     what goes in the box.
    //
    //     Putting something on the shelf is one line, and you index the map with square brackets,
    //     like a Python dictionary. If the label is already in use the old contents are simply
    //     replaced, even if they were a different type, so there is nothing else to handle.
    //
    //         storage_[key] = value;
    //
    SAPLING_TODO("Blackboard::set");
  }

  // Return the value stored under `key`, or std::nullopt if the key is
  // missing OR holds a value of a different type.
  // USED BY: conditions and actions reading facts, e.g. the BatteryOk check reads
  // "battery". It returns "maybe a value" rather than crashing, because a key may
  // not exist yet (the turtle's first pose message may not have arrived).
  template <typename T>
  std::optional<T> get(const std::string& key) const {
    // TODO (M1): look up `key` and return its value, or an empty optional if there is none. Replace
    //     the SAPLING_TODO line below with your code.
    //
    //     It can come up empty in two ways: the label isn't on the shelf at all, or it is, but the
    //     box holds a different type from T (you asked for an int but someone stored a string). In
    //     both cases return std::nullopt, which means 'no value'. That is why the return type is
    //     std::optional<T> rather than plain T.
    //
    //     Step 1, is the label there? Ask the map to find it and compare the answer with end(),
    //     which means 'not found':
    //
    //         auto it = storage_.find(key);
    //         if (it == storage_.end()) return std::nullopt;
    //
    //     Step 2, what is in the box? `it->second` is the std::any. Ask it whether it really holds
    //     a T. Use the pointer form of any_cast (note the &): it gives back a pointer to the T, or
    //     nullptr if the box holds something else. The non-pointer form would throw an error
    //     instead, which you don't want here:
    //
    //         const T* p = std::any_cast<T>(&it->second);
    //
    //     Step 3 is yours: if p is nullptr, return the 'no value' marker. Otherwise return the
    //     value p points at (a * in front of p reads the value through the pointer).
    //
    SAPLING_TODO("Blackboard::get");
  }

  // True if anything is stored under `key`.
  // USED BY: code that needs to know "has this been set yet?" without caring about the type.
  bool has(const std::string& key) const {
    // TODO (M1): return whether `key` is on the shelf. Replace the SAPLING_TODO line below with
    //     your code.
    //
    //     You used find() and end() in get(). Same two here, but you only need a true/false answer.
    //     (storage_.count(key) also works.)
    //
    SAPLING_TODO("Blackboard::has");
  }

  void erase(const std::string& key) {
    // TODO (M1): remove `key` from the shelf. Replace the SAPLING_TODO line below with your code.
    //
    //     std::unordered_map has an erase() function that takes the key. It does nothing if the key
    //     isn't there, so you don't need to check first.
    //
    //         storage_.erase(key);
    //
    SAPLING_TODO("Blackboard::erase");
  }

 private:
  std::unordered_map<std::string, std::any> storage_;
};

}  // namespace sapling
