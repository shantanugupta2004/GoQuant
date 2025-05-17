#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "OKXWebSocket.h"
#include <boost/json.hpp>
#include <iostream>
#include<sstream>
#include "../Simulator/TradeSimulator.h"

using namespace boost::json;
using namespace boost::asio;
using namespace boost::beast;
using tcp = ip::tcp;

OKXWebSocket::OKXWebSocket()
    : resolver_(context_), 
      ws_(context_) 
{
}

void OKXWebSocket::handleIncomingMessage(const std::string& message, websocket::stream<tcp::socket>& ws)
{
    try {
        std::cout << "[DEBUG] Incoming Message: " << message << std::endl;

        boost::json::value jsonVal = boost::json::parse(message);
        boost::json::object jsonObject = jsonVal.as_object();

        // Parse input parameters with type checks
        std::string spotAsset = jsonObject["spotAsset"].as_string().c_str();
        std::string orderType = jsonObject["orderType"].as_string().c_str();

        // Handle Quantity
        double quantity = 0.0;
        if (jsonObject["quantity"].kind() == kind::double_) {
            quantity = jsonObject["quantity"].as_double();
        } else if (jsonObject["quantity"].kind() == kind::int64) {
            quantity = static_cast<double>(jsonObject["quantity"].as_int64());
        } else if (jsonObject["quantity"].kind() == kind::string) {
            std::stringstream ss(jsonObject["quantity"].as_string().c_str());
            if (!(ss >> quantity)) {
                throw std::runtime_error("Failed to parse quantity from string");
            }
        } else {
            throw std::runtime_error("Quantity is not a valid double, int, or parseable string");
        }

        // Handle Volatility
        double volatility = 0.0;
        if (jsonObject["volatility"].kind() == kind::double_) {
            volatility = jsonObject["volatility"].as_double();
        } else if (jsonObject["volatility"].kind() == kind::int64) {
            volatility = static_cast<double>(jsonObject["volatility"].as_int64());
        } else if (jsonObject["volatility"].kind() == kind::string) {
            std::stringstream ss(jsonObject["volatility"].as_string().c_str());
            if (!(ss >> volatility)) {
                throw std::runtime_error("Failed to parse volatility from string");
            }
        } else {
            throw std::runtime_error("Volatility is not a valid double, int, or parseable string");
        }

        std::string feeTier = jsonObject["feeTier"].as_string().c_str();

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
        response["slippage"] = result.slippage;
        response["tradeCost"] = result.tradeCost;
        response["marketImpact"] = result.marketImpact;
        response["makerTaker"] = result.makerTaker;
        response["latency"] = result.latency;

        std::string responseStr = boost::json::serialize(response);

        // Send response to client
        ws.write(boost::asio::buffer(responseStr));
        std::cout << "[SENT] " << responseStr << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "[ERROR] Message Handling Failed: " << ex.what() << std::endl;
    }
}

void OKXWebSocket::startFrontendListener() {
    try {
        tcp::acceptor acceptor(context_, tcp::endpoint(tcp::v4(), 8081));
        std::cout << "[INFO] WebSocket listener started on ws://localhost:8081\n";

        for (;;) {
            tcp::socket socket(context_);
            acceptor.accept(socket);
            std::cout << "[INFO] Client connected to WebSocket\n";

            websocket::stream<tcp::socket> ws(std::move(socket));

            // Accept websocket handshake
            ws.accept();

            for (;;) {
                boost::beast::flat_buffer buffer;
                ws.read(buffer);
                std::string msg = boost::beast::buffers_to_string(buffer.data());

                handleIncomingMessage(msg, ws);
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "[ERROR] WebSocket Listener Error: " << ex.what() << std::endl;
    }
}

void OKXWebSocket::connect(const std::string& uri) {
    try {
        auto const host = "ws.gomarket-cpp.goquant.io";
        auto const port = "80";

        auto const results = resolver_.resolve(host, port);
        boost::asio::connect(ws_.next_layer(), results.begin(), results.end());

        ws_.handshake(host, uri);
        std::cout << "[INFO] Connected to OKX WebSocket!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Connection failed: " << e.what() << std::endl;
    }
}

void OKXWebSocket::subscribe(const std::string& channel) {
    std::string msg = R"({"op": "subscribe", "args": [")" + channel + R"("]})";
    ws_.write(net::buffer(msg));
    std::cout << "[INFO] Subscribed to channel: " << channel << std::endl;
}

void OKXWebSocket::listen() {
    flat_buffer buffer;

    try {
        while (true) {
            ws_.read(buffer);

            std::string message = boost::beast::buffers_to_string(buffer.data());
            std::cout << "[MESSAGE] " << message << std::endl;

            buffer.clear();
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}
