#include <TcpServerFactory.h>

#include <ITcpServer.h>
#ifdef _WIN64
    #include <WinTcpServer.h>
#endif // _WIN64

#include <memory>

std::unique_ptr<Networking::ITcpServer> 
Factories::CreateTcpServer(IMessageQueue::Ptr messageQueue) {
#ifdef _WIN64
    return std::make_unique<Networking::WinTcpServer>(std::weak_ptr<Logging::ILogger>(), messageQueue);
#endif // _WIN64

}
    