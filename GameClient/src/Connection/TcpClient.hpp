#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

#include "ClientServerContract.hpp"
#include "TcpConnection.hpp"
using boost::asio::ip::tcp;

class TcpClient
{
private:
    boost::asio::io_context& m_io;
    tcp::resolver::results_type m_endpoints;
public:
    TcpClient(boost::asio::io_context& io);
    ~TcpClient();

    void Send(std::shared_ptr<std::string> message, bool response);
};
