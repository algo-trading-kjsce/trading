/**
 * @file curl_handler.cpp
 * @author ashwinn76
 * @brief definition of curl_handler member functions
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "curl_handler.hpp"

namespace trading::internet
{
    curl_handler::curl_handler()
    {
        curl_global_init( CURL_GLOBAL_DEFAULT );
    }

    curl_handler::~curl_handler()
    {
        curl_global_cleanup();
    }

}
