//
// Created by owner on 10/21/24.
//

#include "boost_https_login.hpp"
#include <boost/asio/ssl.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio/strand.hpp>
#include <boost/json.hpp>
#include <functional>
#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;            // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

std::string get_credential_json(const char* userName, const char* password)
{
    std::stringstream ss;
    ss << "{ \"username\": \"" << userName << "\", \"password\": \"" << password << "\" }";
    return ss.str();
}

bool boost_https_login(boost::asio::io_service& io_service, const char* host, const char* userName, const char* password, std::shared_ptr<IEventHandler> const& event_handler)
{
    try {
        const std::string login_credentials = get_credential_json(userName, password);

        net::io_context context;
        beast::tcp_stream stream(context);
        const std::string port = "443";
        const std::string path = "/v1/sessions/";

        ssl::context ctx(ssl::context::sslv23_client);
        ssl::stream<tcp::socket> ssocket = { io_service, ctx };
        tcp::resolver resolver(io_service);
        auto const it = resolver.resolve(host, port);

        connect(ssocket.lowest_layer(), it);
        ssocket.handshake(ssl::stream_base::handshake_type::client);

        {
            http::request<http::string_body> request(http::verb::post, path, 11);
            request.set(http::field::host, host);
            request.set(http::field::content_type, "application/json");
            request.set(http::field::accept_charset, "utf-8");

            request.body() = std::move(login_credentials);

            request.prepare_payload();
            //std::cout << request << std::endl;

            //beast::http::write(stream, request);
            beast::http::write(ssocket, request);
            http::response<http::dynamic_body> res;
            beast::flat_buffer buffer;
            http::read(ssocket, buffer, res);

            const std::string data_json_str = boost::beast::buffers_to_string(res.body().data());
            boost::json::value jsonValue = boost::json::parse(data_json_str);
            const std::string status = jsonValue.at("status").as_string().c_str();
            if (status != "ok") {
                std::cout << "Status is not ok: " << status << std::endl;
                return false;
            }
            std::string token = jsonValue.at("data").as_string().c_str();
            std::cout << "Token: " << token << std::endl;
            if (event_handler != nullptr) {
                event_handler->onTokenCreated(token.c_str());
            }
            return true;
        }

    } catch (boost::wrapexcept<boost::system::system_error> const &wrapexcept) {
        std::cout << "Exception:" << wrapexcept.what();
        return false;
    }

    return false;
}
