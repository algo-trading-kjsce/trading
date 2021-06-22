/**
 * @file strategy_manager.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-23
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "enums.hpp"
#include "stock_data.hpp"

#include "strategy.hpp"

namespace trading
{
class trading_manager;

class strategy_manager
{
private:
    trading_manager& m_manager;

    std::array<strategy_ptr, static_cast<std::size_t>( trading_strategy::__LAST )> m_ptrs{};

    std::unordered_map<trading_strategy, std::unordered_set<std::string>> m_active_strategies{};

public:
    strategy_manager( trading_manager& i_manager );

    std::optional<trading_strategy> is_active( const std::string& i_ticker ) const;

    void trade( const std::string& i_ticker, const stock_data& i_stock_data );

    strategy_ptr get_strategy( trading_strategy i_strategy );
};
}
