#include "TcpClient.hpp"

TcpClient::TcpClient(boost::asio::io_context& io) :
    m_io(io)
{
    tcp::resolver resolver(io);
    m_endpoints =
      resolver.resolve("127.0.0.1", "14");
}

TcpClient::~TcpClient()
{
}

void TcpClient::Send(std::shared_ptr<std::string> message, bool response)
{
    std::cout << "TcpClient::Send Begin" << std::endl;
    try
    {
        std::shared_ptr<TcpConnection> newConPointer = TcpConnection::Create(m_io, m_endpoints);
        newConPointer->StartSend(message, response);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }    
    std::cout << "TcpClient::Send End" << std::endl;
}
