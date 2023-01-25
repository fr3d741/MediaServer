#pragma once

#include "Macros.h"
#include <Utility/IMessageQueue.h>
#include <Logging/ILogger.h>

#include <string>
#include <vector>

namespace File_System {

    class DLL IFileSystemWatcher {
    public:
        virtual void AddPath(const std::string& path) = 0;
        virtual void AddPath(const std::vector<std::string>& paths) = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
    };

    class DLL FileSystemWatcher : public IFileSystemWatcher {

        Logging::ILogger::Ptr _logger;
        IMessageQueue::Ptr _queue;
        std::vector<std::string> _paths;
        bool _started = false;
    public:
        FileSystemWatcher(Logging::ILogger::Ptr logger, IMessageQueue::Ptr queue);
        void AddPath(const std::string& path) override;
        void AddPath(const std::vector<std::string>& paths) override;
        void Start() override;
        void Stop() override;
    };

}
