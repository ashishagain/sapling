#include <gtest/gtest.h>

#include "test_helpers.hpp"

using namespace sapling;
using namespace sapling::testing;

// ------------------------------------------------------------ ReactiveSequence

TEST(M4ReactiveSequence, ReticksEarlierChildren) {
  auto battery_ok = scripted("BatteryOK", {S});
  auto drive = scripted("Drive", {R, R, S});
  ReactiveSequence seq;
  seq.addChild(battery_ok).addChild(drive);
  EXPECT_EQ(seq.tick(), Status::Running);
  EXPECT_EQ(seq.tick(), Status::Running);
  EXPECT_EQ(seq.tick(), Status::Success);
  EXPECT_EQ(battery_ok->ticks, 3) << "a ReactiveSequence re-checks every child on every tick";
}

TEST(M4ReactiveSequence, FailingConditionInterruptsRunningAction) {
  auto battery_ok = scripted("BatteryOK", {S, F});
  auto drive = scripted("Drive", {R});
  ReactiveSequence seq;
  seq.addChild(battery_ok).addChild(drive);
  EXPECT_EQ(seq.tick(), Status::Running);
  EXPECT_EQ(seq.tick(), Status::Failure);
  EXPECT_EQ(drive->halts, 1) << "the running action must be halted when the condition fails";
  EXPECT_EQ(drive->ticks, 1);
}

TEST(M4ReactiveSequence, RunningChildHaltsLaterChildren) {
  // First tick: A succeeds, B is running. Second tick: A is running, so B must be halted.
  auto a = scripted("A", {S, R});
  auto b = scripted("B", {R});
  ReactiveSequence seq;
  seq.addChild(a).addChild(b);
  seq.tick();
  EXPECT_EQ(seq.tick(), Status::Running);
  EXPECT_EQ(b->halts, 1);
}

// ------------------------------------------------------------ ReactiveFallback

TEST(M4ReactiveFallback, SuccessOfEarlierChildInterruptsRunningAction) {
  auto goal_reached = scripted("GoalReached", {F, S});
  auto drive = scripted("Drive", {R});
  ReactiveFallback fb;
  fb.addChild(goal_reached).addChild(drive);
  EXPECT_EQ(fb.tick(), Status::Running);
  EXPECT_EQ(fb.tick(), Status::Success);
  EXPECT_EQ(drive->halts, 1);
}

TEST(M4ReactiveFallback, AllFail) {
  auto a = scripted("A", {F}), b = scripted("B", {F});
  ReactiveFallback fb;
  fb.addChild(a).addChild(b);
  EXPECT_EQ(fb.tick(), Status::Failure);
}

// -------------------------------------------------------------------- Parallel

TEST(M4Parallel, SucceedsWhenThresholdReached) {
  auto a = scripted("A", {S});
  auto b = scripted("B", {R, S});
  auto c = scripted("C", {R});
  Parallel par(2);
  par.addChild(a).addChild(b).addChild(c);

  EXPECT_EQ(par.tick(), Status::Running);
  EXPECT_EQ(par.tick(), Status::Success);
  EXPECT_EQ(a->ticks, 1) << "completed children are not re-ticked";
  EXPECT_EQ(c->halts, 1) << "children still running when the Parallel finishes must be halted";
}

TEST(M4Parallel, FailsWhenThresholdUnreachable) {
  auto a = scripted("A", {F});
  auto b = scripted("B", {R});
  Parallel par(2);
  par.addChild(a).addChild(b);
  EXPECT_EQ(par.tick(), Status::Failure);
  EXPECT_EQ(b->ticks, 1) << "all pending children are ticked before deciding";
  EXPECT_EQ(b->halts, 1);
}

TEST(M4Parallel, ResetsAfterCompleting) {
  auto a = scripted("A", {S});
  auto b = scripted("B", {S});
  Parallel par(2);
  par.addChild(a).addChild(b);
  EXPECT_EQ(par.tick(), Status::Success);
  EXPECT_EQ(par.tick(), Status::Success);
  EXPECT_EQ(a->ticks, 2);
}

TEST(M4Parallel, HaltHaltsAllRunningChildren) {
  auto a = scripted("A", {R});
  auto b = scripted("B", {R});
  Parallel par(2);
  par.addChild(a).addChild(b);
  par.tick();
  par.halt();
  EXPECT_EQ(a->halts, 1);
  EXPECT_EQ(b->halts, 1);
}
