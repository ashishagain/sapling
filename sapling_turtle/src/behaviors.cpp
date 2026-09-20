#include "sapling_turtle/behaviors.hpp"

#include <cmath>
#include <utility>

namespace sapling_turtle {

using sapling::Status;

// Wrap an angle to [-pi, pi]. You'll need this for the heading error.
[[maybe_unused]] static double wrapAngle(double a) { return std::atan2(std::sin(a), std::cos(a)); }

// -------------------------------------------------------------------- MoveTo

MoveTo::MoveTo(std::string name, Point2D goal, TurtleIO io, double tolerance)
    : Node(std::move(name)), goal_(goal), io_(std::move(io)), tolerance_(tolerance) {}

Status MoveTo::onTick() {
  // Milestone 6.
  // 1. Read the "pose" (Pose2D) from the blackboard.
  // 2. Compute the distance and heading error to goal_.
  // 3. Close enough? Stop and succeed. Otherwise:
  //      angular = k_ang * heading_error
  //      linear  = k_lin * distance, but ~0 while the heading error is large
  //                (turn on the spot first, then drive)
  //    Clamp both to something sane (turtlesim is happy with ~2 m/s, ~4 rad/s).
  SAPLING_TODO("MoveTo::onTick");
}

void MoveTo::onHalt() {
  // Milestone 6: stop the turtle.
  SAPLING_TODO("MoveTo::onHalt");
}

// ------------------------------------------------------------------ Recharge

Recharge::Recharge(std::string name, double per_tick, TurtleIO io)
    : Node(std::move(name)), per_tick_(per_tick), io_(std::move(io)) {}

Status Recharge::onTick() {
  // Milestone 6
  SAPLING_TODO("Recharge::onTick");
}

// ---------------------------------------------------------------- Build tree

sapling::NodePtr buildPatrolTree(const PatrolConfig& config, const TurtleIO& io) {
  // Milestone 6: build the tree described in behaviors.hpp and return its root.
  SAPLING_TODO("buildPatrolTree");
}

}  // namespace sapling_turtle
