#pragma once

#include <list>
#include <string>
#include <memory>
#include <mutex>

#include <ILogger.h>

namespace Logging {

    class Logger : public ILogger
    {
        std::mutex _mutex;
        std::shared_ptr<std::list<std::string>> _messages;
    public:
        Logger();

        void LogMessage(const char* msg) override;
        void LogMessage(const std::string& msg) override;
    };

}
