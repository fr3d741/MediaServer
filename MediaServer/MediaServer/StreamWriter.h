#pragma once

#include <memory>
#include <string>

class IStreamWriter {
public:
    typedef std::shared_ptr<IStreamWriter> Ptr;

    virtual void Write(const std::string& path, const std::string& content) = 0;
};

class StreamWriter : public IStreamWriter {
public:

    void Write(const std::string& path, const std::string& content) override;
};

