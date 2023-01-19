#pragma once

#include <shared_mutex>
#include <string>
#include <queue>

#include <IMessageQueue.h>

class MessageQueue : public IMessageQueue
{
    mutable std::shared_mutex _mtx;
    std::queue<std::string> _queue;
public:

    void Add(const std::string& msg) override;
    std::string Pop() override;
    bool HasMessage() const override;
};

