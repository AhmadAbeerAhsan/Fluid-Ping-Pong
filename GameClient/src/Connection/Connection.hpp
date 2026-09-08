#pragma once

#include "UdpClient.hpp"
#include "TcpClient.hpp"
#include "LockFreeQueue.hpp"
#include "GameEvent.hpp"
#include "../Utilities/LockFreeQueue.hpp"

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

    LockFreeQueue<GameEventData> game_events{};
    LockFreeQueue<GameSessionData> game_sessions{};
    LockFreeQueue<ErrorData> error_messages{};
};