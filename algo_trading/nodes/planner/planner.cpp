/**
 * @file planner.cpp
 * @author ashwinn76
 * @brief Declaration of planner node
 * @version 0.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "modules/planner/planner.hpp"
#include "modules/converter/task.hpp"
#include "modules/converter/trade_request.hpp"

#include "libs/core/channel_type.hpp"
#include "libs/core/node_type.hpp"
#include "libs/core/utils.hpp"
#include "libs/core/task.hpp"

#include "type_trait_utils.hpp"
#include "wise_enum_include.hpp"

#include "algo_trading_idl/msg/task_msg.hpp"
#include "algo_trading_idl/msg/trade_request_msg.hpp"
#include "algo_trading_idl/msg/trade_result_msg.hpp"

#include <rclcpp/rclcpp.hpp>

#include <csignal>
#include <functional>

using namespace trading;
using namespace trading::core;

using namespace algo_trading_idl::msg;

std::vector<std::string> parse_tickers( int i_argc, char** i_argv )
{
    std::vector<std::string> tickers;
    tickers.reserve( i_argc - 1 );

    for( int i = 1; i < i_argc; ++i )
    {
        tickers.push_back( i_argv[i] );
    }

    return tickers;
}

int main( int argc, char** argv )
{
    rclcpp::init( argc, argv );

    std::atomic_bool kill_process{ false };

    std::signal( SIGINT, ros::sigint_handler );

    const auto planner_node = ros::create_node_set_info( ros::node_type::planner, std::ref( kill_process ), true );

    planner::trade_planner planner{ kill_process, parse_tickers( argc, argv ) };

    const auto task_subscriber{ planner_node->create_subscription<TaskMsg>(
        std::to_string( ros::channel_type::tasks ), 10, [&]( const TaskMsg& i_msg ) {
            planner.process_task( converter::from_message( i_msg ) );
        } ) };

    const auto trade_publisher{ planner_node->create_publisher<TradeRequestMsg>(
        std::to_string( ros::channel_type::requests ), 10 ) };

    const auto trade_result_subscriber{ planner_node->create_subscription<TradeResultMsg>(
        std::to_string( ros::channel_type::results ), 10, [&]( [[maybe_unused]] const TradeResultMsg& i_msg ) {
        } ) };

    rclcpp::executors::MultiThreadedExecutor node_executor{ rclcpp::ExecutorOptions{}, 2_sz };

    node_executor.add_node( planner_node );

    rclcpp::Rate loop_rate{ 10 };

    while( rclcpp::ok() && !kill_process )
    {
        node_executor.spin_some();

        const auto trade_requests{ planner.retrieve_tasks() };

        for( auto&& trade_request : trade_requests )
        {
            trade_publisher->publish( converter::to_message( trade_request ) );
        }
    }

    rclcpp::shutdown();
    return 0;
}
