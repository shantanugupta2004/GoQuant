#ifndef OKX_WEBSOCKET_H
#define OKX_WEBSOCKET_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <string>
#include <thread>

using namespace boost::asio;
using namespace boost::beast;
using boost::asio::ip::tcp;
using boost::asio::ssl::context;

class OKXWebSocket {
public:
    OKXWebSocket();
    void startFrontendListener();
    void handleIncomingMessage(const std::string& message, websocket::stream<tcp::socket>& ws);
    void connect(const std::string& uri);
    void subscribe(const std::string& channel);
    void listen();

private:
    io_context context_;
    tcp::resolver resolver_;
    context ctx_;
    websocket::stream<ssl::stream<tcp::socket>> ws_;
};

#endif // OKX_WEBSOCKET_H
