/**
 * @file translator.cpp
 * @author ashwinn76
 * @brief Definition of task translation
 * @version 0.1
 * @date 2022-07-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "modules/task_translator/task_translator.hpp"

#include "libs/core/node_type.hpp"
#include "libs/core/task_type.hpp"

#include "clock.hpp"
#include "logger_macros.hpp"
#include "type_trait_utils.hpp"

#include <exception>
#include <optional>
#include <string_view>
#include <unordered_set>

using namespace trading::core;
using namespace std::literals::string_view_literals;

namespace
{
auto find_type( std::string_view i_msg )
{
    static const std::unordered_set<task_type> map{ task_type::status };

    auto idx{ i_msg.find( '@' ) };

    if( idx == std::string_view::npos )
    {
        idx = i_msg.find( ' ' );
    }

    const std::string substr{ i_msg.substr( 0_sz, idx ) };

    try
    {
        const auto task_t{ std::to_enum<task_type>( substr ) };

        if( map.find( task_t ) != map.end() )
        {
            return task_t;
        }
    }
    catch( const std::runtime_error& )
    {
    }

    LOG_WARN_SEND_MESSAGE( "Did not find '{}' in the list of known tasks.", i_msg );

    return task_type::unknown;
}

auto find_tickers( std::string_view i_msg )
{
    std::vector<std::string> tickers{};

    if( auto ticker_start_idx{ i_msg.find( ' ' ) }; ticker_start_idx != std::string_view::npos )
    {
        while( ( ticker_start_idx++ ) < i_msg.size() )
        {
            const auto next_limiter{ i_msg.find( ' ', ticker_start_idx ) };

            tickers.emplace_back( i_msg.substr( ticker_start_idx, next_limiter - ticker_start_idx ) );

            ticker_start_idx = next_limiter;
        }
    }

    return tickers;
}

// auto find_node_for_task_type( const task_type )
}  // namespace

namespace trading::translator
{
void task_translator::translate( const std::string& i_msg )
{
    std::lock_guard lk{ m_mtx_ };

    const auto task_type{ find_type( i_msg ) };
    if( task_type == task_type::unknown )
    {
        LOG_WARN( "Unknown task type." );
        return;
    }

    auto tickers{ find_tickers( i_msg ) };

    for( auto&& ticker : tickers )
    {
        m_tasks_.emplace_back( task_type, node_type::messenger, clock::now(), ticker_info{ ticker, 0.0 } );

        LOG_INFO( "New task created: {}", std::to_string( m_tasks_.back() ) );
    }
}

std::vector<core::task> task_translator::retrieve_tasks()
{
    std::lock_guard lk{ m_mtx_ };

    return std::move( m_tasks_ );
}
}  // namespace trading::translator
