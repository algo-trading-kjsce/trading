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

#include "common/fs_include.hpp"
#include "common/type_trait_utils.hpp"

#include "core/json_helper.hpp"

#include <string>
#include <vector>

namespace trading
{
class trading_manager;

class telegram_bot
{
private:
    trading_manager& m_manager;

    fs::path m_credentials{};

    std::string m_url{};

    json m_document{};

    telegram_bot( trading_manager& i_manager, fs::path i_credentials_path );

public:
    no_copy_no_move_class( telegram_bot );

    ~telegram_bot();

    void send_message( const std::string& i_message );

    void process_updates( bool i_create_tasks = true );

    /**
     * @brief Get the telegram bot object
     *
     * @return telegram bot
     */
    static telegram_bot& get_bot( trading_manager&, fs::path );
};
}
