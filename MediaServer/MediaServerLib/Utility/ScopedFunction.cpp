#include <Utility/ScopedFunction.h>

ScopedFunction::ScopedFunction(std::function<void()> callOnDtor) 
    : _callOnDtor(callOnDtor) {
}

ScopedFunction::~ScopedFunction() {
    _callOnDtor();
}
