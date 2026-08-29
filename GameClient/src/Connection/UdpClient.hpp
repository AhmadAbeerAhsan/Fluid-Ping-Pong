#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <functional>
#include <thread>
#include <array>

#include "ClientServerContract.hpp"

using boost::asio::ip::udp;

class UdpClient
{
public:
    UdpClient(boost::asio::io_context& io);

    void StartSend(std::shared_ptr<std::string>& message_ptr);
    void Close();
private:
    udp::socket m_socket;
    std::array<char,128> recv_buf;
    udp::endpoint reciever_endpoint;

    void StartRecieve();

    void HandleRecieve(const boost::system::error_code& ec, std::size_t len);

    void HandleSend(std::shared_ptr<std::string> message_ptr){};

public:
    const std::string connect{"Connect"};
    const std::string leave{"Leave"};
    const std::string join{"Join"};
    const char deliminator{'|'};
    const std::string listChats{"ListChats"};
    const std::string chat{"Chat"};
};