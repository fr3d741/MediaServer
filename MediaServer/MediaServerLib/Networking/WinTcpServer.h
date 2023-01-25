#pragma once

#include <Networking/ITcpServer.h>
#include <Utility/IMessageQueue.h>

#include <memory>

namespace Logging {
    class ILogger;
}

namespace Networking {

    class WinTcpServer : public ITcpServer
    {
        std::weak_ptr<Logging::ILogger> _logger;
        IMessageQueue::Ptr _messageQueue;
    public:
        WinTcpServer(std::weak_ptr<Logging::ILogger> logger, IMessageQueue::Ptr messageQueue);

        void ListenOn(unsigned short port) override;

    private:
        int listenCore(unsigned short port);
    };

}
