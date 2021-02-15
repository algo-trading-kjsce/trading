/**
 * @file telegram_bot.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <mutex>

#include "tasks.hpp"
#include "telegram_bot.hpp"
#include "trading_manager.hpp"

static std::mutex telegram_mtx{};

namespace
{
class telegram_lock
{
private:
    std::lock_guard<std::mutex> m_lock;

public:
    telegram_lock() : m_lock{ telegram_mtx }
    {
    }
};

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

    assert( false );
    return py_object{};
}


void add_task( std::string i_cmd_str, trading::trading_manager& i_manager )
{
    if( i_cmd_str == "stop" )
    {
        i_manager.add_task( std::make_unique<trading::abort_task>( i_manager ) );
    }
    else
    {
        i_manager.add_task( std::make_unique<trading::unknown_cmd>( i_manager, std::move( i_cmd_str ) ) );
    }
}
}

namespace trading
{
telegram_bot::telegram_bot( trading_manager& i_manager ) : m_manager{ i_manager }
{
    m_bot = create_telegram_bot();
    m_messageFunc = py_object{ PyObject_GetAttrString( m_bot, "sendMessage" ) };
    m_updatesFunc = py_object{ PyObject_GetAttrString( m_bot, "checkForUpdates" ) };
}


void telegram_bot::sendMessage( const std::string& i_message ) const
{
    auto lock{ telegram_lock{} };

    auto str{ py_object{ std::move( i_message ) } };

    auto args_tuple{ py_object{ PyTuple_Pack( 1, str.get_pointer() ) } };

    auto py_obj{ py_object{ PyObject_Call( m_messageFunc, args_tuple, nullptr ) } };
}


void telegram_bot::process_updates() const
{
    auto lock{ telegram_lock{} };

    auto messages{ std::vector<std::string>{} };

    if( auto py_obj{ py_object{ _PyObject_CallNoArg( m_updatesFunc ) } }; py_obj != nullptr && PyList_Check( py_obj ) )
    {
        if( auto sz{ PyList_GET_SIZE( py_obj.get_pointer() ) }; sz > 0 )
        {
            for( auto idx{ 0 }; idx < sz; ++idx )
            {
                auto item_ptr{ py_object{ PyList_GetItem( py_obj, idx ), true } };

                add_task( item_ptr.string(), m_manager );
            }
        }
    }
}
}
