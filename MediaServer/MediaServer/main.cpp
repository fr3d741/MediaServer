#include <MediaServer.h>

#include <CommonDefines.h>
#include <JsonNode.h>
#include <RestApi.h>
#include <Logger.h>
#include <MessageQueue.h>
#include <FileSystemWatcher.h>
#include <TcpServerFactory.h>
#include <ITcpServer.h>
#include <Configuration.h>
#include <GeneralUtilities.h>
#include <StreamWriter.h>
#include <XmlNode.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

static JsonNode::Ptr 
ReadConfigFile(Logging::ILogger::Ptr logger) {

    std::ifstream stream;
    stream.open("config.json", std::ios::in);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return JsonNode::Parse(logger, buffer.str());
}

class MockWriter :public IStreamWriter {

    std::wofstream out;
    bool is_good = false;
public:
    MockWriter() {
        out.open("f:\\tmp\\log.txt");
        is_good = out.is_open() && out.good();
    }

    void Write(const std::string& path, const std::string& content) {

        out << GeneralUtilities::Convert(path) << "\t\t" << GeneralUtilities::Convert(content) << std::endl;
        out.flush();
    }

    void Write(const std::wstring& path, const std::wstring& content) {

        out << path << "\t\t" << content << std::endl;
        out.flush();
    }

    void Write(const std::string& path, const std::wstring& content) {

        out << GeneralUtilities::Convert(path) << "\t\t" << content << std::endl;
        out.flush();
    }
};

int main()
{
    std::setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));

    IConfiguration::Instance(std::make_shared<Configuration>());

    auto logger = std::make_shared<Logging::Logger>();
    logger->Start();
    auto ptr = ReadConfigFile(logger);

    RestApi::ApiKey() = ptr->GetString("api_key");
    auto message_queue = std::make_shared<MessageQueue>();

    auto server = MediaServer(logger, message_queue, std::make_shared<MockWriter>());
    File_System::FileSystemWatcher fs(logger, message_queue);
    auto json_array = ptr->GetArray("paths_to_watch");
    std::cout << "Paths:" << std::endl;
    for (auto js : json_array) {
        auto p = js->GetString("path");
        auto t = js->GetInt("type");
        fs.AddPath(p);
        server.RegisterPath(p, t);

        {

            auto json = JsonNode::Create(logger);
            json->Add(KeyWords(Keys::NodeType), static_cast<int>(Tags::Path_Update));
            json->Add(TagWords(Tags::Path_Update), p);
            message_queue->Add(json->ToString());

            std::cout << p << " " << (t == MediaType::Movie ? "[Movie]" : "[TvShow]") << std::endl;
        }
    }

    fs.Start();

    auto tcp_server = Factories::CreateTcpServer(message_queue);
    tcp_server->ListenOn(0);

    server.Start();

    fs.Stop();
    logger->Stop();

    return 1;
}
