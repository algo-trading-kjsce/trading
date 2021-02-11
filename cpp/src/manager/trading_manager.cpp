/**
 * @file trading_manager.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-09
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <algorithm>

#include "fs_include.hpp"

#include "pylib/python_utils.hpp"

#include "trading_manager.hpp"

namespace
{
using namespace trading::python;

/**
 * @brief Create a telegram bot object
 *
 * @return RAII wrapped telegram bot
 */
auto create_telegram_bot()
{
    if( auto modulePtr{ py_object{ PyImport_ImportModule( "telegrambot" ) } }; modulePtr != nullptr )
    {
        if( auto dictPtr{ py_object{ PyModule_GetDict( modulePtr ), true } }; dictPtr != nullptr )
        {
            if( auto funcPtr{ py_object{ PyDict_GetItemString( dictPtr, "telegram_bot" ), true } }; funcPtr != nullptr )
            {
                if( PyCallable_Check( funcPtr ) )
                {
                    if( auto botPtr{ py_object{ _PyObject_CallNoArg( funcPtr ) } }; botPtr != nullptr )
                    {
                        return botPtr;
                    }
                }
            }
        }
    }

    return py_object{};
}


/**
 * @brief Get the current application location
 *
 * @return current application path
 */
auto get_application_location()
{
    auto app_path{ fs::path{} };

#ifdef __linux__
    char buf[PATH_MAX]{};

    auto len{ readlink( "/proc/self/exe", buf, sizeof( buf ) - 1_sz ) };

    if( len != -1 )
    {
        buf[len] = '\0';

        app_path = buf;
    }
#else
#    error "You have to add new cases here to find app path!!"
#endif

    return app_path;
}


/**
 * @brief Start the python environment
 *
 */
void add_python_paths( std::vector<std::string> i_python_script_paths )
{
    init_python_env();

    PyRun_SimpleString( "import sys" );

    if( auto app_location{ get_application_location() }; !app_location.empty() )
    {
        if( !fs::is_directory( app_location ) )
        {
            app_location = app_location.parent_path();
        }

        auto str{ app_location.string() };

        std::replace( str.begin(), str.end(), '\\', '/' );

        auto pythonCmd{ "sys.path.insert(0, \"" + str + "\")" };

        PyRun_SimpleString( pythonCmd.c_str() );
    }

    for( auto&& path : i_python_script_paths )
    {
        std::replace( path.begin(), path.end(), '\\', '/' );

        auto pythonCmd{ "sys.path.insert(0, \"" + path + "\")" };

        PyRun_SimpleString( pythonCmd.c_str() );
    }
}

}

namespace trading
{
trading_manager::trading_manager()
{
    m_telegram_bot = create_telegram_bot();
}


void run_trading_manager( [[maybe_unused]] std::vector<std::string> i_python_script_paths )
{
    add_python_paths( std::move( i_python_script_paths ) );

    auto manager{ trading_manager{} };
}
}
