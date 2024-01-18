/**
 * @file trade_context.hpp
 * @author ashwinn76
 * @brief Declaration of trade_context
 * @version 0.1
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "libs/core/candle.hpp"

#include "structs.hpp"

#include <atomic>
#include <deque>
#include <mutex>
#include <string>

namespace trading::planner
{
class trade_block
{
public:
    /**
     * @brief Construct a new trade context object
     *
     * @param i_ticker ticker for which context is being built
     */
    explicit trade_block( std::string i_ticker ) noexcept;

    /**
     * @brief add new candle
     *
     * @param i_candle incoming candle
     */
    void add_candle( core::candle i_candle );

    /**
     * @brief remove oldest candle
     *
     */
    void remove_candle();

    /**
     * @brief flag to check if we should keep processing this ticker
     *
     * @return true if ticker should keep processing
     */
    bool should_keep_processing() const noexcept;

    /**
     * @brief flag to check if new candles have been added
     *
     * @return true if new candles have been added
     */
    bool has_new_candles() const noexcept;

    /**
     * @brief Set the candles processed flag
     *
     */
    void set_candles_processed() noexcept;

    /**
     * @brief Get the price points from context
     *
     * @return price_points
     */
    price_points get_price_points() const;

    /**
     * @brief Get the ticker for the trade
     *
     * @return std::string
     */
    std::string get_ticker() const noexcept;

private:
    const std::string m_ticker_{};

    std::atomic_bool m_candles_processed_{ true };
    std::atomic_bool m_keep_processing_{ true };

    mutable std::mutex m_mtx_{};
    std::deque<core::candle> m_candles_{};
};
}  // namespace trading::planner
