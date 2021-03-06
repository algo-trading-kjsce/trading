/**
 * @file ta_handler.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-14
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "../ta-lib/ta_func.h"

/**
 * @brief handler for initializing and safely closing TA-Library
 *
 */
class ta_handler
{
public:
    /**
     * @brief Construct a new ta_handler object
     *
     */
    explicit ta_handler() noexcept
    {
        [[maybe_unused]] auto retCode{ TA_Initialize() };
    }

    /**
     * @brief Destroy the ta_handler object
     *
     */
    ~ta_handler()
    {
        TA_Shutdown();
    }
};
