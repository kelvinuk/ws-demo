//
// Created by owner on 10/21/24.
//

#ifndef WS_DEMO_MY_LOGGER_HPP
#define WS_DEMO_MY_LOGGER_HPP

#include <boost/beast.hpp>

void
fail(boost::beast::error_code ec, char const* what);

#endif //WS_DEMO_MY_LOGGER_HPP
