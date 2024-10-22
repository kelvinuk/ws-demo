
#include <iostream>

#include <boost/asio.hpp>
#include "ipc/boost_https_login.hpp"
#include "ipc/event_handler.hpp"
#include "ipc/boost_mo_ws_client_async.hpp"

namespace net = boost::asio;            // from <boost/asio.hpp>

int main() {
    net::io_service io_service;
    std::string username = "devinterview"; // "john";
    std::string password = "OwAb6wrocirEv"; // "j0hnpass";

    std::shared_ptr<IEventHandler> event_handler = std::make_shared<EventHandler>();
    if (boost_https_login(io_service, "api.mollybet.com", username.c_str(), password.c_str(), event_handler) == false) {
        return -1;
    }

    std::string token = event_handler->getCreatedToken();

    boost_mo_ws_client("api.mollybet.com", token.c_str(), event_handler);

    return 0;
}
