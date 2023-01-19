#pragma once

#include <IMessageQueue.h>
#include <ILogger.h>

#include <vector>
#include <string>

class MessageQueue;

namespace File_System {

    class WinFSWatcher {
    public:

        static void WatchDirectories(Logging::ILogger::Ptr logger, std::vector<std::string>& paths, IMessageQueue::Ptr queue);
    };

}
