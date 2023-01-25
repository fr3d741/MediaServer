#pragma once

#include "Macros.h"
#include <Configuration/IConfiguration.h>
#include <Logging/ILogger.h>

#include <string>

class DLL ConfigurationFactory
{
public:
    static IConfiguration::Ptr LoadConfiguration(Logging::ILogger::Ptr logger, const std::string& path);
};

