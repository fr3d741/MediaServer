#pragma once

#include "Macros.h"
#include <Utility/IMessageQueue.h>
#include <Logging/ILogger.h>
#include <Utility/StreamWriter.h>

#include <memory>
#include <string>
#include <set>
#include <map>

class DLL MediaServer {

    Logging::ILogger::Ptr _logger;
    IMessageQueue::Ptr _queue;
    std::set<string> _cache;
    std::map<std::string, int> _path_types;
    IStreamWriter::Ptr _writer;

public:
    MediaServer(Logging::ILogger::Ptr logger, IMessageQueue::Ptr queue, IStreamWriter::Ptr writer);

    void Start();
    void RegisterPath(const std::string& path, int type);

private:
    void processMessage(std::string& message);
};
