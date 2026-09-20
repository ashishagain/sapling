// ROS 2 wrapper around the patrol behavior tree. This file is complete - it
// only wires topics and parameters to the blackboard and ticks the tree on a
// timer. All the decision making lives in the tree (behaviors.cpp).
#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <turtlesim/msg/pose.hpp>

#include "sapling/sapling.hpp"
#include "sapling_turtle/behaviors.hpp"

namespace sapling_turtle {

class PatrolNode : public rclcpp::Node {
 public:
  PatrolNode() : rclcpp::Node("sapling_patrol") {
    const auto waypoints_flat = declare_parameter<std::vector<double>>(
        "waypoints", {2.0, 2.0, 9.0, 2.0, 9.0, 9.0, 2.0, 9.0});
    PatrolConfig config;
    for (std::size_t i = 0; i + 1 < waypoints_flat.size(); i += 2) {
      config.waypoints.push_back({waypoints_flat[i], waypoints_flat[i + 1]});
    }
    config.charger = {declare_parameter<double>("charger_x", 5.5),
                      declare_parameter<double>("charger_y", 5.5)};
    config.low_battery_threshold = declare_parameter<double>("low_battery_threshold", 25.0);
    config.recharge_per_tick = declare_parameter<double>("recharge_per_tick", 2.0);
    drain_per_meter_ = declare_parameter<double>("battery_drain_per_meter", 1.5);
    const double tick_rate_hz = declare_parameter<double>("tick_rate_hz", 20.0);

    cmd_pub_ = create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
    pose_sub_ = create_subscription<turtlesim::msg::Pose>(
        "turtle1/pose", 10, [this](turtlesim::msg::Pose::ConstSharedPtr msg) { onPose(*msg); });

    TurtleIO io;
    io.send_velocity = [this](double linear, double angular) {
      geometry_msgs::msg::Twist cmd;
      cmd.linear.x = linear;
      cmd.angular.z = angular;
      cmd_pub_->publish(cmd);
    };
    io.log = [this](const std::string& msg) { RCLCPP_INFO(get_logger(), "%s", msg.c_str()); };

    blackboard_ = std::make_shared<sapling::Blackboard>();
    blackboard_->set<double>("battery", 100.0);

    tree_ = std::make_unique<sapling::Tree>(buildPatrolTree(config, io), blackboard_);
    RCLCPP_INFO(get_logger(), "Behavior tree:\n%s", tree_->toString().c_str());

    timer_ = create_wall_timer(std::chrono::duration<double>(1.0 / tick_rate_hz),
                               [this] { onTimer(); });
  }

  ~PatrolNode() override {
    if (tree_) tree_->halt();  // make sure the turtle stops when we shut down
  }

 private:
  void onPose(const turtlesim::msg::Pose& msg) {
    const Pose2D pose{msg.x, msg.y, msg.theta};
    // Drain the battery proportionally to the distance travelled.
    if (last_pose_) {
      const double dist = std::hypot(pose.x - last_pose_->x, pose.y - last_pose_->y);
      const double battery = blackboard_->get<double>("battery").value_or(100.0);
      blackboard_->set<double>("battery", std::max(0.0, battery - drain_per_meter_ * dist));
    }
    last_pose_ = pose;
    blackboard_->set<Pose2D>("pose", pose);
  }

  void onTimer() {
    const sapling::Status status = tree_->tickOnce();
    if (status == sapling::Status::Success) {
      RCLCPP_INFO(get_logger(), "Patrol loop complete, starting again.");
    } else if (status == sapling::Status::Failure) {
      RCLCPP_WARN(get_logger(), "Tree returned Failure, retrying next tick.");
    }
    RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 2000, "battery: %.0f%%",
                         blackboard_->get<double>("battery").value_or(-1.0));
  }

  double drain_per_meter_ = 1.5;
  std::optional<Pose2D> last_pose_;
  sapling::Blackboard::Ptr blackboard_;
  std::unique_ptr<sapling::Tree> tree_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace sapling_turtle

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<sapling_turtle::PatrolNode>());
  rclcpp::shutdown();
  return 0;
}
