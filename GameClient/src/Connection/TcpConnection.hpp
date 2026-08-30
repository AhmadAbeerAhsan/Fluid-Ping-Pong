#pragma once

#include <boost/asio.hpp>
#include "ClientServerContract.hpp"
#include <memory>
#include <iostream>
#include <format>
#include "SWSRSlidingWindow.hpp"

using boost::asio::ip::tcp;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    static std::shared_ptr<TcpConnection> Create(
        boost::asio::io_context& io,
        tcp::resolver::results_type& endpoints,
        SWSRSlidingWindow<GameSessionData>& ui_events_raw,
        SWSRSlidingWindow<ErrorData>& error_messages
    )
    {
        return std::shared_ptr<TcpConnection>(new TcpConnection(
            io,
            endpoints,
            ui_events_raw,
            error_messages
        ));
    }

    tcp::socket& GetSocket(){ return m_socket; }

    void StartSend(std::shared_ptr<std::string> message, bool response);
private:
    TcpConnection(
        boost::asio::io_context& io,
        tcp::resolver::results_type& endpoints,
        SWSRSlidingWindow<GameSessionData>& game_sessions_window,
        SWSRSlidingWindow<ErrorData>& error_messages
    );

    tcp::socket m_socket;
    boost::asio::streambuf m_recv_streambuf;
    ConnectionBuf work_buf;
    size_t work_len;
    int m_possible_new_session_id;

    SWSRSlidingWindow<GameSessionData>& m_game_sessions_window;
    SWSRSlidingWindow<ErrorData>& m_error_messages;
    void StartRecieve();
    void HandleRecieve(const boost::system::error_code& ec, std::size_t len);
    void HandleDisconnect();

    void InterpretMessage();
};