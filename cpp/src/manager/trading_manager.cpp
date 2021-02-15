/**
 * @file trading_manager.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-09
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <algorithm>

#include "fs_include.hpp"

#include "pylib/python_utils.hpp"

#include "trading_manager.hpp"

static std::mutex task_queue_mutex{};

namespace
{
using namespace trading::python;

/**
 * @brief Get the current application location
 *
 * @return current application path
 */
auto get_application_location()
{
    auto app_path{ fs::path{} };

#ifdef __linux__
    char buf[PATH_MAX]{};

    auto len{ readlink( "/proc/self/exe", buf, sizeof( buf ) - 1_sz ) };

    if( len != -1 )
    {
        buf[len] = '\0';

        app_path = buf;
    }
#else
#    error "You have to add new cases here to find app path!!"
#endif

    return app_path;
}


/**
 * @brief Start the python environment
 *
 */
void add_python_paths( std::vector<std::string> i_python_script_paths )
{
    init_python_env();

    PyRun_SimpleString( "import sys" );

    if( auto app_location{ get_application_location() }; !app_location.empty() )
    {
        if( !fs::is_directory( app_location ) )
        {
            app_location = app_location.parent_path();
        }

        auto str{ app_location.string() };

        std::replace( str.begin(), str.end(), '\\', '/' );

        auto pythonCmd{ "sys.path.insert(0, \"" + str + "\")" };

        PyRun_SimpleString( pythonCmd.c_str() );
    }

    for( auto&& path : i_python_script_paths )
    {
        std::replace( path.begin(), path.end(), '\\', '/' );

        auto pythonCmd{ "sys.path.insert(0, \"" + path + "\")" };

        PyRun_SimpleString( pythonCmd.c_str() );
    }
}

}

namespace trading
{
trading_manager::task_lock::task_lock() noexcept : m_lock{ task_queue_mutex }
{
}


trading_manager::trading_manager( std::vector<std::string> i_stocks ) : m_telegram_bot{ *this }
{
    for( auto&& stock : i_stocks )
    {
        m_price_info[stock] = m_robinhood_bot.get_historical_prices( stock );
    }

    m_telegram_thread = std::thread{ [&]() {
        while( m_keep_running )
        {
            m_telegram_bot.process_updates();
        }
    } };
}


void trading_manager::add_task( task_ptr i_task_ptr )
{
    auto lock{ task_lock{} };

    if( i_task_ptr->get_type() == trading_task_type::termination )
    {
        m_tasks.push_front( std::move( i_task_ptr ) );
    }
    else
    {
        m_tasks.push_back( std::move( i_task_ptr ) );
    }
}


void trading_manager::execute_task()
{
    auto ptr{ task_ptr{} };

    {
        auto lock{ task_lock{} };

        if( !m_tasks.empty() && m_keep_running )
        {
            ptr = std::move( m_tasks.front() );
            m_tasks.pop_front();
        }
    }

    if( ptr != nullptr )
    {
        ptr->execute();
    }
}


void trading_manager::clear_tasks()
{
    auto lock{ task_lock{} };

    while( !m_tasks.empty() )
    {
        m_tasks.pop_front();
    }
}

void trading_manager::await()
{
    while( m_keep_running )
    {
        execute_task();
    }

    m_telegram_thread.join();
}


void run_trading_manager( std::vector<std::string> i_python_script_paths, std::vector<std::string> i_stocks )
{
    add_python_paths( std::move( i_python_script_paths ) );

    auto manager{ trading_manager{ std::move( i_stocks ) } };

    manager.await();
}
}
