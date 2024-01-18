/**
 * @file task.hpp
 * @author ashwinn76
 * @brief Declaration of task class
 * @version 0.1
 * @date 2022-07-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "libs/core/node_type.hpp"
#include "libs/core/task_type.hpp"
#include "libs/core/ticker_info.hpp"

#include "clock.hpp"
#include "json_include.hpp"

#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace trading::core
{
/**
 * @brief tasks for nodes to talk to each other
 *
 */
class task
{
private:
    task_type m_type_{ task_type::unknown };
    node_type m_node_{ node_type::unknown };

    time_point m_timestamp_{};

    std::optional<ticker_info> m_ticker_{};

public:
    /**
     * @brief Construct a new task object
     *
     */
    task() noexcept = default;

    /**
     * @brief Construct a new task object
     *
     * @param i_type type of task
     * @param i_node node creating the task
     * @param i_timestamp timestamp of task creation
     * @param i_tickers list of ticker for this task
     */
    task( task_type i_type,
          node_type i_node,
          time_point i_timestamp,
          std::optional<ticker_info> i_ticker = std::nullopt ) noexcept;

    /**
     * @brief Get task type
     *
     * @return task_type
     */
    task_type type() const noexcept;

    /**
     * @brief Get the task source
     *
     * @return node_type
     */
    node_type node() const noexcept;

    /**
     * @brief Get task timestamp
     *
     * @return time_point
     */
    time_point timestamp() const noexcept;

    /**
     * @brief Get the tickers which the task is supposed to be related to
     *
     * @return std::vector<std::string>
     */
    const std::optional<ticker_info>& ticker() const noexcept;
};

void to_json( json& o_js, const task& i_task );
void from_json( const json& i_js, task& o_task );

bool operator==( const task& i_lhs, const task& i_rhs ) noexcept;
bool operator!=( const task& i_lhs, const task& i_rhs ) noexcept;

}  // namespace trading::core
