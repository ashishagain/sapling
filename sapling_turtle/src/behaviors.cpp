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
  // WHY: this is the robot's "drive to a point" ability, the building block of the whole
  // patrol. The tree uses MoveTo for every waypoint and for the trip to the charger.
  // It takes many ticks to arrive, so it returns Running until it gets there; that is
  // exactly the situation the Sequence "memory" and the ReactiveSequence
  // interruption (M2/M4) were designed for.
  // 1. Read the "pose" (Pose2D) from the blackboard.
  // 2. Compute the distance and heading error to goal_.
  // 3. Close enough? Stop and succeed. Otherwise:
  //      angular = k_ang * heading_error
  //      linear  = k_lin * distance, but ~0 while the heading error is large
  //                (turn on the spot first, then drive)
  //    Clamp both to something sane (turtlesim is happy with ~2 m/s, ~4 rad/s).
  //
  // HINTS:
  //  * Blackboard access: blackboard()->get<Pose2D>("pose") returns a std::optional.
  //    Empty = patrol_node has not received a pose yet: return Status::Running.
  //  * dx = goal_.x - pose.x; dy = goal_.y - pose.y;
  //    distance = std::hypot(dx, dy);
  //    desired heading = std::atan2(dy, dx);
  //    heading_error = wrapAngle(desired heading - pose.theta)  (helper above; without
  //    wrapping, the turtle can spin the long way round).
  //  * distance < tolerance_: io_.send_velocity(0, 0); return Success.
  //  * Otherwise pick gains (start with k_ang = 4.0, k_lin = 1.5), then:
  //      angular = std::clamp(k_ang * heading_error, -4.0, 4.0);
  //      linear  = std::clamp(k_lin * distance, 0.0, 2.0);
  //    and scale linear down to 0 while std::abs(heading_error) is large (say > 0.5
  //    rad) so it turns on the spot first. Then io_.send_velocity(linear, angular);
  //    return Running. (std::clamp needs #include <algorithm>.)
  //  * Tune the gains by watching the turtle: overshooting/spiralling means the gains
  //    are too high or the heading error is not wrapped.
  SAPLING_TODO("MoveTo::onTick");
}

void MoveTo::onHalt() {
  // Milestone 6: stop the turtle.
  // WHY: called when the tree interrupts MoveTo (e.g. battery went low). The demo would
  // visibly break without it: the turtle keeps driving toward a waypoint it has been
  // told to abandon. See the "delete this body" experiment in the roadmap.
  // HINT: one call: io_.send_velocity(0.0, 0.0). Without it the last velocity command
  // keeps being applied and the turtle drives on after the tree has moved elsewhere.
  SAPLING_TODO("MoveTo::onHalt");
}

// ------------------------------------------------------------------ Recharge

Recharge::Recharge(std::string name, double per_tick, TurtleIO io)
    : Node(std::move(name)), per_tick_(per_tick), io_(std::move(io)) {}

Status Recharge::onTick() {
  // Milestone 6
  // WHY: the "refuel" step of the demo. When the battery is low the tree drives to the
  // charger, then runs Recharge, which needs several ticks to fill the battery. While
  // it returns Running the tree stays here; when it returns Success the patrol resumes.
  // HINT:
  //  1. double battery = blackboard()->get<double>("battery").value_or(0.0);
  //  2. battery = std::min(100.0, battery + per_tick_);
  //  3. blackboard()->set<double>("battery", battery);
  //  4. battery >= 100.0 -> Success, else Running. (std::min needs <algorithm>.)
  // Optionally io_.send_velocity(0, 0) so the turtle sits still while charging, and
  // io_.log(...) to announce it (io_.log may be empty in tests, so check it first).
  // No onHalt() override is needed: recharging has nothing to clean up.
  SAPLING_TODO("Recharge::onTick");
}

// ---------------------------------------------------------------- Build tree

sapling::NodePtr buildPatrolTree(const PatrolConfig& config, const TurtleIO& io) {
  // Milestone 6: build the tree described in behaviors.hpp and return its root.
  // WHY: this is where everything from M1-M5 comes together: leaves (Condition, MoveTo,
  // Recharge), Sequence, Fallback and ReactiveSequence get assembled into the
  // patrol robot's brain. patrol_node.cpp calls this once at startup and then ticks the
  // result. It is the same job as enter_room.cpp's main(), for a real robot.
  // HINT: build bottom-up with std::make_shared, like examples/enter_room.cpp:
  //  1. battery_ok = make_shared<sapling::Condition>("BatteryOk", lambda) where the
  //     lambda takes (sapling::Blackboard& bb) and returns
  //     bb.get<double>("battery").value_or(0.0) > config.low_battery_threshold.
  //  2. go_recharge = make_shared<sapling::Sequence>("GoRecharge"), with
  //     ->addChild(MoveTo to config.charger).addChild(Recharge with
  //     config.recharge_per_tick).
  //  3. ensure_battery = make_shared<sapling::Fallback>("EnsureBattery"): children
  //     battery_ok, then go_recharge.
  //  4. patrol = make_shared<sapling::Sequence>("Patrol"): loop over
  //     config.waypoints, adding one MoveTo per waypoint (name it e.g. "wp" +
  //     std::to_string(i)). Pass `io` to every MoveTo/Recharge.
  //  5. root = make_shared<sapling::ReactiveSequence>("Root"): children
  //     ensure_battery, then patrol. Return root.
  // addChild returns the node so calls chain; the NodePtr return type accepts a
  // shared_ptr<ReactiveSequence> directly.
  SAPLING_TODO("buildPatrolTree");
}

}  // namespace sapling_turtle
