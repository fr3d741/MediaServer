#pragma once

#include <Utility/IMessageQueue.h>
#include <Logging/ILogger.h>

#include <vector>
#include <string>

class MessageQueue;

namespace File_System {

    class WinFSWatcher {
    public:

        static void WatchDirectories(Logging::ILogger::Ptr logger, const std::vector<std::string>& paths, IMessageQueue::Ptr queue);
    };

}
