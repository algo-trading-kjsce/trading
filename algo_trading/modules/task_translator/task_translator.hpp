/**
 * @file translator.hpp
 * @author ashwinn76
 * @brief Declaration of task translation
 * @version 0.1
 * @date 2022-07-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "libs/core/task.hpp"

#include "type_trait_utils.hpp"

#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace trading::translator
{
class task_translator
{
public:
    /**
     * @brief Construct a new task translator object
     *
     */
    task_translator() noexcept = default;

    /**
     * @brief translate messages to tasks
     *
     * @param i_msg
     */
    void translate( const std::string& i_msg );

    /**
     * @brief retrieve newly created tasks
     *
     * @return tasks
     */
    std::vector<core::task> retrieve_tasks();

private:
    std::mutex m_mtx_{};
    std::vector<core::task> m_tasks_{};

public:
    no_copy_no_move_class( task_translator );
};

}  // namespace trading::translator
