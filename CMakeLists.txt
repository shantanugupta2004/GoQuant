cmake_minimum_required(VERSION 3.10)
project(GoQuant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(WebSocket)

# Add source files
add_executable(GoQuant main.cpp WebSocket/OKXWebSocket.cpp)

#Link necessary libraries
target_link_libraries(GoQuant
    /usr/lib/x86_64-linux-gnu/libssl.so
    /usr/lib/x86_64-linux-gnu/libcrypto.so
    /lib/x86_64-linux-gnu/libboost_json.so
    boost_system
    boost_thread
)


