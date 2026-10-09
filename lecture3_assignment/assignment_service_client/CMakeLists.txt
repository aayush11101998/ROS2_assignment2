cmake_minimum_required(VERSION 3.8)
project(assignment_service_client)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

# Service interface generation
find_package(rosidl_default_generators REQUIRED)

# ============================================================
# Generate custom service
# ============================================================

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/Racetrack.srv"
)

# ============================================================
# Executables
# ============================================================

add_executable(program
  src/program.cpp
)

add_executable(controller
  src/controller.cpp
)

# ============================================================
# ROS 2 dependencies
# ============================================================

ament_target_dependencies(
  program
  rclcpp
  geometry_msgs
  sensor_msgs
  std_msgs
)

ament_target_dependencies(
  controller
  rclcpp
  geometry_msgs
  sensor_msgs
  std_msgs
)

# ============================================================
# Connect generated service interface
# ============================================================

rosidl_get_typesupport_target(
  cpp_typesupport_target
  ${PROJECT_NAME}
  "rosidl_typesupport_cpp"
)

target_link_libraries(
  program
  "${cpp_typesupport_target}"
)

target_link_libraries(
  controller
  "${cpp_typesupport_target}"
)

# ============================================================
# Install executables
# ============================================================

install(
  TARGETS
    program
    controller
  DESTINATION lib/${PROJECT_NAME}
)

# ============================================================
# Install service files
# ============================================================

install(
  DIRECTORY srv
  DESTINATION share/${PROJECT_NAME}
)

# ============================================================
# Testing / Linting
# ============================================================
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
