#pragma once

#include "Macros.h"
#include <Configuration/IConfiguration.h>
#include <Utility/JsonNode.h>

class DLL Configuration : public IConfiguration
{
    JsonNode::Ptr _node;
public:
    Configuration(JsonNode::Ptr node);

    bool IsDebug() override;

    std::map<std::string, int> GetPaths() override;
    std::string GetApiKey() override;
};

