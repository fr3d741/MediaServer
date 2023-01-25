#pragma once

#include "Macros.h"
#include <Logging/ILogger.h>
#include <CommonDefines.h>

#include <memory>
#include <vector>

class JsonNodeImpl;

class DLL JsonNode {

    std::shared_ptr<JsonNodeImpl> _impl;
    Logging::ILogger::Ptr _logger;
    JsonNode(Logging::ILogger::Ptr logger);

public:
    typedef std::shared_ptr<JsonNode> Ptr;

    void Add(const std::string& key, int value);
    void Add(const std::string& key, const std::string& value);
    void Add(const std::string& key, double value);
    void Add(const std::string& key, bool value);
    void Add(const std::string& key, Ptr value);

    bool Get(const std::string& key, int& value);
    bool Get(const std::string& key, std::string& value);
    bool Get(const std::string& key, double& value);
    bool Get(const std::string& key, bool& value);
    bool Get(const std::string& key, Ptr& value);
    bool Get(const std::string& key, std::vector<Ptr>& value);

    int GetInt(const std::string& key);
    std::string GetString(const std::string& key);
    string GetWString(const std::string& key);
    double GetDouble(const std::string& key);
    bool GetBool(const std::string& key);
    Ptr GetPointer(const std::string& key);
    std::vector<Ptr> GetArray(const std::string& key);
    std::vector<std::string> GetArrayOfString(const std::string& key);

    int GetInt();
    std::string GetString();
    double GetDouble();
    bool GetBool();
    Ptr GetPointer();
    std::vector<Ptr> GetArray();
    std::vector<std::string> GetArrayOfString();

    std::string GetAsString(const std::string& key);

    bool Has(const std::string& key);

    std::string ToString();

    // Utilities
    std::vector<std::string> FindTagRecursive(const std::string& key);

    // static classes
    static Ptr Parse(Logging::ILogger::Ptr logger, const std::string& json);
    static Ptr Create(Logging::ILogger::Ptr logger);
    static Ptr Create(std::shared_ptr<JsonNodeImpl> ptr);
};
