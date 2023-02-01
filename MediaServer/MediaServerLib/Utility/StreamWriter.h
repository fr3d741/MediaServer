#pragma once

#include "Macros.h"
#include <CommonDefines.h>
#include <memory>

class DLL IStreamWriter {
public:
    typedef std::shared_ptr<IStreamWriter> Ptr;

    virtual void Write(const std::string& path, const std::string& content) = 0;
    virtual void Write(const string& path, const string& content) = 0;
    virtual void Write(const std::string& path, const string& content) = 0;
};

class DLL StreamWriter : public IStreamWriter {
public:

    void Write(const std::string& path, const std::string& content) override;
    void Write(const string& path, const string& content) override;
    void Write(const std::string& path, const string& content) override;
};

