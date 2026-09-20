#pragma once
// Milestone 6: robot behaviours for turtlesim.
//
// Note that nothing in this file includes ROS. The leaves talk to the outside
// world through the small TurtleIO struct and the blackboard, so they can be
// unit-tested with a fake TurtleIO and no simulator. patrol_node.cpp is the
// only place that knows about topics and messages.
//
// Blackboard keys used:
//   "pose"    : Pose2D  - latest turtle pose, written by patrol_node
//   "battery" : double  - 0..100, drained by patrol_node while moving,
//                         refilled by the Recharge action

#include <functional>
#include <string>
#include <vector>

#include "sapling/sapling.hpp"

namespace sapling_turtle {

struct Point2D {
  double x = 0.0;
  double y = 0.0;
};

struct Pose2D {
  double x = 0.0;
  double y = 0.0;
  double theta = 0.0;  // radians
};

// Everything the leaves are allowed to do to the world.
struct TurtleIO {
  std::function<void(double linear, double angular)> send_velocity;
  std::function<void(const std::string&)> log;
};

struct PatrolConfig {
  std::vector<Point2D> waypoints;
  Point2D charger;
  double low_battery_threshold = 25.0;  // BatteryOk fails below this
  double recharge_per_tick = 2.0;       // how much Recharge adds per tick
};

// Drive to (x, y) with a simple proportional controller.
//  - No "pose" on the blackboard yet -> Running (wait for the first message).
//  - Within `tolerance` of the goal  -> stop the turtle, Success.
//  - Otherwise                       -> send a velocity command, Running.
// onHalt() MUST stop the turtle - otherwise it keeps driving after the tree
// has moved on to something else. (Try removing it and see what happens!)
class MoveTo : public sapling::Node {
 public:
  MoveTo(std::string name, Point2D goal, TurtleIO io, double tolerance = 0.15);

 protected:
  sapling::Status onTick() override;
  void onHalt() override;

 private:
  Point2D goal_;
  TurtleIO io_;
  double tolerance_;
};

// Add `per_tick` to "battery" every tick. Running until it reaches 100,
// then Success.
class Recharge : public sapling::Node {
 public:
  Recharge(std::string name, double per_tick, TurtleIO io);

 protected:
  sapling::Status onTick() override;

 private:
  double per_tick_;
  TurtleIO io_;
};

// Build the patrol tree. Suggested design:
//
//   ReactiveSequence Root
//   ├── Fallback EnsureBattery
//   │   ├── Condition BatteryOk          ("battery" > low_battery_threshold)
//   │   └── Sequence GoRecharge
//   │       ├── MoveTo  charger
//   │       └── Recharge
//   └── Sequence Patrol
//       ├── MoveTo wp0
//       ├── MoveTo wp1
//       └── ...
//
// Questions worth answering in docs/DESIGN.md once it works:
//  * Why must Root be a *Reactive*Sequence? What happens with a plain Sequence?
//  * Why is EnsureBattery a plain Fallback and not a ReactiveFallback?
//    (Hint: what would happen halfway through recharging?)
//  * After recharging, the patrol restarts at wp0. Why? How could you make it
//    resume at the waypoint it was heading to instead?
sapling::NodePtr buildPatrolTree(const PatrolConfig& config, const TurtleIO& io);

}  // namespace sapling_turtle
