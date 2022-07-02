/**
 * @file robinhood_bot.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "common/fs_include.hpp"

#include "stock_exchange.hpp"

#include <mutex>

static std::mutex robinhood_request_mtx{};

namespace
{
class rh_lock
{
private:
    std::lock_guard<std::mutex> m_lock;

public:
    rh_lock() : m_lock{ robinhood_request_mtx }
    {
    }
};
}


namespace trading
{
robinhood_bot::robinhood_bot()
{
}

csv_data robinhood_bot::get_historical_prices( [[maybe_unused]] const std::string& i_symbol )
{
    return {};
}

candle_s robinhood_bot::get_latest_price( [[maybe_unused]] const std::string& i_symbol )
{
    auto lock{ rh_lock{} };

    auto candle{ candle_s{} };

    return candle;
}


std::pair<double, double> robinhood_bot::buy( [[maybe_unused]] const std::string& i_ticker )
{
    return {};
}

double robinhood_bot::sell( [[maybe_unused]] const std::string& i_ticker )
{
    return {};
}
}
