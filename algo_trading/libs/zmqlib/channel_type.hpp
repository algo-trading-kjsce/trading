/**
 * @file channel_type.hpp
 * @author ashwinn76
 * @brief Declaration of channel_type enum
 * @version 0.1
 * @date 2022-07-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "wise_enum_include.hpp"

namespace trading::zeromq
{

WISE_ENUM_CLASS( channel_type, incoming_messages, outgoing_messages, logs, tasks, candles, requests, results )

}  // namespace trading::zmq
