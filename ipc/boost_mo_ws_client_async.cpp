#include "./boost_mo_ws_client_async.hpp"
#include <boost/beast/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <boost/json.hpp>
#include "../utils/my_logger.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

#define DEF_PATH "/v1/stream"

std::string getTargetPath(const char* base, const char* token)
{
    std::stringstream ss;
    ss << base << "/?token=" << token;
    return ss.str();
}

namespace mo {
    // Sends a WebSocket message and prints the response
    class session : public std::enable_shared_from_this<session> {
        tcp::resolver resolver_;
        //websocket::stream<beast::tcp_stream> ws_;
        websocket::stream<
                beast::ssl_stream<beast::tcp_stream>> ws_;
        beast::flat_buffer buffer_;
        std::string host_;
        std::string text_;
        std::string target_;
        std::shared_ptr<IEventHandler> event_handler_;
    public:
        // Resolver and socket require an io_context
        explicit
        session(net::io_context &ioc, ssl::context& ctx)
                : resolver_(net::make_strand(ioc)), ws_(net::make_strand(ioc), ctx) {
        }

        void setTarget(const std::string& target) {
            target_ = target;
        }

        void setEventHandler(std::shared_ptr<IEventHandler> event_handler) {
            event_handler_ = event_handler;
        }

        // Start the asynchronous operation
        void
        run(
                char const *host,
                char const *port,
                char const *text) {
            // Save these for later
            host_ = host;
            text_ = text;

            // Look up the domain name
            resolver_.async_resolve(
                    host,
                    port,
                    beast::bind_front_handler(
                            &session::on_resolve,
                            shared_from_this()));
        }

        void
        on_resolve(
                beast::error_code ec,
                tcp::resolver::results_type results) {
            if (ec)
                return fail(ec, "resolve");

            // Set the timeout for the operation
            beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

            // Make the connection on the IP address we get from a lookup
            beast::get_lowest_layer(ws_).async_connect(
                    results,
                    beast::bind_front_handler(
                            &session::on_connect,
                            shared_from_this()));
        }

        void
        on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
            std::cout << "On connect" << std::endl;
            //ws_.binary(true); // KFC

            if (ec)
                return fail(ec, "connect");

