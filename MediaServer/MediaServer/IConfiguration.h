#pragma once

#include <memory>


class IConfiguration {

protected:
    IConfiguration() = default;
    IConfiguration(const IConfiguration&) = delete;
    IConfiguration& operator=(const IConfiguration&) = delete;
public:

    static IConfiguration& Instance();
    static void Instance(std::shared_ptr<IConfiguration> configuration);

    virtual bool IsDebug() = 0;
};
