import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    config = os.path.join(get_package_share_directory("sapling_turtle"), "config", "patrol.yaml")
    return LaunchDescription([
        Node(package="turtlesim", executable="turtlesim_node", name="sim"),
        Node(
            package="sapling_turtle",
            executable="patrol_node",
            name="sapling_patrol",
            parameters=[config],
            output="screen",
        ),
    ])
