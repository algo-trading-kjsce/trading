/**
 * @file planner.hpp
 * @author ashwinn76
 * @brief Declaration of planner class
 * @version 0.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "trade_block.hpp"

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace trading
{
namespace core
{
class task;
}

namespace planner
{
class trade_planner
{
private:
    std::unordered_map<std::string, std::shared_ptr<trade_block>> m_trade_blocks_{};

    std::unordered_map<std::string, std::future<void>> m_trade_threads_{};

    mutable std::mutex m_task_mtx_{};
    std::vector<core::task> m_trade_requests_{};

    std::reference_wrapper<std::atomic_bool> m_kill_flag_;

public:
    /**
     * @brief Construct a new planner object
     *
     * @param i_kill_flag kill switch
     * @param i_tickers incoming tickers to trade
     */
    trade_planner( std::reference_wrapper<std::atomic_bool> i_kill_flag, std::vector<std::string> i_tickers );

    /**
     * @brief Get the newly created trade requests
     *
     * @return std::vector<core::trade_request>
     */
    std::vector<core::task> retrieve_tasks();

    /**
     * @brief process tasks for planner
     *
     * @param i_task incoming task
     */
    void process_task( core::task i_task );
};
}  // namespace planner
}  // namespace trading
