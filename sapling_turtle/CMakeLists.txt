cmake_minimum_required(VERSION 3.16)
project(sapling_turtle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(sapling_core REQUIRED)

# ROS-free behaviours: testable without a simulator.
add_library(sapling_turtle_behaviors STATIC src/behaviors.cpp)
target_include_directories(sapling_turtle_behaviors PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(sapling_turtle_behaviors PUBLIC sapling::sapling_core)
target_compile_options(sapling_turtle_behaviors PRIVATE -Wall -Wextra)

add_executable(patrol_node src/patrol_node.cpp)
target_link_libraries(patrol_node
  sapling_turtle_behaviors
  rclcpp::rclcpp
  ${geometry_msgs_TARGETS}
  ${turtlesim_TARGETS}
)

install(TARGETS patrol_node DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch config DESTINATION share/${PROJECT_NAME})

ament_package()
