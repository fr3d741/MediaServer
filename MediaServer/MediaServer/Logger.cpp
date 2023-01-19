#include <Logger.h>
#include <thread>
#include <chrono>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <XmlNode.h>

using namespace Logging;
using namespace std::chrono_literals;

static std::ofstream
fileStream() {

    std::ofstream stream;
    auto now = std::chrono::steady_clock::now();
    auto file = std::to_string(now.time_since_epoch().count());
    auto file_path = "logs\\" + file + ".mslog";
    auto p = std::filesystem::current_path();
    std::cout << p.string() << std::endl;
    stream.open(file_path, std::ios::out);
    auto ii = stream.is_open();
    return stream;
}

static std::string 
GetTimeStamp() {

#pragma warning(disable : 4996)
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
#pragma warning(default : 4996)
    std::stringstream stream;
    stream << std::put_time(&tm, "%c");
    return stream.str();
}

Logger::Logger() {

    static std::ofstream logger_file = fileStream();
    auto isopen = logger_file.is_open();
    auto isgood = logger_file.good();

    static std::thread t([&]() {

        while (true)
        {
            std::shared_ptr<std::list<std::string>> replica_messages = std::make_shared<std::list<std::string>>();
            {
                std::lock_guard guard(_mutex);
                _messages.swap(replica_messages);
            }

            if (replica_messages != nullptr && !replica_messages->empty()) {
                for (auto msg : *replica_messages) {
                    logger_file << msg << std::endl;
                }
                logger_file.flush();
                logger_file.close();
                break;
            }

            std::this_thread::sleep_for(1min);
        }
    });
}

void
Logger::LogMessage(const char* msg) {

    XmlNode root("entry");
    root.AddAttribute("time_stamp", GetTimeStamp());
    root.AddChild("message", msg);
    {
        std::lock_guard guard(_mutex);
        _messages->push_back(root.Dump());
    }

    //std::cout << msg << std::endl;
}

void
Logger::LogMessage(const std::string& msg) {

    LogMessage(msg.c_str());
}
