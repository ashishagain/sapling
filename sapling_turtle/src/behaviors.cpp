#include "sapling_turtle/behaviors.hpp"

#include <cmath>
#include <utility>

namespace sapling_turtle {

// Functions with a SAPLING_TODO line inside are yours to write. Everything else in this file is
// written for you: read it if you're curious, but you don't need to change it.

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
  // TODO (M6): make the turtle drive toward goal_ and report when it has arrived. Replace the
  //     SAPLING_TODO line below with your code. The numbered plan above is the outline; this says
  //     what each piece is.
  //
  //     The turtle's pose is published onto the blackboard under the key "pose" as a Pose2D (x, y,
  //     and theta, the direction it faces, in radians). Read it with
  //     blackboard()->get<Pose2D>("pose"). That gives a std::optional: if it is empty no pose has
  //     arrived yet, so return Running and try again next tick.
  //
  //     The goal is goal_.x and goal_.y. Work out how far away it is (std::hypot of dx and dy,
  //     which are goal minus turtle) and what direction it is in (std::atan2(dy, dx) gives an
  //     angle). The heading error is that direction minus pose->theta. Pass it through wrapAngle(),
  //     the helper at the top of this file, so a turn of 350 degrees becomes -10 and the turtle
  //     takes the short way round.
  //
  //     If the distance is smaller than tolerance_ the turtle has arrived: stop it and return
  //     Success. Otherwise steer with io_.send_velocity(linear, angular), your only way to drive
  //     (linear is forward speed in m/s, angular is turning speed in rad/s). Make each one
  //     proportional to how wrong things are: angular from the heading error, linear from the
  //     distance. Gains of about 4.0 and 1.5 are a first guess. std::clamp(value, low, high), with
  //     #include <algorithm>, keeps them in range (about +-4.0 and 0..2.0).
  //
  //     Decide what to do when the turtle points far away from the goal: driving forward while
  //     turning makes a wide spiral. Then run it and tune the numbers by watching the turtle.
  //
  SAPLING_TODO("MoveTo::onTick");
}

void MoveTo::onHalt() {
  // Milestone 6: stop the turtle.
  // WHY: called when the tree interrupts MoveTo (e.g. battery went low). The demo would
  // visibly break without it: the turtle keeps driving toward a waypoint it has been
  // told to abandon. See the "delete this body" experiment in the roadmap.
  // TODO (M6): make the turtle stop. Replace the SAPLING_TODO line below with your code.
  //
  //     io_.send_velocity(linear, angular) is how you drive. Send zero for both. Without this the
  //     turtle keeps driving with the last command it was given.
  //
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
  // TODO (M6): raise the battery a little each tick. Replace the SAPLING_TODO line below with your
  //     code.
  //
  //     The battery level lives on the blackboard under the key "battery" as a double.
  //     blackboard()->get<double>("battery") gives a std::optional, and .value_or(0.0) turns
  //     'nothing there' into 0. blackboard()->set<double>("battery", x) writes it back.
  //
  //     per_tick_ is how much to add each tick. Don't let the level go above 100: std::min(a, b)
  //     (with #include <algorithm>) picks the smaller of two numbers. Return Running while charging
  //     and Success once it reaches 100.
  //
  SAPLING_TODO("Recharge::onTick");
}

// ---------------------------------------------------------------- Build tree

sapling::NodePtr buildPatrolTree(const PatrolConfig& config, const TurtleIO& io) {
  // Milestone 6: build the tree described in behaviors.hpp and return its root.
  // WHY: this is where everything from M1-M5 comes together: leaves (Condition, MoveTo,
  // Recharge), Sequence, Fallback and ReactiveSequence get assembled into the
  // patrol robot's brain. patrol_node.cpp calls this once at startup and then ticks the
  // result. It is the same job as enter_room.cpp's main(), for a real robot.
  // TODO (M6): build the tree drawn in behaviors.hpp, from the bottom up, and return its root.
  //     Replace the SAPLING_TODO line below with your code.
  //
  //     sapling_core/examples/enter_room.cpp builds a tree the same way (std::make_shared to create
  //     each node, addChild to attach children). Follow the diagram in behaviors.hpp: BatteryOk is
  //     a sapling::Condition, GoRecharge and Patrol are Sequences, EnsureBattery is a Fallback, and
  //     Root is a ReactiveSequence.
  //
  //     Sources for the values: config.low_battery_threshold, config.charger,
  //     config.recharge_per_tick and config.waypoints (one MoveTo per waypoint, so use a loop).
  //     MoveTo and Recharge constructors are in behaviors.hpp, and each takes `io`.
  //
  //     Watch out: copy low_battery_threshold into the BatteryOk lambda's capture list. Don't
  //     capture `config` itself: it is only a reference and may be gone by the time the tree runs.
  //
  SAPLING_TODO("buildPatrolTree");
}

}  // namespace sapling_turtle
