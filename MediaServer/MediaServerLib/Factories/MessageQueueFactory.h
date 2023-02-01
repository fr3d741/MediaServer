#pragma once

#include "Macros.h"
#include <Utility/IMessageQueue.h>

class DLL MessageQueueFactory {
public:
    static IMessageQueue::Ptr CreateMessageQueue();
};
