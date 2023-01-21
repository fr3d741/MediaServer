#pragma once

namespace Networking {

    class ITcpServer
    {
    public:
        virtual ~ITcpServer() = default;

        virtual void ListenOn(unsigned short port) = 0;
    };
}
