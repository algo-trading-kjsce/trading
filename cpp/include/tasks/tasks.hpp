/**
 * @file tasks.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include "common/enums.hpp"
#include "common/structs.hpp"

#include <memory>

namespace trading
{
class trading_manager;

/**
 * @brief Base class for all tasks that the trading manager can execute
 *
 */
class task_base
{
private:
    trading_manager& m_manager;  // Reference to the manager
    trading_task_type m_type;  // Type of task

public:
    /**
     * @brief Construct a new task base object
     *
     * @param i_manager Incoming trading manager
     * @param i_task_type Type of trading task
     */
    task_base( trading_manager& i_manager, trading_task_type i_task_type ) noexcept;

    ~task_base() noexcept = default;

    /**
     * @brief Get the trading manager
     *
     * @return the manager
     */
    trading_manager& get_manager() noexcept;

    /**
     * @brief Get the trading task type
     *
     * @return trading_task_type
     */
    trading_task_type get_type() const noexcept;

    /**
     * @brief executor of task
     *
     */
    virtual void execute() = 0;
};

using task_ptr = std::unique_ptr<task_base>;


/**
 * @brief This task will abort the entire process and kill the trading manager
 *
 */
class abort_task : public task_base
{
public:
    /**
     * @brief Construct a new abort task object
     *
     * @param i_manager Trading manager
     */
    abort_task( trading_manager& i_manager ) noexcept;

    void execute() override;
};


class trade_task : public task_base
{
protected:
    candle_s m_candle{};
    std::string m_ticker{};

public:
    /**
     * @brief Construct a new trade task object
     *
     * @param i_manager trading manager
     * @param i_ticker ticker to trade
     * @param i_candle last candle
     * @param i_task_type trade type
     */
    trade_task( trading_manager& i_manager,
                const std::string& i_ticker,
                const candle_s& i_candle,
                trading_task_type i_task_type ) noexcept;
};


class buy_task : public trade_task
{
private:
    trading_strategy m_strategy{};

public:
    /**
     * @brief Construct a new buy task object
     *
     * @param i_manager trading manager
     * @param i_ticker ticker to buy
     * @param i_candle last candle
     * @param i_strategy trading strategy
     */
    buy_task( trading_manager& i_manager,
              const std::string& i_ticker,
              const candle_s& i_candle,
              trading_strategy i_strategy ) noexcept;

    void execute() override;
};


class sell_task : public trade_task
{
public:
    /**
     * @brief Construct a new sell task object
     *
     * @param i_manager trading manager
     * @param i_ticker ticker to sell
     * @param i_candle last candle
     */
    sell_task( trading_manager& i_manager, const std::string& i_ticker, const candle_s& i_candle ) noexcept;

    void execute() override;
};


/**
 * @brief This is a placeholder for unknown commands
 *
 */
class unknown_cmd : public task_base
{
private:
    std::string m_cmd{};

public:
    /**
     * @brief Construct a new unknown cmd object
     *
     * @param i_manager Trading manager
     * @param i_cmd Command string
     */
    unknown_cmd( trading_manager& i_manager, std::string i_cmd ) noexcept;

    void execute() override;
};
}
