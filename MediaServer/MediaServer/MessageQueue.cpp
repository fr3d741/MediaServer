#include <MessageQueue.h>

void 
MessageQueue::Add(const std::string& msg) {

    std::unique_lock<std::shared_mutex> l(_mtx);
    _queue.push(msg);
}

std::string 
MessageQueue::Pop() {

    std::unique_lock<std::shared_mutex> l(_mtx);
    auto val = _queue.front();
    _queue.pop();
    return val;
}

bool MessageQueue::HasMessage() const {

    std::shared_lock<std::shared_mutex> lock(_mtx);
    return !_queue.empty();
}
