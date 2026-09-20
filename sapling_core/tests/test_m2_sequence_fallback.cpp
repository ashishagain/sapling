#include <gtest/gtest.h>

#include "test_helpers.hpp"

using namespace sapling;
using namespace sapling::testing;

// -------------------------------------------------------------------- Sequence

TEST(M2Sequence, AllSucceed) {
  auto a = scripted("A", {S}), b = scripted("B", {S});
  Sequence seq;
  seq.addChild(a).addChild(b);
  EXPECT_EQ(seq.tick(), Status::Success);
  EXPECT_EQ(a->ticks, 1);
  EXPECT_EQ(b->ticks, 1);
}

TEST(M2Sequence, StopsAtFirstFailure) {
  auto a = scripted("A", {S}), b = scripted("B", {F}), c = scripted("C", {S});
  Sequence seq;
  seq.addChild(a).addChild(b).addChild(c);
  EXPECT_EQ(seq.tick(), Status::Failure);
  EXPECT_EQ(c->ticks, 0) << "children after a failure must not be ticked";
}

TEST(M2Sequence, RunningChildMakesSequenceRunning) {
  auto a = scripted("A", {S}), b = scripted("B", {R, R, S});
  Sequence seq;
  seq.addChild(a).addChild(b);
  EXPECT_EQ(seq.tick(), Status::Running);
  EXPECT_EQ(seq.tick(), Status::Running);
  EXPECT_EQ(seq.tick(), Status::Success);
}

TEST(M2Sequence, ResumesFromRunningChild) {
  auto a = scripted("A", {S}), b = scripted("B", {R, S});
  Sequence seq;
  seq.addChild(a).addChild(b);
  seq.tick();
  seq.tick();
  EXPECT_EQ(a->ticks, 1) << "a Sequence (with memory) must not re-tick children that already succeeded";
  EXPECT_EQ(b->ticks, 2);
}

TEST(M2Sequence, StartsOverAfterCompleting) {
  auto a = scripted("A", {S}), b = scripted("B", {S});
  Sequence seq;
  seq.addChild(a).addChild(b);
  seq.tick();
  seq.tick();
  EXPECT_EQ(a->ticks, 2);
  EXPECT_EQ(b->ticks, 2);
}

TEST(M2Sequence, ChildrenAreIdleAfterCompleting) {
  auto a = scripted("A", {S}), b = scripted("B", {S});
  Sequence seq;
  seq.addChild(a).addChild(b);
  seq.tick();
  EXPECT_EQ(a->status(), Status::Idle);
  EXPECT_EQ(b->status(), Status::Idle);
}

TEST(M2Sequence, HaltHaltsRunningChildAndResets) {
  auto a = scripted("A", {S}), b = scripted("B", {R});
  Sequence seq;
  seq.addChild(a).addChild(b);
  seq.tick();
  seq.halt();
  EXPECT_EQ(b->halts, 1);
  EXPECT_EQ(seq.status(), Status::Idle);
  seq.tick();
  EXPECT_EQ(a->ticks, 2) << "after a halt the Sequence must start from the first child";
}

TEST(M2Sequence, EmptyIsSuccess) {
  Sequence seq;
  EXPECT_EQ(seq.tick(), Status::Success);
}

// -------------------------------------------------------------------- Fallback

TEST(M2Fallback, StopsAtFirstSuccess) {
  auto a = scripted("A", {F}), b = scripted("B", {S}), c = scripted("C", {S});
  Fallback fb;
  fb.addChild(a).addChild(b).addChild(c);
  EXPECT_EQ(fb.tick(), Status::Success);
  EXPECT_EQ(c->ticks, 0);
}

TEST(M2Fallback, AllFail) {
  auto a = scripted("A", {F}), b = scripted("B", {F});
  Fallback fb;
  fb.addChild(a).addChild(b);
  EXPECT_EQ(fb.tick(), Status::Failure);
  EXPECT_EQ(a->ticks, 1);
  EXPECT_EQ(b->ticks, 1);
}

TEST(M2Fallback, ResumesFromRunningChild) {
  auto a = scripted("A", {F}), b = scripted("B", {R, S});
  Fallback fb;
  fb.addChild(a).addChild(b);
  EXPECT_EQ(fb.tick(), Status::Running);
  EXPECT_EQ(fb.tick(), Status::Success);
  EXPECT_EQ(a->ticks, 1);
}

TEST(M2Fallback, HaltHaltsRunningChild) {
  auto a = scripted("A", {F}), b = scripted("B", {R});
  Fallback fb;
  fb.addChild(a).addChild(b);
  fb.tick();
  fb.halt();
  EXPECT_EQ(b->halts, 1);
  EXPECT_EQ(fb.status(), Status::Idle);
}

TEST(M2Fallback, EmptyIsFailure) {
  Fallback fb;
  EXPECT_EQ(fb.tick(), Status::Failure);
}
