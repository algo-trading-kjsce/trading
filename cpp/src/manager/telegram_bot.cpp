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

#include <fstream>
#include <mutex>

#include "helper/curl_helper.hpp"

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

/**
 * @brief add task to the trading manager
 *
 * @param i_cmd_str command string
 * @param i_manager trading manager
 */
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


/**
 * @brief Process the json response to extract message info
 *
 * @param i_json incoming json
 * @param i_user_id user id that needs to be parsed for
 * @return last update id and a list of commands
 */
auto process_updates( const json& i_json, std::int64_t i_user_id )
{
    auto latest_update_id{ 0_i64 };

    auto cmds{ std::vector<std::string>{} };
    cmds.reserve( i_json["result"].size() );

    for( auto&& update : i_json["result"] )
    {
        latest_update_id = update["update_id"].get<std::int64_t>();

        if( update.contains( "message" ) )
        {
            auto& message{ update["message"] };

            auto current_user_id{ message["chat"]["id"].get<std::int64_t>() };

            if( current_user_id == i_user_id )
            {
                cmds.emplace_back( message["text"].get<std::string>() );
            }
        }
    }

    return std::make_pair( latest_update_id, std::move( cmds ) );
}

[[maybe_unused]] size_t writeFunction( void* ptr, size_t size, size_t nmemb, void* data )
{
    if( data != nullptr )
    {
        reinterpret_cast<std::string*>( data )->append( reinterpret_cast<char*>( ptr ), size * nmemb );
    }

    return size * nmemb;
}
}

namespace trading
{
telegram_bot::telegram_bot( trading_manager& i_manager, fs::path i_path ) :
    m_manager{ i_manager }, m_credentials{ std::move( i_path ) }, m_url{ "https://api.telegram.org/bot" }
{
    if( auto stream{ std::ifstream{ m_credentials } }; stream.good() )
    {
        stream >> m_document;

        m_url.append( m_document["bot_key"].get<std::string>() ).append( "/" );
    }

    this->process_updates( false );
    this->send_message( "Bot has started" );
}

telegram_bot::~telegram_bot()
{
    this->send_message( "Bot is shutting down" );

    if( auto stream{ std::ofstream{ m_credentials } }; stream.good() )
    {
        stream << m_document;
    }
}

void telegram_bot::send_message( const std::string& i_message )
{
    auto lock{ telegram_lock{} };

    auto complete_url{ m_url + "sendMessage" };

    json js;
    js["chat_id"] = m_document["user_id"].get<std::int64_t>();
    js["text"] = i_message;

    auto str{ std::to_string( js ) };

    auto curl_list_ptr{ trading::curl_list_ptr{ curl_slist_append( nullptr, "Content-Type: application/json" ),
                                                curl_slist_free_all } };

    auto curl_ptr{ trading::curl_handle_ptr{ curl_easy_init(), curl_easy_cleanup } };

    curl_easy_setopt( curl_ptr.get(), CURLOPT_URL, complete_url.c_str() );

    curl_easy_setopt( curl_ptr.get(), CURLOPT_POSTFIELDS, str.c_str() );
    curl_easy_setopt( curl_ptr.get(), CURLOPT_POSTFIELDSIZE, str.size() );
    curl_easy_setopt( curl_ptr.get(), CURLOPT_POST, 1 );

    curl_easy_setopt( curl_ptr.get(), CURLOPT_HTTPHEADER, curl_list_ptr.get() );

    auto response{ trading::curl::curl_response{ curl_ptr.get() } };

    curl_easy_perform( curl_ptr.get() );
}

void telegram_bot::process_updates( bool i_create_tasks /* = true*/ )
{
    auto lock{ telegram_lock{} };

    auto full_url{ m_url +
                   "getUpdates?offset=" + std::to_string( m_document["last_update_id"].get<std::int64_t>() + 1_i64 ) };

    auto curl_ptr{ trading::curl_handle_ptr{ curl_easy_init(), curl_easy_cleanup } };

    curl_easy_setopt( curl_ptr.get(), CURLOPT_URL, full_url.c_str() );

    auto response{ trading::curl::curl_response{ curl_ptr.get(), true } };

    curl_easy_perform( curl_ptr.get() );

    auto js = std::stojson( response.m_response );

    auto [new_update_id, messages] = ::process_updates( js, m_document["user_id"].get<std::int64_t>() );

    if( new_update_id != 0_i64 )
    {
        m_document["last_update_id"] = new_update_id;
    }

    if( i_create_tasks )
    {
        for( auto&& message : messages )
        {
            add_task( message, m_manager );
        }
    }
}

telegram_bot& telegram_bot::get_bot( trading_manager& i_manager, fs::path i_path )
{
    static std::unique_ptr<telegram_bot> _telegram_bot{};

    if( _telegram_bot == nullptr )
    {
        _telegram_bot = std::unique_ptr<telegram_bot>{ new telegram_bot{ i_manager, i_path } };
    }

    return *_telegram_bot;
}
}
