#pragma once

#include "Macros.h"
#include <CommonDefines.h>

#include <memory>

namespace Logging {

    class DLL ILogger {
    public:
        typedef std::shared_ptr<ILogger> Ptr;

        virtual void LogMessage(const char* msg) = 0;
        virtual void LogMessage(const std::string& msg) = 0;
        virtual void LogMessage(const string& msg) = 0;
    };
}