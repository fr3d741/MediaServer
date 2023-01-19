#include <FileSystemWatcher.h>
#include <thread>

#ifdef _WIN64
    #include "WinFSWatcher.h"
#endif

using namespace File_System;

FileSystemWatcher::FileSystemWatcher(Logging::ILogger::Ptr logger, IMessageQueue::Ptr message_queue)
    : _logger(logger)
    , _queue(message_queue) {

}

void 
FileSystemWatcher::AddPath(const std::string& path) {

    _paths.push_back(path);
    if (_started == false)
        return;

#ifdef _WIN64
    WinFSWatcher::WatchDirectories(_logger, _paths, _queue);
#elif __linux__
    // TODO:
#endif
}

void 
FileSystemWatcher::AddPath(const std::vector<std::string>& paths) {

    _paths.insert(_paths.end(), paths.begin(), paths.end());
    if (_started == false)
        return;

#ifdef _WIN64
    WinFSWatcher::WatchDirectories(_logger, _paths, _queue);
#elif __linux__
    // TODO:
#endif
}

void 
FileSystemWatcher::Start() {

    _started = true;
    WinFSWatcher::WatchDirectories(_logger, _paths, _queue);
}
