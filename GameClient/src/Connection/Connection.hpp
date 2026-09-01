#pragma once

#include "UdpClient.hpp"
#include "TcpClient.hpp"
#include "SWSRSlidingWindow.hpp"
#include "GameEvent.hpp"

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

    SWSRSlidingWindow<GameEventData> game_events{};
    SWSRSlidingWindow<GameSessionData> game_sessions{};
    SWSRSlidingWindow<ErrorData> error_messages{};
};