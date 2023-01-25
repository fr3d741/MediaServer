#pragma once

#include <shared_mutex>
#include <string>
#include <queue>

#include <Utility/IMessageQueue.h>

class MessageQueue : public IMessageQueue
{
    mutable std::shared_mutex _mtx;
    std::queue<std::string> _queue;
public:

    void Add(const std::string& msg) override;
    std::string Pop() override;
    virtual std::queue<std::string> BulkPop() override;
    bool HasMessage() const override;
};

