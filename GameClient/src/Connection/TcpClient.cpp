#include "TcpClient.hpp"

TcpClient::TcpClient(
    boost::asio::io_context& io,
    SWSRSlidingWindow<GameSessionData>& game_sessions_window,
    SWSRSlidingWindow<ErrorData>& error_messages
) :
    m_io(io),
    m_game_sessions_window(game_sessions_window),
    m_error_messages(error_messages)
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
        std::shared_ptr<TcpConnection> newConPointer = TcpConnection::Create(
            m_io, m_endpoints,
            m_game_sessions_window,
            m_error_messages
        );
        newConPointer->StartSend(message, response);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }    
    std::cout << "TcpClient::Send End" << std::endl;
}
