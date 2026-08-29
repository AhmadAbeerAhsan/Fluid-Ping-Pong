#include "UdpClient.hpp"

UdpClient::UdpClient(boost::asio::io_context& io) :
    m_socket(io)
{
    std::cout << "UdpClient::UdpClient Begin" << std::endl;
    udp::resolver resolver{io};
    reciever_endpoint = 
        *resolver.resolve(udp::v4(), "127.0.0.1", std::to_string(server_udp_port)).begin();

    std::cout << "Resolved to " << reciever_endpoint.address().to_string() 
        << ":" << reciever_endpoint.port() << '\n';

    m_socket.open(udp::v4());
    m_socket.bind(udp::endpoint(udp::v4(), 0));
    StartRecieve();
    std::cout << "UdpClient::UdpClient End" << std::endl;
}

void UdpClient::StartSend(std::shared_ptr<std::string>& message_ptr)
{
    std::cout << "UdpClient::StartSend Begin" << std::endl;
    m_socket.async_send_to(
        boost::asio::buffer(*message_ptr),
        reciever_endpoint,
        std::bind(
            &UdpClient::HandleSend,
            this,
            message_ptr
        )
    );
    std::cout << "UdpClient::StartSend End" << std::endl;
}

void UdpClient::Close()
{
    boost::system::error_code ec;
    m_socket.close();
}

void UdpClient::StartRecieve()
{
    std::cout << "UdpClient::StartRecieve Begin" << std::endl;
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
    std::cout << "UdpClient::StartRecieve End" << std::endl;
}

void UdpClient::HandleRecieve(const boost::system::error_code& ec, std::size_t len)
{
    std::cout << "UdpClient::HandleRecieve Begin" << std::endl;
    if (ec)
    {
        std::cout << "UdpClient::HandleRecieve error: " << ec.message() << std::endl;
    }
    std::cout.write(recv_buf.data(), len);
    std::cout << '\n';  

    StartRecieve();
    std::cout << "UdpClient::HandleRecieve End" << std::endl;
}