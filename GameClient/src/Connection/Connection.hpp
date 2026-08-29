#pragma once

#include "UdpClient.hpp"
#include "TcpClient.hpp"
#include <queue>

class Connection
{
private:
    //std::queue<>
public:
    Connection();
    void Run();

    boost::asio::io_context io;
    UdpClient udpC;
    TcpClient tcpC;
};