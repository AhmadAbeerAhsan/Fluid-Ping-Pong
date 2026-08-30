#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <functional>
#include <thread>
#include <array>

#include "ClientServerContract.hpp"
#include "SWSRSlidingWindow.hpp"

using boost::asio::ip::udp;

class UdpClient
{
public:
    UdpClient(
        boost::asio::io_context& io,
        SWSRSlidingWindow<GameEventData>& game_events,
        SWSRSlidingWindow<ErrorData>& error_messages
    );

    void StartSend(std::shared_ptr<std::string> message_ptr);
    void Close();
private:
    udp::socket m_socket;
    std::array<char,conn_buf_size> recv_buf;
    size_t recv_len;
    udp::endpoint reciever_endpoint;

    SWSRSlidingWindow<GameEventData>& m_game_events;
    SWSRSlidingWindow<ErrorData>& m_error_messages;

    void StartRecieve();

    void HandleRecieve(const boost::system::error_code& ec, std::size_t len);
    void InterpretMessage();
    void HandleSend(std::shared_ptr<std::string> message_ptr){};
};