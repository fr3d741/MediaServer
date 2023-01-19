#pragma once

#include <string>
#include <memory>

class IMessageQueue {
public:
    typedef std::shared_ptr<IMessageQueue> Ptr;

    virtual void Add(const std::string& msg) = 0;
    virtual std::string Pop() = 0;
    virtual bool HasMessage() const = 0;
};
