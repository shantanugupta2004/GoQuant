#include <boost/asio/ssl.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "OKXWebSocket.h"
#include <boost/json.hpp>
#include <iostream>
#include "../Simulator/TradeSimulator.h"

using namespace boost::json;
using namespace boost::asio;
using namespace boost::beast;
using tcp = ip::tcp;

OKXWebSocket::OKXWebSocket()
    : resolver_(context_), 
      ctx_(boost::asio::ssl::context::sslv23),
      ws_(context_, ctx_) 
{
    ctx_.set_default_verify_paths();
}

void OKXWebSocket::handleIncomingMessage(const std::string& message, websocket::stream<tcp::socket>& ws) {
    try {
        boost::json::value jsonVal = boost::json::parse(message);
        boost::json::object jsonObject = jsonVal.as_object();

        // Parse input parameters
        std::string spotAsset = boost::json::value_to<std::string>(jsonObject["spotAsset"]);
        std::string orderType = boost::json::value_to<std::string>(jsonObject["orderType"]);

        double quantity = 0.0;
        if (jsonObject["quantity"].kind() == boost::json::kind::int64)
            quantity = static_cast<double>(jsonObject["quantity"].as_int64());
        else
            quantity = jsonObject["quantity"].as_double();

        double volatility = 0.0;
        if (jsonObject["volatility"].kind() == boost::json::kind::int64)
            volatility = static_cast<double>(jsonObject["volatility"].as_int64());
        else
            volatility = jsonObject["volatility"].as_double();

        std::string feeTier = boost::json::value_to<std::string>(jsonObject["feeTier"]);

        std::cout << "[INFO] Parsed Quantity: " << quantity << ", Volatility: " << volatility << std::endl;

        // Run simulation
        TradeSimulator simulator(spotAsset, orderType, quantity, volatility, feeTier);
        SimulationResult result = simulator.run();

        // Prepare JSON response
        boost::json::object response;
        response["entryPrice"] = result.entryPrice;
        response["executionPrice"] = result.executionPrice;
        response["fees"] = result.fees;
        response["pnl"] = result.pnl;

        std::string responseStr = boost::json::serialize(response);

        // Send response to client
        ws.write(boost::asio::buffer(responseStr));
    }
    catch (const std::exception& ex) {
        std::cerr << "[ERROR] Message Handling Failed: " << ex.what() << std::endl;
    }
}



void OKXWebSocket::startFrontendListener() {
    try {
        io_context ioc;
        tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 8081));
        std::cout << "[INFO] WebSocket listener started on ws://localhost:8081" << std::endl;

        for (;;) {
            tcp::socket socket(ioc);
            acceptor.accept(socket);

            websocket::stream<tcp::socket> ws(std::move(socket));
            ws.accept();
            std::cout << "[INFO] Client connected to WebSocket" << std::endl;

            for (;;) {
                flat_buffer buffer;
                ws.read(buffer);

                std::string msg = buffers_to_string(buffer.data());
                std::cout << "[RECEIVED] " << msg << std::endl;

                // ✨ Handle the incoming message
                handleIncomingMessage(msg, ws);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] WebSocket Listener Error: " << e.what() << std::endl;
    }
}



void OKXWebSocket::connect(const std::string& uri) {
    try {
        // Extract hostname and port
        auto const host = "ws.gomarket-cpp.goquant.io";
        auto const port = "443";

        // Resolve the hostname
        auto const results = resolver_.resolve(host, port);

        // Establish TCP connection
        boost::asio::connect(ws_.next_layer().next_layer(), results.begin(), results.end());

        // Perform SSL handshake
        ws_.next_layer().handshake(boost::asio::ssl::stream_base::client);

        // Perform WebSocket handshake
        ws_.handshake(host, uri);

        std::cout << "[INFO] Connected to OKX WebSocket!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Connection failed: " << e.what() << std::endl;
    }
}


void OKXWebSocket::subscribe(const std::string& channel) {
    // Subscription message
    std::string msg = R"({"op": "subscribe", "args": [")" + channel + R"("]})";
    ws_.write(net::buffer(msg));
    std::cout << "[INFO] Subscribed to channel: " << channel << std::endl;
}

// void OKXWebSocket::listen() {
//     while (true) {
//         try {
//             flat_buffer buffer;
//             ws_.read(buffer);

//             // Output the message
//             std::cout << "[MESSAGE] " << boost::beast::buffers_to_string(buffer.data()) << std::endl;

//         } catch (const std::exception& e) {
//             std::cerr << "[ERROR] Listen failed: " << e.what() << std::endl;
//             break;
//         }
//     }
// }

void OKXWebSocket::listen() {
    flat_buffer buffer;

    try {
        while (true) {
            ws_.read(buffer);

            // Convert the buffer to a string
            std::string message = boost::beast::buffers_to_string(buffer.data());
            std::cout << "[MESSAGE] " << message << std::endl;

            // Clear the buffer after reading
            buffer.clear();

            // Parse the JSON
            value jsonData = parse(message);
            object obj = jsonData.as_object();

            // Extract the asks and bids
            array asks = obj["asks"].as_array();
            array bids = obj["bids"].as_array();

            // Display the top 5 asks and bids
            std::cout << "\nTop 5 Asks:\n";
            for (size_t i = 0; i < std::min(asks.size(), size_t(5)); ++i) {
                std::cout << "Price: " << asks[i].as_array()[0] 
                          << " | Amount: " << asks[i].as_array()[1] << "\n";
            }

            std::cout << "\nTop 5 Bids:\n";
            for (size_t i = 0; i < std::min(bids.size(), size_t(5)); ++i) {
                std::cout << "Price: " << bids[i].as_array()[0] 
                          << " | Amount: " << bids[i].as_array()[1] << "\n";
            }
            std::cout << "-----------------------------------\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

void teardown(boost::beast::role_type role, 
              boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& stream, 
              boost::system::error_code& ec) {
    // Send a shutdown to the SSL stream
    stream.shutdown(ec);

    // According to documentation, ignore 'short read error' as it's expected
    if (ec == boost::asio::error::eof || ec == boost::asio::ssl::error::stream_truncated) {
        ec = {};
    }
}
