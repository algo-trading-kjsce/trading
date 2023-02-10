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

#include "modules/messenger/messenger.hpp"

#include "libs/curl/curl_handler.hpp"
#include "libs/curl/curl_helper.hpp"

// #include "logger_macros.hpp"

#include <fmt/format.h>

#include <cstdlib>
#include <fstream>
#include <mutex>

using namespace trading;

namespace
{
/**
 * @brief Process the json response to extract message info
 *
 * @param i_json incoming json
 * @param i_user_id user id that needs to be parsed for
 * @return last update id and a list of commands
 */
auto process_updates( const json& i_json, const std::int64_t i_user_id )
{
    auto latest_update_id{ 0_i64 };

    auto cmds{ std::vector<std::string>{} };
    cmds.reserve( i_json["result"].size() );

    for( const auto& update : i_json["result"] )
    {
        if( update.contains( "update_id" ) )
        {
            latest_update_id = update["update_id"].get<std::int64_t>();

            if( update.contains( "message" ) )
            {
                const auto& message{ update["message"] };

                const auto current_user_id{ message["from"]["id"].get<std::int64_t>() };

                if( ( current_user_id == i_user_id ) && message.contains( "text" ) )
                {
                    auto msg{ message["text"].get<std::string>() };

                    // LOG_INFO( fmt::format( "Got message: '{}'", msg ) );

                    cmds.push_back( std::move( msg ) );
                }
            }
        }
    }

    return std::make_pair( latest_update_id, std::move( cmds ) );
}

}  // namespace

namespace trading::messenger
{
telegram_bot::telegram_bot( fs::path i_path ) :
    m_credentials_{ std::move( i_path ) }, m_url_{ "https://api.telegram.org/bot" }
{
    // LOG_ERROR_IF( !fs::exists( m_credentials_ ), "Messenger credentials '{}' not found!", m_credentials_.c_str() );
    if( !fs::exists( m_credentials_ ) )
    {
        m_credentials_ = fs::path{ std::getenv( "CREDENTIALSDIR" ) } / "telegram_bot.json";
    }
    if( std::ifstream stream{ m_credentials_ }; stream.good() )
    {
        // LOG_INFO( "Messenger credentials file found." );

        stream >> m_document_;

        m_url_.append( m_document_["bot_key"].get<std::string>() ).append( "/" );
    }

    // LOG_ERROR_IF( m_document_.is_null(), "Messenger credentials '{}' file empty!", m_credentials_.c_str() );

    this->push( "Bot has started!" );
}

telegram_bot::~telegram_bot()
{
    this->push( "Bot is shutting down." );

    if( std::ofstream stream{ m_credentials_ }; stream.good() )
    {
        stream << m_document_;
    }
}

void telegram_bot::push( const std::string& i_message ) const
{
    // LOG_INFO( "Sending message: '{}' ...", i_message );

    const auto complete_url{ m_url_ + "sendMessage" };

    json js;
    js["chat_id"] = m_document_["user_id"].get<std::int64_t>();
    js["text"] = i_message;

    const auto options_str{ to_string( js ) };

    const trading::curl::curl_list_ptr curl_list_ptr{ curl_slist_append( nullptr, "Content-Type: application/json" ),
                                                      curl_slist_free_all };

    const trading::curl::curl_handle_ptr curl_ptr{ curl_easy_init(), curl_easy_cleanup };

    curl_easy_setopt( curl_ptr.get(), CURLOPT_URL, complete_url.c_str() );

    curl_easy_setopt( curl_ptr.get(), CURLOPT_POSTFIELDS, options_str.c_str() );
    curl_easy_setopt( curl_ptr.get(), CURLOPT_POSTFIELDSIZE, options_str.size() );
    curl_easy_setopt( curl_ptr.get(), CURLOPT_POST, 1 );

    curl_easy_setopt( curl_ptr.get(), CURLOPT_HTTPHEADER, curl_list_ptr.get() );

    const trading::curl::curl_response response{ curl_ptr.get() };

    curl_easy_perform( curl_ptr.get() );

    // LOG_INFO( "Message sent." );
}

std::vector<std::string> telegram_bot::pull()
{
    const auto full_url{ m_url_ + "getUpdates?offset=" +
                         std::to_string( m_document_["last_update_id"].get<std::int64_t>() + 1_i64 ) };

    const auto curl_ptr{ trading::curl::curl_handle_ptr{ curl_easy_init(), curl_easy_cleanup } };

    curl_easy_setopt( curl_ptr.get(), CURLOPT_URL, full_url.c_str() );

    const auto response{ trading::curl::curl_response{ curl_ptr.get(), true } };

    curl_easy_perform( curl_ptr.get() );

    const auto js = json::parse( response.body );

    const auto [new_update_id, messages] = process_updates( js, m_document_["user_id"].get<std::int64_t>() );

    if( new_update_id != 0_i64 )
    {
        m_document_["last_update_id"] = new_update_id;
    }

    // LOG_INFO_IF( !messages.empty(), "Found {} new message(s).", messages.size() );

    return messages;
}

}  // namespace trading::messenger
