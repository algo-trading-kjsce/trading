/**
 * @file telegram_bot.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <string>
#include <vector>

#include "pylib/py_object.hpp"

namespace trading
{
class trading_manager;

/**
 * @brief C++ wrapper for Python Telegram Bot
 *
 */
class telegram_bot
{
private:
    trading_manager& m_manager;
    python::py_object m_bot{};  // Telegram Bot
    python::py_object m_messageFunc{};  // Function pointer to send messages
    python::py_object m_updatesFunc{};  // Function pointer to get updates from bot

public:
    /**
     * @brief Construct a new telegram bot object
     *
     * @param i_manager trading manager
     */
    telegram_bot( trading_manager& i_manager );


    /**
     * @brief sends a message to the user
     *
     * @param i_message message
     */
    void sendMessage( const std::string& i_message ) const;


    /**
     * @brief Update the trading manager with new commands or messages from the user
     *
     */
    void process_updates() const;
};
}