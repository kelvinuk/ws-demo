//
// Created by owner on 10/17/24.
//

#ifndef WS_DEMO_BOOST_MO_WEBSOCKET_CLIENT_ASYNC_HPP
#define WS_DEMO_BOOST_MO_WEBSOCKET_CLIENT_ASYNC_HPP
#include "../interface/ievent_handler.hpp"
#include <memory>
bool boost_mo_ws_client(const char* host, const char* token, std::shared_ptr<IEventHandler> const& event_handler);

#endif //WS_DEMO_BOOST_WEBSOCKET_CLIENT_ASYNC_HPP
