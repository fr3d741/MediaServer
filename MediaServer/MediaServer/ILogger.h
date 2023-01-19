#pragma once

#include <string>
#include <memory>

namespace Logging {

    class ILogger {
    public:
        typedef std::shared_ptr<ILogger> Ptr;

        virtual void LogMessage(const char* msg) = 0;
        virtual void LogMessage(const std::string& msg) = 0;
    };
}