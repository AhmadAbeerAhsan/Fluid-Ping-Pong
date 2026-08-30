#include "UdpClient.hpp"

UdpClient::UdpClient(
    boost::asio::io_context& io,
    SWSRSlidingWindow<GameEventData>& game_events,
    SWSRSlidingWindow<ErrorData>& error_messages
) :
    m_socket(io),
    m_game_events(game_events),
    m_error_messages(error_messages)
{
    std::cout << "UdpClient::UdpClient Begin\n";
    udp::resolver resolver{io};
    reciever_endpoint = 
        *resolver.resolve(udp::v4(), "127.0.0.1", std::to_string(server_udp_port)).begin();

    std::cout << std::format(
        "{} {}:{}\n",
        reciever_endpoint.address().to_string(),
        "Message:",
        reciever_endpoint.port()
    );

    m_socket.open(udp::v4());
    m_socket.bind(udp::endpoint(udp::v4(), 0));
    StartRecieve();
    std::cout << "UdpClient::UdpClient End\n";
}

void UdpClient::StartSend(std::shared_ptr<std::string> message_ptr)
{
    std::cout << "UdpClient::StartSend Begin\n";
    m_socket.async_send_to(
        boost::asio::buffer(*message_ptr),
        reciever_endpoint,
        std::bind(
            &UdpClient::HandleSend,
            this,
            message_ptr
        )
    );
    std::cout << "UdpClient::StartSend End\n";
}

void UdpClient::Close()
{
    boost::system::error_code ec;
    m_socket.close();
}

void UdpClient::StartRecieve()
{
    std::cout << "UdpClient::StartRecieve Begin\n";
    std::shared_ptr<udp::endpoint> remote_endpoint_ptr{ std::make_shared<udp::endpoint>() };
    m_socket.async_receive_from(
        boost::asio::buffer(recv_buf),
        reciever_endpoint,
        std::bind(
            &UdpClient::HandleRecieve,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
    std::cout << "UdpClient::StartRecieve End\n";
}

void UdpClient::HandleRecieve(const boost::system::error_code& ec, std::size_t len)
{
    std::cout << "UdpClient::HandleRecieve Begin\n";
    if (ec)
    {
        std::cout << "UdpClient::HandleRecieve error: " << ec.message() << std::endl;
    }

    recv_len = len;
    InterpretMessage();
    StartRecieve();
    std::cout << "UdpClient::HandleRecieve End\n";
}

void UdpClient::InterpretMessage()
{
    std::cout << std::format(
        "{} {} {}\n","TcpConnection::InterpretMessage Begin:", "Message:", recv_buf.data()
    );
    if (recv_len < 4)
    {
        return;
    }  

    if (recv_buf[0] == contract(Action::Success) && recv_buf[1] == contract(Action::Deliminator))
    {
        GameEventData g{recv_buf, recv_len};
        m_game_events.Push(g);
        return;
    }

    if (recv_buf[0] == contract(Action::Error) && recv_buf[1] == contract(Action::Deliminator))
    {
        ErrorData e{recv_buf, recv_len};
        m_error_messages.Push(e);
        return;
    }
    std::cout << "UdpClient::InterpretMessage End\n";
}
