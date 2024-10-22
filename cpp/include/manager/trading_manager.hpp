/**
 * @file trading_manager.hpp
 * @author ashwinn76
 * @brief Header for a supervising class for conducting trades
 * @version 0.1
 * @date 2021-02-09
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <atomic>
#include <deque>
#include <future>
#include <mutex>
#include <unordered_map>

#include "curl_handler.hpp"

#include "robinhood_bot.hpp"
#include "strategy_manager.hpp"
#include "tasks.hpp"
#include "telegram_bot.hpp"

namespace trading
{
/**
 * @brief High level manager for maintaining communication pipelines, and perform computations
 *
 */
class trading_manager
{
private:
    std::atomic_bool m_keep_running{ true };

    robinhood_bot m_robinhood_bot{};  // Robinhood bot to retrieve prices and execute trades
    telegram_bot& m_telegram_bot;  // Telegram bot to send and receive messages from user

    strategy_manager m_strategy_manager;

    trading::curl_handler t{};

    std::future<void> m_telegram_proc{};
    std::vector<std::future<void>> m_trading_procs{};

    std::unordered_map<std::string, stock_data> m_stocks{};
    std::unordered_map<std::string, complete_transaction_t> m_trades{};

    std::deque<task_ptr> m_tasks{};

    /**
     * @brief Clear all pending tasks and stop processing
     *
     */
    void clear_tasks();

    /**
     * @brief Execute the top task in the queue
     *
     */
    void execute_task();

    void add_trade( const std::string& i_ticker,
                    trading_strategy i_strategy,
                    candle_s i_candle,
                    double i_nshares,
                    double i_price );

    void finish_trade( const std::string& i_ticker, candle_s i_candle, double i_price );

public:
    /**
     * @brief Construct a new trading manager object
     *
     */
    trading_manager( std::vector<std::string> i_stocks );

    /**
     * @brief Add a task in the queue for the manager
     *
     * @param i_task_ptr Incoming task
     */
    void add_task( task_ptr i_task_ptr );

    /**
     * @brief Wait for pending threads
     *
     */
    void await();

    /**
     * @brief fetch stock data for ticker
     *
     * @param i_ticker incoming ticker
     * @return stock data for ticker
     */
    const stock_data& find_stock_data( const std::string& i_ticker ) const;


    /**
     * @brief fetch the last transaction data for ticker
     *
     * @param i_ticker incoming ticker
     * @return complete transaction information for ticker
     */
    std::optional<complete_transaction_t> find_transaction_data( const std::string& i_ticker ) const;

    /**
     * @brief Get the robinhood bot
     *
     * @return active robinhood_bot
     */
    robinhood_bot& get_robinhood_bot();

    friend abort_task;
    friend buy_task;
    friend sell_task;
};

/**
 * @brief The main loop for the app in case of running trades till stopped.
 *
 * @param i_stocks List of symbols to trade
 */
void run_trading_manager( std::vector<std::string> i_stocks );
}
