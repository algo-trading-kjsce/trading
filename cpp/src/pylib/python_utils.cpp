/**
 * @file python_utils.cpp
 * @author ashwinn76
 * @brief Source for python C API utility functions
 * @version 0.1
 * @date 2021-02-07
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "python_utils.hpp"

namespace trading::python
{
python_env_handler::python_env_handler()
{
    Py_Initialize();
}

python_env_handler::~python_env_handler()
{
    Py_Finalize();
}

std::shared_ptr<python_env_handler> init_python_env()
{
    static auto s_python_env_handler{ std::shared_ptr<python_env_handler>{} };

    if( s_python_env_handler == nullptr )
    {
        s_python_env_handler = std::shared_ptr<python_env_handler>{ new python_env_handler{} };
    }

    return s_python_env_handler;
}

}
