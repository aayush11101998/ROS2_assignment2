#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "assignment_service_client/srv/racetrack.hpp"

#include <cstdio>
#include <memory>

using Racetrack = assignment_service_client::srv::Racetrack;

float speed = 0.0;
float y = 0.0;

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("controller");

  auto client = node->create_client<Racetrack>("/racetrack");

  std::printf(
    "PRESS: w to increase speed (max speed 10)\n"
    "PRESS: s to decrease speed\n"
  );

  std::printf("Current speed: %f\n", speed);

  char button;

  while (rclcpp::ok())
  {
    std::scanf("%c", &button);

    if (button == 'w')
    {
      while (!client->wait_for_service(std::chrono::seconds(1)))
      {
        if (!rclcpp::ok())
        {
          rclcpp::shutdown();
          return 0;
        }

        std::printf("Waiting for /racetrack service...\n");
      }

      auto request = std::make_shared<Racetrack::Request>();
      request->controller = button;

      auto future = client->async_send_request(request);

      if (
        rclcpp::spin_until_future_complete(
          node,
          future
        ) == rclcpp::FutureReturnCode::SUCCESS)
      {
        y = future.get()->speed;

        if (y == speed && y == 10 && button == 'w')
        {
          std::printf("Speed cannot be increased further\n");
        }
        else
        {
          speed = y;
          std::printf("Speed: %f\n", speed);
        }
      }
    }

    else if (button == 's')
    {
      while (!client->wait_for_service(std::chrono::seconds(1)))
      {
        if (!rclcpp::ok())
        {
          rclcpp::shutdown();
          return 0;
        }

        std::printf("Waiting for /racetrack service...\n");
      }

      auto request = std::make_shared<Racetrack::Request>();
      request->controller = button;

      auto future = client->async_send_request(request);

      if (
        rclcpp::spin_until_future_complete(
          node,
          future
        ) == rclcpp::FutureReturnCode::SUCCESS)
      {
        y = future.get()->speed;

        if (y == 0 && y == speed && button == 's')
        {
          std::printf("Speed cannot be decreased further\n");
        }
        else
        {
          speed = y;
          std::printf("Speed: %f\n", speed);
        }
      }
    }

    else
    {
      std::printf("Please press the correct buttons\n");
    }
  }

  rclcpp::shutdown();

  return 0;
}
