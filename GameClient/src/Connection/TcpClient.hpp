#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

#include "ClientServerContract.hpp"
#include "TcpConnection.hpp"
#include "SWSRSlidingWindow.hpp"

using boost::asio::ip::tcp;

class TcpClient
{
private:
    boost::asio::io_context& m_io;
    tcp::resolver::results_type m_endpoints;
    SWSRSlidingWindow<GameSessionData>& m_game_sessions_window;
    SWSRSlidingWindow<ErrorData>& m_error_messages;
public:
    TcpClient(
        boost::asio::io_context& io,
        SWSRSlidingWindow<GameSessionData>& game_sessions_window,
        SWSRSlidingWindow<ErrorData>& error_messages
    );
    ~TcpClient();

    void Send(std::shared_ptr<std::string> message, bool response);
};
