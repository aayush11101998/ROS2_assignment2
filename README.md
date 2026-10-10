Clone this repository in your ROS 2 workspace:
**https://github.com/aayush11101998/ROS2_assignment2.git**
Next, clone the Stage simulator and its ROS 2 interface into your workspace:
**git clone https://github.com/tuw-robotics/Stage.git**
**git clone https://github.com/tuw-robotics/stage_ros2.git**
Ensure that the FLTK development packages are installed. If not, install them using:
**sudo apt install -y libfltk1.3-dev**
Also, ensure that `ackermann_msgs` is installed:
**sudo apt install -y ros-humble-ackermann-msgs**
If you encounter errors related to `marker_msgs`, check whether the package is available for your ROS 2 distribution and install it if available. 
To run the world `ros2 run stage_ros2 stage_ros2 --ros-args   -p world_file:=/"full address to your ros_ws"/assignment_service_client/world/my_world.world`
Use `sensor_msgs/msg/laser_scan` topic to subscribe to the value of sensor.

## Racetrack

![Racetrack](https://raw.githubusercontent.com/aayush11101998/ROS2_assignment2/master/lecture3_assignment/assignment_service_client/world/tracciato.png)
