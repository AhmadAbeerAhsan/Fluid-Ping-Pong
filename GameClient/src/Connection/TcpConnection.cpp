#include "TcpConnection.hpp"

TcpConnection::TcpConnection(boost::asio::io_context& io, tcp::resolver::results_type& endpoints):
    m_socket{io}
{
    std::cout << "TcpConnection::TcpConnection Begin" << std::endl;
    boost::asio::connect(m_socket, endpoints);
    std::cout << "TcpConnection::TcpConnection End" << std::endl;
}

void TcpConnection::StartSend(std::shared_ptr<std::string> message, bool response)
{
    std::cout << "TcpConnection::StartSend Begin" << std::endl;
    boost::system::error_code ignored_error;
    m_socket.send(
        boost::asio::buffer(*message)
    );
    if (response)
    {
        StartRecieve();
    }
    std::cout << "TcpConnection::StartSend End" << std::endl;
}

void TcpConnection::StartRecieve()
{
    std::cout << "TcpConnection::StartRecieve Begin" << std::endl;
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
    std::cout << "TcpConnection::StartRecieve End" << std::endl;
}

void TcpConnection::HandleRecieve(const boost::system::error_code& ec, std::size_t len)
{
    std::cout << "TcpConnection::HandleRecieve Begin" << std::endl;
    if (ec)
    {
        std::cout << "HandleRecieve error: " << ec.message() << std::endl;
        HandleDisconnect();
        return;   // do NOT touch work_buf or call InterpretMessage
    }

    if (len >= buf_size)
    {
        //return error
        return;
    }
    
    std::istream stream(&m_recv_streambuf);
    stream.read(work_buf.data(), static_cast<std::streamsize>(len));
    work_len = static_cast<int>(len);
    InterpretMessage();

    StartRecieve();
    std::cout << "TcpConnection::HandleRecieve End" << std::endl;
}

void TcpConnection::InterpretMessage()
{
    std::cout << "TcpConnection::InterpretMessage Begin" << std::endl;
    std::cout << "Message: " << work_buf.data() << '\n';
    if (work_len < 4)
    {
        return;
    }
    if (work_buf[0] == contract(Action::Error) && work_buf[1] == contract(Action::EndDeliminator))
    {
        std::cout << "Error" << std::endl;
        return;
    }
    if (work_buf[0] == contract(Action::Success) && work_buf[1] == contract(Action::Deliminator))
    {
        std::cout << "Success" << std::endl;

        if (work_buf[2] == contract(Action::Create) && work_buf[3] == contract(Action::Deliminator))
        {
            int new_session_id = ParseIntegerTillDeliminator(work_buf, 4, work_len, contract(Action::EndDeliminator));

            if (new_session_id != -1)
            {
                std::cout << "Created session id: " << new_session_id << std::endl;
            }
            else
            {
                std::cerr << "Malformed Create response (bad/missing session id)" << std::endl;
            }
            return;
        }
    }
    std::cout << "TcpConnection::InterpretMessage End" << std::endl;
}

void TcpConnection::HandleDisconnect()
{
    std::cout << "TcpConnection::HandleDisconnect Begin" << std::endl;
    boost::system::error_code ec;
    m_socket.close(ec);
    // flag the connection as dead for higher-level game code
    std::cout << "TcpConnection::HandleDisconnect End" << std::endl;
}