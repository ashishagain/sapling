// The classic behavior tree example: a robot that wants to enter a room.
//
//   Sequence Root
//   ├── Fallback GetDoorOpen
//   │   ├── Condition IsDoorOpen
//   │   ├── Action    OpenDoor
//   │   └── Sequence  UnlockThenOpen
//   │       ├── Retry(5) PickLock
//   │       └── Action   OpenDoor
//   └── Action EnterRoom
//
// Run it once your Milestone 1-5 tests pass:  ./build/enter_room_example
#include <iostream>

#include "sapling/sapling.hpp"

using namespace sapling;

int main() {
  auto bb = std::make_shared<Blackboard>();
  bb->set<bool>("door_open", false);
  bb->set<bool>("door_locked", true);
  bb->set<int>("attempts", 0);

  auto say = [](const std::string& msg) { std::cout << "  " << msg << '\n'; };

  auto make_open_door = [say] {
    return std::make_shared<Action>("OpenDoor", [say](Blackboard& b) {
      if (b.get<bool>("door_locked").value_or(true)) {
        say("OpenDoor: it's locked");
        return Status::Failure;
      }
      say("OpenDoor: opened");
      b.set<bool>("door_open", true);
      return Status::Success;
    });
  };

  auto is_open = std::make_shared<Condition>(
      "IsDoorOpen", [](Blackboard& b) { return b.get<bool>("door_open").value_or(false); });

  auto pick_lock = std::make_shared<Action>("PickLock", [say](Blackboard& b) {
    int n = b.get<int>("attempts").value_or(0) + 1;
    b.set<int>("attempts", n);
    if (n < 3) {
      say("PickLock: attempt " + std::to_string(n) + " failed");
      return Status::Failure;
    }
    say("PickLock: click!");
    b.set<bool>("door_locked", false);
    return Status::Success;
  });

  auto unlock_then_open = std::make_shared<Sequence>("UnlockThenOpen");
  unlock_then_open->addChild(std::make_shared<Retry>(pick_lock, 5, "Retry(5)"))
      .addChild(make_open_door());

  auto get_door_open = std::make_shared<Fallback>("GetDoorOpen");
  get_door_open->addChild(is_open).addChild(make_open_door()).addChild(unlock_then_open);

  auto enter = std::make_shared<Action>("EnterRoom", [say](Blackboard&) {
    say("EnterRoom: I'm in!");
    return Status::Success;
  });

  auto root = std::make_shared<Sequence>("Root");
  root->addChild(get_door_open).addChild(enter);

  Tree tree(root, bb);
  std::cout << "Tree:\n" << tree.toString() << "\nRunning:\n";
  Status result = tree.tickWhileRunning();
  std::cout << "\nResult: " << result << '\n';
  return result == Status::Success ? 0 : 1;
}
