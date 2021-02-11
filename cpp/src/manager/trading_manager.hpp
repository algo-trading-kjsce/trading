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

#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "pylib/py_object.hpp"

namespace trading
{
/**
 * @brief High level manager for maintaining communication pipelines, and perform computations
 *
 */
class trading_manager
{
private:
    python::py_object m_telegram_bot{};  // Telegram bot to send and receive messages from user

    std::thread m_telegram_thread{};
    std::thread m_processing_thread{};

public:
    /**
     * @brief Construct a new trading manager object
     *
     */
    trading_manager();
};


/**
 * @brief The main loop for the app in case of running trades till stopped.
 *
 * @param i_python_script_paths List of paths to be considered for finding python script
 */
void run_trading_manager( std::vector<std::string> i_python_script_paths );
}
