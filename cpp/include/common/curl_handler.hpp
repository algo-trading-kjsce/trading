/**
 * @file curl_handler.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <memory>

#include <curl/curl.h>

namespace trading
{
/**
 * @brief Handler to ensure proper initialization and cleanup of cURL
 *
 */
class curl_handler
{
public:
    curl_handler()
    {
        curl_global_init( CURL_GLOBAL_DEFAULT );
    }

    ~curl_handler()
    {
        curl_global_cleanup();
    }
};

using curl_handle_ptr = std::unique_ptr<CURL, decltype( &curl_easy_cleanup )>;
using curl_list_ptr = std::unique_ptr<curl_slist, decltype( &curl_slist_free_all )>;
}