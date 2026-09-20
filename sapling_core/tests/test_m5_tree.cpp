#include <gtest/gtest.h>

#include "test_helpers.hpp"

using namespace sapling;
using namespace sapling::testing;

TEST(M5Tree, AttachesBlackboardToAllNodes) {
  auto a = scripted("A", {S});
  auto inner = std::make_shared<Fallback>("Inner");
  inner->addChild(a);
  auto root = std::make_shared<Sequence>("Root");
  root->addChild(inner);

  auto bb = std::make_shared<Blackboard>();
  Tree tree(root, bb);
  EXPECT_EQ(a->blackboard(), bb);
  EXPECT_EQ(&tree.blackboard(), bb.get());
}

TEST(M5Tree, TickOnce) {
  auto a = scripted("A", {R, S});
  Tree tree(a);
  EXPECT_EQ(tree.tickOnce(), Status::Running);
  EXPECT_EQ(tree.tickOnce(), Status::Success);
}

TEST(M5Tree, TickWhileRunning) {
  auto a = scripted("A", {R, R, R, S});
  Tree tree(a);
  EXPECT_EQ(tree.tickWhileRunning(), Status::Success);
  EXPECT_EQ(a->ticks, 4);
}

TEST(M5Tree, Halt) {
  auto a = scripted("A", {R});
  auto root = std::make_shared<Sequence>();
  root->addChild(a);
  Tree tree(root);
  tree.tickOnce();
  tree.halt();
  EXPECT_EQ(a->halts, 1);
  EXPECT_EQ(root->status(), Status::Idle);
}

TEST(M5Tree, ToString) {
  auto sub = std::make_shared<Fallback>("Sub");
  sub->addChild(scripted("B", {S}));
  auto root = std::make_shared<Sequence>("Root");
  root->addChild(scripted("A", {S})).addChild(sub).addChild(std::make_shared<Inverter>(scripted("C", {S})));

  Tree tree(root);
  EXPECT_EQ(tree.toString(),
            "Root\n"
            "  A\n"
            "  Sub\n"
            "    B\n"
            "  Inverter\n"
            "    C\n");
}

// The classic "enter the room" tree, end to end, driven by the blackboard.
TEST(M5Tree, EnterRoomScenario) {
  auto bb = std::make_shared<Blackboard>();
  bb->set<bool>("door_open", false);
  bb->set<bool>("door_locked", true);
  bb->set<int>("lockpick_attempts", 0);

  auto is_open = std::make_shared<Condition>("IsDoorOpen", [](Blackboard& b) { return *b.get<bool>("door_open"); });
  auto make_open_door = [] {
    return std::make_shared<Action>("OpenDoor", [](Blackboard& b) {
      if (*b.get<bool>("door_locked")) return Status::Failure;
      b.set<bool>("door_open", true);
      return Status::Success;
    });
  };
  auto pick_lock = std::make_shared<Action>("PickLock", [](Blackboard& b) {
    int n = *b.get<int>("lockpick_attempts") + 1;
    b.set<int>("lockpick_attempts", n);
    if (n < 3) return Status::Failure;
    b.set<bool>("door_locked", false);
    return Status::Success;
  });
  auto unlock_then_open = std::make_shared<Sequence>("UnlockThenOpen");
  unlock_then_open->addChild(std::make_shared<Retry>(pick_lock, 5)).addChild(make_open_door());

  auto get_door_open = std::make_shared<Fallback>("GetDoorOpen");
  get_door_open->addChild(is_open).addChild(make_open_door()).addChild(unlock_then_open);

  bool entered = false;
  auto enter = std::make_shared<Action>("EnterRoom", [&entered](Blackboard&) {
    entered = true;
    return Status::Success;
  });
  auto root = std::make_shared<Sequence>("Root");
  root->addChild(get_door_open).addChild(enter);

  Tree tree(root, bb);
  EXPECT_EQ(tree.tickWhileRunning(), Status::Success);
  EXPECT_TRUE(entered);
  EXPECT_EQ(*bb->get<int>("lockpick_attempts"), 3);
}
