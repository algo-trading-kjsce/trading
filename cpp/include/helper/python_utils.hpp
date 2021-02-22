/**
 * @file python_utils.hpp
 * @author ashwinn76
 * @brief Utility functions for Python C API
 * @version 0.1
 * @date 2021-02-07
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <memory>

#include "../common/fs_include.hpp"
#include "../common/type_trait_utils.hpp"

#include "py_object.hpp"

namespace trading::python
{
class python_env_handler
{
private:
    /**
     * @brief Construct a new python env handler object
     *
     */
    python_env_handler();

public:
    python_env_handler( const python_env_handler& ) = delete;
    python_env_handler( python_env_handler&& ) = delete;

    /**
     * @brief Destroy the python env handler object
     *
     */
    ~python_env_handler();

    friend std::shared_ptr<python_env_handler> init_python_env();
};

/**
 * @brief Initialize the Python environment for running scripts
 *
 * @return Shared instance of python instance handler
 */
std::shared_ptr<python_env_handler> init_python_env();


/**
 * @brief Get the telegram bot object
 *
 * @return Native wrapper around Python Telegram object
 */
py_object get_telegram_bot();

}
