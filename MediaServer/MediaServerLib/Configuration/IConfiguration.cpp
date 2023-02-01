#include <Configuration/IConfiguration.h>

#include <mutex>

std::once_flag instance_flag;
std::shared_ptr<IConfiguration> _instance;

IConfiguration& 
IConfiguration::Instance() {
    return *_instance;
}

void 
IConfiguration::Instance(Ptr configuration) {
    std::call_once(instance_flag, [=]() { _instance = configuration; });
}
