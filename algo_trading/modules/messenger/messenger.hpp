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

#include "fs_include.hpp"
#include "json_include.hpp"
#include "type_trait_utils.hpp"

#include <mutex>
#include <string>
#include <vector>

namespace trading::messenger
{
class telegram_bot
{
private:
    fs::path m_credentials_{};

    std::string m_url_{};

    json m_document_{};

    std::mutex m_mtx_{};

public:
    telegram_bot( fs::path i_credentials );

    ~telegram_bot();

    void push( const std::string& i_message ) const;

    std::vector<std::string> pull();

    no_copy_no_move_class( telegram_bot );
};

}  // namespace trading::messenger
