#include "Connection.hpp"

Connection::Connection():
    udpC(io),
    tcpC(io)
{
}

void Connection::Run()
{
    io.run();
}
