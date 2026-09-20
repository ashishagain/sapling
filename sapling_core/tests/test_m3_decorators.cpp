#include <gtest/gtest.h>

#include "test_helpers.hpp"

using namespace sapling;
using namespace sapling::testing;

TEST(M3Decorator, NeedsAChild) {
  EXPECT_THROW(Inverter(nullptr), std::invalid_argument);
}

TEST(M3Decorator, SetBlackboardPropagates) {
  auto a = scripted("A", {S});
  Inverter inv(a);
  auto bb = std::make_shared<Blackboard>();
  inv.setBlackboard(bb);
  EXPECT_EQ(a->blackboard(), bb);
}

TEST(M3Decorator, HaltHaltsChild) {
  auto a = scripted("A", {R});
  Inverter inv(a);
  inv.tick();
  inv.halt();
  EXPECT_EQ(a->halts, 1);
}

TEST(M3Inverter, Inverts) {
  EXPECT_EQ(Inverter(scripted("A", {S})).tick(), Status::Failure);
  EXPECT_EQ(Inverter(scripted("A", {F})).tick(), Status::Success);
  EXPECT_EQ(Inverter(scripted("A", {R})).tick(), Status::Running);
}

TEST(M3ForceSuccess, AlwaysSucceedsUnlessRunning) {
  EXPECT_EQ(ForceSuccess(scripted("A", {S})).tick(), Status::Success);
  EXPECT_EQ(ForceSuccess(scripted("A", {F})).tick(), Status::Success);
  EXPECT_EQ(ForceSuccess(scripted("A", {R})).tick(), Status::Running);
}

TEST(M3Retry, SucceedsWithinOneTick) {
  auto a = scripted("PickLock", {F, F, S});
  Retry retry(a, 3);
  EXPECT_EQ(retry.tick(), Status::Success);
  EXPECT_EQ(a->ticks, 3);
}

TEST(M3Retry, GivesUpAfterMaxAttempts) {
  auto a = scripted("PickLock", {F});
  Retry retry(a, 3);
  EXPECT_EQ(retry.tick(), Status::Failure);
  EXPECT_EQ(a->ticks, 3);
}

TEST(M3Retry, ResetsBetweenRuns) {
  auto a = scripted("PickLock", {F});
  Retry retry(a, 2);
  retry.tick();
  retry.tick();
  EXPECT_EQ(a->ticks, 4) << "each completed Retry run should get a fresh set of attempts";
}

TEST(M3Retry, RunningKeepsAttemptCount) {
  auto a = scripted("A", {F, R, F, F});
  Retry retry(a, 3);
  EXPECT_EQ(retry.tick(), Status::Running);  // attempt 1 failed, attempt 2 is running
  EXPECT_EQ(retry.tick(), Status::Failure);  // attempt 2 failed, attempt 3 failed
  EXPECT_EQ(a->ticks, 4);
}

TEST(M3Repeat, RepeatsWithinOneTick) {
  auto a = scripted("Wave", {S});
  Repeat rep(a, 3);
  EXPECT_EQ(rep.tick(), Status::Success);
  EXPECT_EQ(a->ticks, 3);
}

TEST(M3Repeat, StopsOnFailure) {
  auto a = scripted("Wave", {S, F, S});
  Repeat rep(a, 5);
  EXPECT_EQ(rep.tick(), Status::Failure);
  EXPECT_EQ(a->ticks, 2);
}

TEST(M3Repeat, RunningKeepsCount) {
  auto a = scripted("Wave", {S, R, S});
  Repeat rep(a, 3);
  EXPECT_EQ(rep.tick(), Status::Running);
  EXPECT_EQ(rep.tick(), Status::Success);
  EXPECT_EQ(a->ticks, 4);
}
