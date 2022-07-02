/**
 * @file base_strategy.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-23
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "enums.hpp"

namespace trading
{
class trading_manager;

class base_strategy
{
protected:
    trading_manager& m_manager;
    trading_strategy m_strategy{};

public:
    base_strategy( trading_manager& i_manager, trading_strategy i_strategy ) noexcept;

    trading_strategy strategy_type() const noexcept;

    virtual std::int32_t buy( const std::string& i_ticker ) = 0;
    virtual bool sell( const std::string& i_ticker ) = 0;
};

using strategy_ptr = std::shared_ptr<base_strategy>;


class bullish_engulfing final : public base_strategy
{
public:
    bullish_engulfing( trading_manager& i_manager ) noexcept;

    std::int32_t buy( const std::string& i_ticker ) override;
    bool sell( const std::string& i_ticker ) override;
};
}
