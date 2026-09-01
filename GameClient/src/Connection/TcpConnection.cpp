#include "TcpConnection.hpp"

TcpConnection::TcpConnection(
    boost::asio::io_context& io,
    tcp::resolver::results_type& endpoints,
    SWSRSlidingWindow<GameSessionData>& game_sessions_window,
    SWSRSlidingWindow<ErrorData>& error_messages
):
    m_socket{io},
    m_game_sessions_window(game_sessions_window),
    m_error_messages(error_messages)
{
    std::cout << "TcpConnection::TcpConnection Begin\n";
    boost::asio::connect(m_socket, endpoints);
    std::cout << "TcpConnection::TcpConnection End\n";
}

void TcpConnection::StartSend(std::shared_ptr<std::string> message, bool response)
{
    std::cout << "TcpConnection::StartSend Begin\n";
    boost::system::error_code ignored_error;
    m_socket.send(
        boost::asio::buffer(*message)
    );
    if (response)
    {
        StartRecieve();
    }
    std::cout << "TcpConnection::StartSend End\n";
}

void TcpConnection::StartRecieve()
{
    std::cout << "TcpConnection::StartRecieve Begin\n";
    boost::asio::async_read_until(
        m_socket,
        m_recv_streambuf,
        contract(Action::EndDeliminator),
        std::bind(
            &TcpConnection::HandleRecieve,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
    std::cout << "TcpConnection::StartRecieve End\n";
}

void TcpConnection::HandleRecieve(const boost::system::error_code& ec, std::size_t len)
{
    std::cout << "TcpConnection::HandleRecieve Begin\n";
    if (ec)
    {
        std::string leftover(
            boost::asio::buffers_begin(m_recv_streambuf.data()),
            boost::asio::buffers_end(m_recv_streambuf.data())
        );
        std::cout << "HandleRecieve error: " << ec.message()
                << " | buffered so far: [" << leftover << "]\n";
        HandleDisconnect();
        return;
    }

    if (len >= conn_buf_size)
    {
        std::cout << "TcpConnection::HandleRecieve len >= conn_buf_size\n";
        return;
    }

    std::cout << std::format(
        "{} {} {}\n","TcpConnection::HandleRecieve:", "Message:", work_buf.data()
    );
    
    std::istream stream(&m_recv_streambuf);
    stream.read(work_buf.data(), static_cast<std::streamsize>(len));
    work_len = static_cast<int>(len);

    try
    {
        InterpretMessage();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    //StartRecieve();
    std::cout << "TcpConnection::HandleRecieve End\n";
}

void TcpConnection::InterpretMessage()
{
    std::cout << std::format(
        "{} {} {}\n","TcpConnection::InterpretMessage Begin:", "Message:", work_buf.data()
    );
    if (work_len < 4)
    {
        return;
    }
    if (work_buf[0] == contract(Action::Error) && work_buf[1] == contract(Action::EndDeliminator))
    {
        std::cout << "Error\n";
        return;
    }
    if (work_buf[0] == contract(Action::Success) && work_buf[1] == contract(Action::Deliminator))
    {
        std::cout << "Success\n";

        if (work_buf[2] == contract(Action::Create) && work_buf[3] == contract(Action::Deliminator))
        {
            GameSessionData new_game_session_data{work_buf, work_len};
            m_game_sessions_window.Push(new_game_session_data);

            return;
        }

        if (work_buf[2] == contract(Action::ListSessions) && work_buf[3] == contract(Action::Deliminator))
        {
            GameSessionData new_game_session_data{work_buf, work_len};
            m_game_sessions_window.Push(new_game_session_data);
            
            return;
        }
    }
    std::cout << "TcpConnection::InterpretMessage End\n";
}

void TcpConnection::HandleDisconnect()
{
    std::cout << "TcpConnection::HandleDisconnect Begin\n";
    boost::system::error_code ec;
    m_socket.close(ec);
    // flag the connection as dead for higher-level game code
    std::cout << "TcpConnection::HandleDisconnect End\n";
}