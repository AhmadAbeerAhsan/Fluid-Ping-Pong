#include "Connection.hpp"

Connection::Connection():
    udpC(io, game_events),
    tcpC(io, game_sessions, error_messages)
{
}

void Connection::Run()
{
    io.run();
}
