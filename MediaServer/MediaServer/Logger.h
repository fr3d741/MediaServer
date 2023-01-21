#pragma once

#include <list>
#include <string>
#include <memory>
#include <mutex>
#include <fstream>
#include <thread>

#include <ILogger.h>

namespace Logging {

    class Logger : public ILogger
    {
        std::ofstream _logger_file;
        std::jthread _worker_thread;
        std::mutex _mutex;
        std::shared_ptr<std::list<std::string>> _messages;
    public:
        Logger();

        void Start();
        void Stop();

        void LogMessage(const char* msg) override;
        void LogMessage(const std::string& msg) override;
        void LogMessage(const std::wstring& msg) override;
    };

}
