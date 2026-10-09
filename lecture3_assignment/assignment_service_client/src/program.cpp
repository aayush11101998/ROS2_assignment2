#include "rclcpp/rclcpp.hpp"

#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

#include "assignment_service_client/srv/racetrack.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

using std::placeholders::_1;

using Racetrack = assignment_service_client::srv::Racetrack;

class RacetrackController : public rclcpp::Node
{
public:

  RacetrackController()
  : Node("controller")
  {
    // --------------------------------------------------------
    // Parameters
    // --------------------------------------------------------

    d_th_ = 3.0;

    speed_ = 0.0;

    max_speed_ = 10.0;

    min_speed_ = 0.0;

    ang_vel_ = 1.0;

    lin_vel_ = 0.5;

    // --------------------------------------------------------
    // Publisher
    // --------------------------------------------------------

    pub_ = this->create_publisher<geometry_msgs::msg::Twist>(
      "/cmd_vel",
      10
    );

    // --------------------------------------------------------
    // Laser subscriber
    // --------------------------------------------------------

    sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
      "/base_scan",
      50,
      std::bind(
        &RacetrackController::decidedirection,
        this,
        _1
      )
    );

    // --------------------------------------------------------
    // Service
    // --------------------------------------------------------

    service_ = this->create_service<Racetrack>(
      "/racetrack",
      std::bind(
        &RacetrackController::control,
        this,
        std::placeholders::_1,
        std::placeholders::_2
      )
    );

    RCLCPP_INFO(
      this->get_logger(),
      "Racetrack controller started."
    );

    RCLCPP_INFO(
      this->get_logger(),
      "Listening on /base_scan"
    );

    RCLCPP_INFO(
      this->get_logger(),
      "Publishing on /cmd_vel"
    );

    RCLCPP_INFO(
      this->get_logger(),
      "Service available: /racetrack"
    );
  }


private:

  // ========================================================
  // Service callback
  // ========================================================

  void control(
    const std::shared_ptr<Racetrack::Request> request,
    std::shared_ptr<Racetrack::Response> response)
  {
    if (
      request->controller == 'w' &&
      speed_ < max_speed_)
    {
      speed_ += 1.0;
    }

    else if (
      request->controller == 's' &&
      speed_ > min_speed_)
    {
      speed_ -= 1.0;
    }

    response->speed = speed_;

    RCLCPP_INFO(
      this->get_logger(),
      "Speed = %.2f",
      speed_
    );
  }


  // ========================================================
  // Distance calculation
  // ========================================================

  double distance(
    const std::vector<float> &scan)
  {
    double min_dist = 50.0;

    for (const auto &value : scan)
    {
      if (std::isfinite(value))
      {
        if (value < min_dist)
        {
          min_dist = value;
        }
      }
    }

    return min_dist;
  }


  // ========================================================
  // LaserScan callback
  // ========================================================

  void decidedirection(
    const sensor_msgs::msg::LaserScan::SharedPtr msg)
  {
    geometry_msgs::msg::Twist my_vel;

    const auto &ranges = msg->ranges;

    if (ranges.empty())
    {
      return;
    }

    // ------------------------------------------------------
    // Divide scan into 3 sectors
    // ------------------------------------------------------

    const size_t sector = ranges.size() / 3;

    if (sector == 0)
    {
      return;
    }

    std::vector<float> left(
      ranges.begin() + 2 * sector,
      ranges.begin() + 3 * sector
    );

    std::vector<float> center(
      ranges.begin() + sector,
      ranges.begin() + 2 * sector
    );

    std::vector<float> right(
      ranges.begin(),
      ranges.begin() + sector
    );

    // ------------------------------------------------------
    // Obstacle detected
    // ------------------------------------------------------

    if (distance(center) < d_th_)
    {
      if (distance(left) < distance(right))
      {
        my_vel.linear.x =
          speed_ * lin_vel_;

        my_vel.angular.z =
          -1.0 * ang_vel_ * speed_;
      }

      else
      {
        my_vel.linear.x =
          lin_vel_ * speed_;

        my_vel.angular.z =
          ang_vel_ * speed_;
      }
    }

    // ------------------------------------------------------
    // No obstacle
    // ------------------------------------------------------

    else
    {
      my_vel.linear.x =
        speed_ * lin_vel_;

      my_vel.angular.z = 0.0;
    }

    pub_->publish(my_vel);
  }


  // ========================================================
  // Variables
  // ========================================================

  double d_th_;

  double speed_;

  double max_speed_;

  double min_speed_;

  double ang_vel_;

  double lin_vel_;


  // ========================================================
  // ROS 2 interfaces
  // ========================================================

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr sub_;

  rclcpp::Service<Racetrack>::SharedPtr service_;
};


// ==========================================================
// MAIN
// ==========================================================

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto node =
    std::make_shared<RacetrackController>();

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}
