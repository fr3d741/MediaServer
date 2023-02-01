#pragma once

#include "Macros.h"
#include <string>
#include <memory>
#include <queue>

class DLL IMessageQueue {
public:
    typedef std::shared_ptr<IMessageQueue> Ptr;

    virtual void Add(const std::string& msg) = 0;
    virtual std::string Pop() = 0;
    virtual std::queue<std::string> BulkPop() = 0;
    virtual bool HasMessage() const = 0;
};
