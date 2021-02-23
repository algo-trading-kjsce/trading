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
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

#include "curl_handler.hpp"

#include "robinhood_bot.hpp"
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

    trading::curl_handler t{};

    std::thread m_telegram_thread{};
    std::vector<std::thread> m_robinhood_price_threads{};

    std::unordered_map<std::string, csv_data> m_price_info{};

    std::deque<task_ptr> m_tasks{};

    class task_lock
    {
    private:
        std::lock_guard<std::mutex> m_lock;

    public:
        task_lock() noexcept;
    };

    /**
     * @brief Clear all pending tasks and stop processing
     *
     */
    void clear_tasks();

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
     * @brief Execute the top task in the queue
     *
     */
    void execute_task();

    /**
     * @brief Wait for pending threads
     *
     */
    void await();

    friend abort_task;
};

/**
 * @brief The main loop for the app in case of running trades till stopped.
 *
 * @param i_python_script_paths List of paths to be considered for finding python script
 * @param i_stocks List of symbols to trade
 */
void run_trading_manager( std::vector<std::string> i_python_script_paths, std::vector<std::string> i_stocks );
}
