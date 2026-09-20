#include <gtest/gtest.h>

#include "test_helpers.hpp"

using namespace sapling;
using namespace sapling::testing;

// ------------------------------------------------------------------ Blackboard

TEST(M1Blackboard, SetThenGet) {
  Blackboard bb;
  bb.set<int>("answer", 42);
  ASSERT_TRUE(bb.get<int>("answer").has_value());
  EXPECT_EQ(*bb.get<int>("answer"), 42);
}

TEST(M1Blackboard, MissingKeyIsNullopt) {
  Blackboard bb;
  EXPECT_FALSE(bb.get<int>("nope").has_value());
  EXPECT_FALSE(bb.has("nope"));
}

TEST(M1Blackboard, WrongTypeIsNullopt) {
  Blackboard bb;
  bb.set<int>("x", 1);
  EXPECT_FALSE(bb.get<std::string>("x").has_value());
}

TEST(M1Blackboard, OverwriteWithDifferentType) {
  Blackboard bb;
  bb.set<int>("x", 1);
  bb.set<std::string>("x", "hello");
  EXPECT_EQ(bb.get<std::string>("x").value_or(""), "hello");
  EXPECT_FALSE(bb.get<int>("x").has_value());
}

TEST(M1Blackboard, HasAndErase) {
  Blackboard bb;
  bb.set<double>("battery", 0.8);
  EXPECT_TRUE(bb.has("battery"));
  bb.erase("battery");
  EXPECT_FALSE(bb.has("battery"));
}

// ------------------------------------------------------------------------ Node

TEST(M1Node, StartsIdle) {
  auto a = scripted("A", {S});
  EXPECT_EQ(a->status(), Status::Idle);
  EXPECT_EQ(a->name(), "A");
}

TEST(M1Node, TickReturnsAndRemembersStatus) {
  auto a = scripted("A", {R, S});
  EXPECT_EQ(a->tick(), Status::Running);
  EXPECT_EQ(a->status(), Status::Running);
  EXPECT_EQ(a->tick(), Status::Success);
  EXPECT_EQ(a->status(), Status::Success);
  EXPECT_EQ(a->ticks, 2);
}

TEST(M1Node, HaltRunningNodeCallsOnHaltAndResets) {
  auto a = scripted("A", {R});
  a->tick();
  a->halt();
  EXPECT_EQ(a->halts, 1);
  EXPECT_EQ(a->status(), Status::Idle);
}

TEST(M1Node, HaltCompletedNodeDoesNotCallOnHalt) {
  auto a = scripted("A", {S});
  a->tick();
  a->halt();
  EXPECT_EQ(a->halts, 0);
  EXPECT_EQ(a->status(), Status::Idle);
}

TEST(M1Node, SetBlackboard) {
  auto a = scripted("A", {S});
  auto bb = std::make_shared<Blackboard>();
  a->setBlackboard(bb);
  EXPECT_EQ(a->blackboard(), bb);
}

TEST(M1Composite, SetBlackboardPropagatesToChildren) {
  auto a = scripted("A", {S});
  auto b = scripted("B", {S});
  auto seq = std::make_shared<Sequence>();
  seq->addChild(a).addChild(b);
  auto bb = std::make_shared<Blackboard>();
  seq->setBlackboard(bb);
  EXPECT_EQ(seq->blackboard(), bb);
  EXPECT_EQ(a->blackboard(), bb);
  EXPECT_EQ(b->blackboard(), bb);
}

// ---------------------------------------------------------------------- Leaves

TEST(M1Leaves, ConditionMapsBoolToStatus) {
  auto bb = std::make_shared<Blackboard>();
  bb->set<bool>("door_open", false);
  Condition is_open("IsDoorOpen", [](Blackboard& b) { return b.get<bool>("door_open").value_or(false); });
  is_open.setBlackboard(bb);

  EXPECT_EQ(is_open.tick(), Status::Failure);
  bb->set<bool>("door_open", true);
  EXPECT_EQ(is_open.tick(), Status::Success);
}

TEST(M1Leaves, ActionCanWriteToBlackboard) {
  auto bb = std::make_shared<Blackboard>();
  Action open("OpenDoor", [](Blackboard& b) {
    b.set<bool>("door_open", true);
    return Status::Success;
  });
  open.setBlackboard(bb);
  EXPECT_EQ(open.tick(), Status::Success);
  EXPECT_TRUE(bb->get<bool>("door_open").value_or(false));
}

TEST(M1Leaves, LeafWithoutBlackboardThrows) {
  Condition c("C", [](Blackboard&) { return true; });
  EXPECT_THROW(c.tick(), std::logic_error);
}
