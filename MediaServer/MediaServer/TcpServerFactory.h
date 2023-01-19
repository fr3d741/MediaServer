#pragma once

#include <IMessageQueue.h>

#include <memory>

namespace Networking {
    class ITcpServer;
}

namespace Factories {

    std::unique_ptr<Networking::ITcpServer> CreateTcpServer(IMessageQueue::Ptr messageQueue);
}
