#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
namespace json = boost::json;
using tcp = net::ip::tcp;

int main() {
    try {
        std::string host = "ws.gomarket-cpp.goquant.io";
        std::string port = "443";
        std::string target = "/ws/l2-orderbook/okx/BTC-USDT-SWAP";

        net::io_context ioc;
        ssl::context ctx(ssl::context::sslv23_client);

        tcp::resolver resolver(ioc);
        websocket::stream<beast::ssl_stream<tcp::socket>> ws(ioc, ctx);

        auto const results = resolver.resolve(host, port);
        net::connect(ws.next_layer().next_layer(), results);

        ws.next_layer().handshake(ssl::stream_base::client);
        ws.handshake(host, target);

        std::cout << "Connected to WebSocket.\n";

        while (true) {
            beast::flat_buffer buffer;
            ws.read(buffer);
            std::string msg = beast::buffers_to_string(buffer.data());
            std::cout << msg << "\n";

            auto parsed = json::parse(msg);
            auto obj = parsed.as_object();

            std::string timestamp = obj["timestamp"].as_string().c_str();
            std::cout << "Timestamp: " << timestamp << "\n";
        }

    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
