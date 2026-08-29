#pragma once

#include <boost/asio.hpp>
#include "ClientServerContract.hpp"
#include "GameSession.hpp"
#include <memory>
#include <iostream>
#include <format>

using boost::asio::ip::tcp;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    static std::shared_ptr<TcpConnection> Create(boost::asio::io_context& io, tcp::resolver::results_type& endpoints)
    {
        return std::shared_ptr<TcpConnection>(new TcpConnection(io, endpoints));
    }

    tcp::socket& GetSocket(){ return m_socket; }

    void StartSend(std::shared_ptr<std::string> message, bool response);
private:
    TcpConnection(boost::asio::io_context& io, tcp::resolver::results_type& endpoints);

    std::shared_ptr<std::unordered_map<int, GameSession>> m_game_sessions;
    tcp::socket m_socket;
    boost::asio::streambuf m_recv_streambuf;
    std::array<char,buf_size> work_buf;
    size_t work_len;
    int m_possible_new_session_id;

    void StartRecieve();
    void HandleRecieve(const boost::system::error_code& ec, std::size_t len);
    void HandleDisconnect();

    void InterpretMessage();
};