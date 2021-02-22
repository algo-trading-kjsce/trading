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

#include <mutex>

#include "fs_include.hpp"

#include "processor/utilities.hpp"

#include "robinhood_bot.hpp"

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

using namespace trading::python;

auto create_robinhood_bot()
{
    if( auto modulePtr{ py_object{ PyImport_ImportModule( "rh" ) } }; modulePtr != nullptr )
    {
        if( auto dictPtr{ py_object{ PyModule_GetDict( modulePtr ), true } }; dictPtr != nullptr )
        {
            if( auto funcPtr{ py_object{ PyDict_GetItemString( dictPtr, "rh_login" ), true } }; funcPtr != nullptr )
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
}


namespace trading
{
robinhood_bot::robinhood_bot()
{
    m_bot = create_robinhood_bot();

    m_historyFunc = py_object{ PyObject_GetAttrString( m_bot, "get_historical_data" ) };

    m_last_price_func = py_object{ PyObject_GetAttrString( m_bot, "get_last_price" ) };
}

csv_data robinhood_bot::get_historical_prices( std::string i_symbol )
{
    auto lock{ rh_lock{} };

    auto str{ py_object{ std::move( i_symbol ) } };

    auto args_tuple{ py_object{ PyTuple_Pack( 1, str.get_pointer() ) } };

    auto py_obj{ py_object{ PyObject_Call( m_historyFunc, args_tuple, nullptr ) } };

    auto file_path{ fs::path{ py_obj.string() } };

    return trading::utilities::read_initial_csv( file_path );
}

candle_s robinhood_bot::get_latest_price( std::string i_symbol )
{
    auto lock{ rh_lock{} };

    auto str{ py_object{ std::move( i_symbol ) } };

    auto args_tuple{ py_object{ PyTuple_Pack( 1, str.get_pointer() ) } };

    auto py_obj{ py_object{ PyObject_Call( m_last_price_func, args_tuple, nullptr ) } };

    auto candle{ candle_s{} };

    if( auto s{ py_obj.string() }; !s.empty() )
    {
        if( auto o_candle{ candle_s::try_create_candle( s ) }; o_candle.has_value() )
        {
            candle = std::move( o_candle.value() );
        }
    }

    return candle;
}
}
