#pragma once

#include "Macros.h"
#include <memory>
#include <string>
#include <list>
#include <map>

class DLL IConfiguration {

protected:
    IConfiguration() = default;
    IConfiguration(const IConfiguration&) = delete;
    IConfiguration& operator=(const IConfiguration&) = delete;
public:
    typedef std::shared_ptr<IConfiguration> Ptr;

    static IConfiguration& Instance();
    static void Instance(Ptr configuration);

    virtual bool IsDebug() = 0;
    virtual std::map<std::string, int> GetPaths() = 0;
    virtual std::string GetApiKey() = 0;
};
