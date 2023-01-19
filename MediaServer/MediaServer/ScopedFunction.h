#pragma once

#include <functional>

class ScopedFunction {
    std::function<void()> _callOnDtor;
public:
    ScopedFunction(std::function<void()> callOnDtor);
    ~ScopedFunction();

};
