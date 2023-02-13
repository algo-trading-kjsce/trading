/**
 * @file node_info.hpp
 * @author ashwinn76
 * @brief Declaration of node_info
 * @version 0.1
 * @date 2022-08-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "libs/core/node_type.hpp"

#include <spdlog/spdlog.h>

#include <atomic>
#include <functional>
#include <memory>

namespace trading::core
{
/**
 * @brief struct to represent important information about the current node
 *
 */
struct node_info
{
    node_type type;

    std::shared_ptr<spdlog::logger> logger;

    std::reference_wrapper<std::atomic_bool> kill_flag;
};
}  // namespace trading::core
