cmake_minimum_required(VERSION 3.16)
project(sapling_core VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

option(SAPLING_BUILD_TESTS "Build unit tests" ON)
option(SAPLING_BUILD_EXAMPLES "Build examples" ON)
option(SAPLING_SANITIZE "Build with AddressSanitizer + UBSan" OFF)

# ----------------------------------------------------------------- library
add_library(sapling_core
  src/node.cpp
  src/leaves.cpp
  src/composite.cpp
  src/decorators.cpp
  src/tree.cpp
)
add_library(sapling::sapling_core ALIAS sapling_core)
target_include_directories(sapling_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(sapling_core PUBLIC cxx_std_17)
target_compile_options(sapling_core PRIVATE -Wall -Wextra -Wpedantic)
set_target_properties(sapling_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(SAPLING_SANITIZE)
  target_compile_options(sapling_core PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(sapling_core PUBLIC -fsanitize=address,undefined)
endif()

# ----------------------------------------------------------------- examples
if(SAPLING_BUILD_EXAMPLES)
  add_executable(enter_room_example examples/enter_room.cpp)
  target_link_libraries(enter_room_example PRIVATE sapling_core)
endif()

# ----------------------------------------------------------------- tests
if(SAPLING_BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()
  add_executable(sapling_tests
    tests/test_m1_basics.cpp
    tests/test_m2_sequence_fallback.cpp
    tests/test_m3_decorators.cpp
    tests/test_m4_reactive_parallel.cpp
    tests/test_m5_tree.cpp
  )
  target_link_libraries(sapling_tests PRIVATE sapling_core GTest::gtest_main)
  include(GoogleTest)
  gtest_discover_tests(sapling_tests)
endif()

# ----------------------------------------------------------------- install
# Lets other CMake / colcon packages (like sapling_turtle) do
#   find_package(sapling_core REQUIRED)
#   target_link_libraries(my_target sapling::sapling_core)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS sapling_core EXPORT sapling_coreTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT sapling_coreTargets
  FILE sapling_coreTargets.cmake
  NAMESPACE sapling::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sapling_core
)
configure_package_config_file(cmake/sapling_coreConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/sapling_coreConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sapling_core
)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/sapling_coreConfigVersion.cmake
  COMPATIBILITY SameMajorVersion
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/sapling_coreConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/sapling_coreConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sapling_core
)
