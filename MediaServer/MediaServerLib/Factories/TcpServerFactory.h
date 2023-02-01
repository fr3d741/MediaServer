#pragma once

#include "Macros.h"
#include <Utility/IMessageQueue.h>
#include <Networking/ITcpServer.h>

#include <memory>

class DLL TcpServerFactory {
public:
    static std::unique_ptr<Networking::ITcpServer> CreateTcpServer(IMessageQueue::Ptr messageQueue);
};
