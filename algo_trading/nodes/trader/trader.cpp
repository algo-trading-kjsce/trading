/**
 * @file messenger.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2022-07-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libs/core/channel_type.hpp"
#include "libs/core/node_type.hpp"
#include "libs/core/task.hpp"
#include "libs/core/utils.hpp"

#include "type_trait_utils.hpp"
#include "wise_enum_include.hpp"

#include <csignal>
#include <functional>

using namespace trading;
using namespace trading::core;

int main( int argc, char** argv )
{
    rclcpp::init( argc, argv );

    std::atomic_bool kill_process{ false };

    std::signal( SIGINT, ros::sigint_handler );

    const auto translator_node{ ros::create_node_set_info( ros::node_type::trader, std::ref( kill_process ), true ) };

    const auto task_subscriber{ translator_node->create_subscription<TaskMsg>(
        std::to_string( ros::channel_type::tasks ), 10, [&]( [[maybe_unused]] const TaskMsg& i_msg ) {} ) };

    const auto task_publisher{ translator_node->create_publisher<TradeResultMsg>(
        std::to_string( ros::channel_type::results ), 10_sz ) };

    const auto subscriber{ translator_node->create_subscription<TradeRequestMsg>(
        std::to_string( ros::channel_type::requests ), 10, [&]( [[maybe_unused]] const TradeRequestMsg& i_msg ) {} ) };


    rclcpp::executors::MultiThreadedExecutor node_executor{ rclcpp::ExecutorOptions{}, 2_sz };

    node_executor.add_node( translator_node );

    rclcpp::Rate loop_rate{ 10 };

    while( rclcpp::ok() && !kill_process )
    {
        node_executor.spin_some();

        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}
