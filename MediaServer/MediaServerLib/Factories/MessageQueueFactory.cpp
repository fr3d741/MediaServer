#include <Factories/MessageQueueFactory.h>

#include <Utility/MessageQueue.h>

IMessageQueue::Ptr
MessageQueueFactory::CreateMessageQueue() {
    return std::make_shared<MessageQueue>();
}