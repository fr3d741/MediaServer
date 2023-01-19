#pragma once

#include <IConfiguration.h>

class Configuration : public IConfiguration
{
public:
    bool IsDebug() override;
};

