#pragma once

namespace Networking {

    class ITcpServer
    {
    public:
        virtual void ListenOn(unsigned short port) = 0;
    };
}
