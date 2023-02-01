#include <Factories/ConfigurationFactory.h>

#include <Configuration/Configuration.h>
#include <Utility/JsonNode.h>

#include <fstream>
#include <sstream>

IConfiguration::Ptr 
ConfigurationFactory::LoadConfiguration(Logging::ILogger::Ptr logger, const std::string& path) {

    std::ifstream stream;
    //stream.open("config.json", std::ios::in);
    stream.open(path, std::ios::in);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    stream.close();

    auto node = JsonNode::Parse(logger, buffer.str());
    return std::make_shared<Configuration>(node);
}
