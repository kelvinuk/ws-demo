//
// Created by owner on 10/21/24.
//

#ifndef WS_DEMO_BOOST_HTTPS_LOGIN_HPP
#define WS_DEMO_BOOST_HTTPS_LOGIN_HPP

#include <boost/asio.hpp>
#include <memory>
#include "../interface/ievent_handler.hpp"
bool boost_https_login(boost::asio::io_service& io_service, const char* host, const char* userName, const char* password, std::shared_ptr<IEventHandler> const& event_handler);

#endif //WS_DEMO_BOOST_HTTPS_LOGIN_HPP
