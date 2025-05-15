#include "WebSocket/OKXWebSocket.h"

int main() {
    OKXWebSocket client;
    client.connect("/ws/l2-orderbook/okx/BTC-USDT-SWAP");
    client.subscribe("BTC-USDT-SWAP");
    client.listen();
    return 0;
}
