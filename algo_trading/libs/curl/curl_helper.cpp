/**
 * @file curl_helper.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "libs/curl/curl_helper.hpp"

namespace trading::curl
{
std::size_t curl_response::handler( void* i_ptr, std::size_t i_size, std::size_t i_nchars, void* i_data_ptr )
{
    if( i_data_ptr != nullptr )
    {
        reinterpret_cast<std::string*>( i_data_ptr )->append( reinterpret_cast<char*>( i_ptr ), i_size * i_nchars );
    }

    return i_size * i_nchars;
}

curl_response::curl_response( CURL* i_pcurl, bool i_response /* = false*/, bool i_header /* = false*/ )
{
    curl_easy_setopt( i_pcurl, CURLOPT_WRITEFUNCTION, &curl_response::handler );

    auto presponse = i_response ? std::addressof( body ) : nullptr;
    curl_easy_setopt( i_pcurl, CURLOPT_WRITEDATA, presponse );

    auto pheader = i_header ? std::addressof( head ) : nullptr;
    curl_easy_setopt( i_pcurl, CURLOPT_WRITEHEADER, pheader );
}
}  // namespace trading::curl
