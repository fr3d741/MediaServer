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

static JsonNode::Ptr 
ReadConfigFile(Logging::ILogger::Ptr logger) {

    std::ifstream stream;
    stream.open("config.json", std::ios::in);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return JsonNode::Parse(logger, buffer.str());
}

class MockWriter :public IStreamWriter {

    std::ofstream out;
    bool is_good = false;
public:
    MockWriter() {
        out.open("f:\\tmp\\log.txt");
        is_good = out.is_open() && out.good();
    }

    void Write(const std::string& path, const std::string& content) {

        XmlNode root("file");
        root.AddAttribute("path", path);
        root.AddChild("value", content);
        out << root.Dump();
        out.flush();
    }
};

int main()
{
    IConfiguration::Instance(std::make_shared<Configuration>());

    auto logger = std::make_shared<Logging::Logger>();
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

    return 1;
}


//#include <iostream>
//#include <nlohmann/json.hpp>
//
//using json = nlohmann::json;
//
//int main()
//{
//    // create JSON arrays
//    json j_no_init_list = json::array();
//    json j_empty_init_list = json::array({});
//    json j_nonempty_init_list = json::array({ "a", "b", "c", "d"});
//    json j_list_of_pairs = json::array({ {"one", 1}, {"two", 2} });
//
//    // serialize the JSON arrays
//    std::cout << j_no_init_list << '\n';
//    std::cout << j_empty_init_list << '\n';
//    std::cout << j_nonempty_init_list << '\n';
//    std::cout << j_list_of_pairs << '\n';
//
//    auto js = json::parse("[\"a\", \"b\", \"c\", \"d\"]");
//    std::cout << "aa:" << js[2] << std::endl;
//    std::cout << js.dump();
//}

//
//#include <iostream>
//#include <iomanip>
//#include <nlohmann/json.hpp>
//
//using json = nlohmann::json;
//
//int main()
//{
//    // create a JSON object
//    json j;
//    //=
//    //{
//    //    {"pi", 3.141},
//    //    {"happy", true},
//    //    {"name", "Niels"},
//    //    {"nothing", nullptr},
//    //    {
//    //        "answer", {
//    //            {"everything", 42}
//    //        }
//    //    },
//    //    {"list", {1, 0, 2}},
//    //    {
//    //        "object", {
//    //            {"currency", "USD"},
//    //            {"value", 42.99}
//    //        }
//    //    }
//    //};
//
//    // add new values
//    j["new"]["key"]["value"] = { "another", "list" };
//
//    // count elements
//    auto s = j.size();
//    j["size"] = s;
//
//    // pretty print with indent of 4 spaces
//    std::cout << std::setw(4) << j << '\n';
//}
