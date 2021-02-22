/**
 * @file curl_helper.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <string>

#include "../common/curl_handler.hpp"
#include "../common/type_trait_utils.hpp"

namespace trading::curl
{
/**
 * @brief handler class for collecting the response from curl operation
 *
 */
class curl_response
{
private:
    /**
     * @brief writer function for curl
     *
     * @param i_ptr incoming new data
     * @param i_size size of character
     * @param i_nchars number of characters
     * @param i_data_ptr data to write to
     * @return amount of data written
     */
    static std::size_t handler( void* i_ptr, std::size_t i_size, std::size_t i_nchars, void* i_data_ptr );

public:
    no_copy_no_move_class( curl_response );

    std::string m_header{};
    std::string m_response{};

    /**
     * @brief Construct a new curl response object
     *
     * @param i_curl_ptr incoming curl handle
     * @param i_response whether to collect response
     * @param i_header whether to collect header
     */
    curl_response( CURL* i_pcurl, bool i_response = false, bool i_header = false );
};
}