            // Set a timeout on the operation
            beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));
            //beast::get_lowest_layer(ws_).expires_never();

            // Set SNI Hostname (many hosts need this to handshake successfully)

            if(! SSL_set_tlsext_host_name(
                    ws_.next_layer().native_handle(),
                    host_.c_str()))
            {
                ec = beast::error_code(static_cast<int>(::ERR_get_error()),
                                       net::error::get_ssl_category());
                return fail(ec, "connect");
            }

            // Perform the SSL handshake
            ws_.next_layer().async_handshake(
                    ssl::stream_base::client,
                    beast::bind_front_handler(
                            &session::on_ssl_handshake,
                            shared_from_this()));
            /*
            // Turn off the timeout on the tcp_stream, because
            // the websocket stream has its own timeout system.
            beast::get_lowest_layer(ws_).expires_never();


            // Set suggested timeout settings for the websocket
            ws_.set_option(
                    websocket::stream_base::timeout::suggested(
                            beast::role_type::client));

            // Set a decorator to change the User-Agent of the handshake
            ws_.set_option(websocket::stream_base::decorator(
                    [](websocket::request_type &req) {
                        req.set(http::field::user_agent,
                                std::string(BOOST_BEAST_VERSION_STRING) +
                                " websocket-client-async");
                    }));

            // Update the host_ string. This will provide the value of the
            // Host HTTP header during the WebSocket handshake.
            // See https://tools.ietf.org/html/rfc7230#section-5.4
            host_ += ':' + std::to_string(ep.port());
            std::cout << host_ << " " << target_ << std::endl;

            // Perform the websocket handshake
            ws_.async_handshake(host_, target_,
                                beast::bind_front_handler(
                                        &session::on_handshake,
                                        shared_from_this()));
            */
        }

        void
        on_ssl_handshake(beast::error_code ec)
        {
            if(ec)
                return fail(ec, "ssl_handshake");

            // Turn off the timeout on the tcp_stream, because
            // the websocket stream has its own timeout system.
            beast::get_lowest_layer(ws_).expires_never();

            // Set suggested timeout settings for the websocket
            ws_.set_option(
                    websocket::stream_base::timeout::suggested(
                            beast::role_type::client));
            // Set a decorator to change the User-Agent of the handshake
            ws_.set_option(websocket::stream_base::decorator(
                    [](websocket::request_type& req)
                    {
                        req.set(http::field::user_agent,
                                std::string(BOOST_BEAST_VERSION_STRING) +
                                " websocket-client-async-ssl");
                    }));
            // Perform the websocket handshake
            ws_.async_handshake(host_, target_,
                                beast::bind_front_handler(
                                        &session::on_handshake,
                                        shared_from_this()));
        }

        void
        on_handshake(beast::error_code ec) {
            std::cout << "On handshake " << text_ << std::endl;

            if (ec)
                return fail(ec, "handshake");

            // Read a message into our buffer
            ws_.async_read(
                    buffer_,
                    beast::bind_front_handler(
                            &session::on_read,
                            shared_from_this()));
        }

        void
        on_write(
                beast::error_code ec,
                std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);

            if (ec)
                return fail(ec, "write");

            // Read a message into our buffer
            //bool flag = true;
            //while (flag) {
            ws_.async_read(
                    buffer_,
                    beast::bind_front_handler(
                            &session::on_read,
                            shared_from_this()));
        }

        void
        on_read(
                beast::error_code ec,
                std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);

            if (ec)
                return fail(ec, "read");

            std::cout << beast::make_printable(buffer_.data()) << std::endl;

            const std::string data_json_str = boost::beast::buffers_to_string(buffer_.data());

            try {
                boost::json::value jsonValue = boost::json::parse(data_json_str);
                buffer_.consume(buffer_.size());

                bool flag = true;
                for (auto& value : jsonValue.at("data").as_array()) {
                    //std::cout << value << std::endl;
                    for (auto &dValue: value.as_array()) {
                        //std::cout << dValue << std::endl;
                        if (dValue.is_string()) {
                            if (dValue.as_string() == "sync") {
                                flag = false;
                            }
                        }
                    }
                }

                if (!flag) {
                    std::cout << "Sync is reached" << std::endl;
                    // Close the WebSocket connection
                    ws_.async_close(websocket::close_code::normal,
                                    beast::bind_front_handler(
                                            &session::on_close,
                                            shared_from_this()));

                    return;
                }

            } catch(std::exception& e) {
                std::cout << e.what() << std::endl;
                return;
            }

            ws_.async_read(
                    buffer_,
                    beast::bind_front_handler(
                            &session::on_read,
                            shared_from_this()));

            /*
            // Close the WebSocket connection
            ws_.async_close(websocket::close_code::normal,
                            beast::bind_front_handler(
                                    &session::on_close,
                                    shared_from_this()));
            */
        }

        void
        on_close(beast::error_code ec) {
            if (ec)
                return fail(ec, "close");

            // If we get here then the connection is closed gracefully

            // The make_printable() function helps print a ConstBufferSequence
            std::cout << beast::make_printable(buffer_.data()) << std::endl;
        }
    };
}

//------------------------------------------------------------------------------

bool boost_mo_ws_client(const char* host, const char* token, std::shared_ptr<IEventHandler> const& event_handler)
{
    const char port[] = "443";
    const char text[] = "bye";

    // The io_context is required for all I/O
    net::io_context ioc;
    const std::string target = getTargetPath(DEF_PATH, token);
    // Launch the asynchronous operation

    // The SSL context is required, and holds certificates
    //ssl::context ctx(ssl::context::sslv23_client);
    ssl::context ctx{ssl::context::tlsv12_client};

    // This holds the root certificate used for verification
    //load_root_certificates(ctx);

    // Launch the asynchronous operation
    auto session = std::make_shared<mo::session>(ioc, ctx);
    session->setTarget(target);
    session->setEventHandler(event_handler);
    session->run(host, port, text);

    // Run the I/O service. The call will return when
    // the socket is closed.
    ioc.run();

    return true;
}
