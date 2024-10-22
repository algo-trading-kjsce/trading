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

#ifdef __linux__
#include <pwd.h>
#    include <unistd.h>
#elif WIN32
#    include <Windows.h>
#    include <libloaderapi.h>
#endif

#include <algorithm>

#include "fs_include.hpp"

#include "trading_manager.hpp"

static std::mutex task_queue_mutex{};
static std::mutex trade_queue_mutex{};

namespace
{

/**
 * @brief Get the home path
 *
 * @return home path
 */
auto get_home_path()
{
    auto path{ fs::path{} };
#ifdef __linux__
    if( auto pwd{ getpwuid( getuid() ) }; pwd != nullptr )
    {
        path = pwd->pw_dir;
    }
    else
    {
        path = getenv( "HOME" );
    }
#elif WIN32
    auto path_ptr{ static_cast<char*>( nullptr ) };
    auto sz{ 0_sz };

    if( auto err{ _dupenv_s( std::addressof( path_ptr ), std::addressof( sz ), "USERPROFILE" ) }; err == 0 )
    {
        path = fs::path{ path_ptr, path_ptr + sz };

        free( path_ptr );
    }
#else
#    error "Fix for Windows and MacOS"
#endif

    return path;
}

class task_lock
{
private:
    std::lock_guard<std::mutex> m_lock;

public:
    task_lock() : m_lock{ task_queue_mutex }
    {
    }
};


class trade_lock
{
private:
    std::lock_guard<std::mutex> m_lock;

public:
    trade_lock() : m_lock{ trade_queue_mutex }
    {
    }
};
}

namespace trading
{
trading_manager::trading_manager( std::vector<std::string> i_stocks ) :
    m_telegram_bot{ telegram_bot::get_bot( *this, get_home_path() / ".credentials" / "telegram" ) },
    m_strategy_manager{ *this }
{
    for( auto&& stock : i_stocks )
    {
        if( auto [filename, dates, stock_map]{ m_robinhood_bot.get_historical_prices( stock ) }; !stock_map.empty() )
        {
            m_stocks.emplace( std::move( stock ), std::move( stock_map.begin()->second ) );
        }
    }

    for( auto&& [ticker, stock_data] : m_stocks )
    {
        m_trading_procs.emplace_back( std::async( std::launch::async, [&]() {
            while( this->m_keep_running )
            {
                auto new_candle{ this->m_robinhood_bot.get_latest_price( ticker ) };

                if( stock_data.add_candle( std::move( new_candle ) ) )
                {
                    this->m_strategy_manager.trade( ticker, stock_data );
                }
            }
        } ) );
    }

    m_telegram_proc = std::async( std::launch::async, [&]() {
        while( this->m_keep_running )
        {
            this->m_telegram_bot.process_updates();
        }
    } );
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

    m_telegram_proc.wait();

    for( auto&& trading_proc : m_trading_procs )
    {
        trading_proc.wait();
    }
}


const stock_data& trading_manager::find_stock_data( const std::string& i_ticker ) const
{
    static auto temp{ stock_data{ 0_i32, date_s{} } };

    if( auto iter{ m_stocks.find( i_ticker ) }; iter != m_stocks.end() )
    {
        return iter->second;
    }

    return temp;
}

std::optional<complete_transaction_t> trading_manager::find_transaction_data( const std::string& i_ticker ) const
{
    auto lk{ trade_lock{} };

    auto temp{ std::optional<complete_transaction_t>{} };

    if( auto iter{ m_trades.find( i_ticker ) }; iter != m_trades.end() )
    {
        temp.emplace( iter->second );
    }

    return temp;
}


void trading_manager::add_trade( const std::string& i_ticker,
                                 trading_strategy i_strategy,
                                 candle_s i_candle,
                                 double i_nshares,
                                 double i_price )
{
    auto lk{ trade_lock{} };

    assert( m_trades.find( i_ticker ) != m_trades.end() );

    auto buy_transaction{ std::make_optional<transaction_s>( trade_type::buy, std::move( i_candle ), i_price ) };

    m_trades[i_ticker] = { i_strategy, i_nshares, std::move( buy_transaction ), std::nullopt };
}


void trading_manager::finish_trade( const std::string& i_ticker, candle_s i_candle, double i_price )
{
    auto lk{ trade_lock{} };

    m_trades[i_ticker].sell.emplace( trade_type::sell, std::move( i_candle ), i_price );
}

robinhood_bot& trading_manager::get_robinhood_bot()
{
    return m_robinhood_bot;
}


void run_trading_manager( std::vector<std::string> i_stocks )
{
    auto manager{ trading_manager{ std::move( i_stocks ) } };

    manager.await();
}
}
