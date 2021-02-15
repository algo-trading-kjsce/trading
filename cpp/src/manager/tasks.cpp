/**
 * @file tasks.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <iostream>

#include "tasks.hpp"

#include "trading_manager.hpp"

namespace trading
{
task_base::task_base( trading_manager& i_manager, trading_task_type i_task_type ) noexcept :
    m_manager{ i_manager }, m_type{ i_task_type }
{
}


trading_task_type task_base::get_type() const noexcept
{
    return m_type;
}


trading_manager& task_base::get_manager() noexcept
{
    return m_manager;
}


abort_task::abort_task( trading_manager& i_manager ) noexcept : task_base{ i_manager, trading_task_type::termination }
{
}


void abort_task::execute()
{
    get_manager().m_keep_running = false;

    get_manager().clear_tasks();
}


unknown_cmd::unknown_cmd( trading_manager& i_manager, std::string i_cmd ) noexcept :
    task_base{ i_manager, trading_task_type::unknown }, m_cmd{ std::move( i_cmd ) }
{
}


void unknown_cmd::execute()
{
    std::cout << "Unknown command: " << m_cmd << '\n';
}
}
