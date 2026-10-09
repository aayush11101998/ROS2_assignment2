Clone the assignment repository into your ROS 2 workspace:

\textbf{git clone https://github.com/aayush11101998/ROS2\_assignment2.git}

Next, navigate to your ROS 2 workspace and clone the Stage simulator and its ROS 2 interface:

\textbf{git clone https://github.com/tuw-robotics/Stage.git}

\textbf{git clone https://github.com/tuw-robotics/stage\_ros2.git}

Ensure that the required FLTK development packages are installed. If not, install them using:

\textbf{sudo apt update \&\& sudo apt install -y libfltk1.3-dev}

Also, ensure that the required ROS 2 message packages are installed:

\textbf{sudo apt install -y ros-humble-ackermann-msgs}

If you encounter errors related to \texttt{marker\_msgs}, check whether the package is available for your ROS 2 distribution. If it is not available through APT, obtain and build the appropriate source package.

Finally, build the Stage simulator and its ROS 2 interface from your workspace.
