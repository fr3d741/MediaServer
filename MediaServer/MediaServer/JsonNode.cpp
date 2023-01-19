#include <JsonNode.h>

#include <nlohmann/json.hpp>

#include <map>
#include <variant>
#include <limits>
#include <exception>

using Json = nlohmann::json;

class JsonNodeImpl {
public:
    typedef std::variant<int, std::string, double, bool, JsonNode::Ptr, std::vector<JsonNode::Ptr>, std::vector<std::string>> Value;

    virtual void Add(const std::string& key, Value value) = 0;

    virtual bool Has(const std::string& key) = 0;

    virtual Value Get(const std::string& key) = 0;

    virtual Value Get() { return Value(); };

    virtual Json Build() = 0;

    virtual std::list<std::string> FindTagValues(const std::string& key) { return {}; }
};

class CleanJsonNodeImpl : public JsonNodeImpl {
    Logging::ILogger::Ptr _logger;
public:
    std::map<std::string, Value> _members;

    CleanJsonNodeImpl(Logging::ILogger::Ptr logger) : _logger(logger) {}

    void Add(const std::string& key, Value value) {
        _members[key] = value;
    }

    bool Has(const std::string& key) {
        return _members.contains(key);
    }

    Value Get(const std::string& key) {
        return _members[key];
    }

    Json Build() {

        auto rp = std::make_shared<Json>();
        auto&& root = *rp;
        for (auto&& item : _members) {
            auto key = item.first;
            auto variant = item.second;

            auto index = variant.index();
            try {
                switch (index)
                {
                case 0: // int
                    root[key] = std::get<int>(variant);
                    break;
                case 1: // std::string
                    root[key] = std::get<std::string>(variant);;
                    break;
                case 2: // double
                    root[key] = std::get<double>(variant);
                    break;
                case 3: // bool
                    root[key] = std::get<bool>(variant);
                    break;
                case 4: // Ptr
                {
                    auto node = std::get<JsonNode::Ptr>(variant);
                    root[key] = node->ToString();
                }
                break;
                default:
                    // TODO: Logging
                    continue;
                }
            }
            catch (std::bad_variant_access&) {
                // TODO: Logging
            }
        }

        return root;
    }

};

class ImportedJsonNodeImpl : public JsonNodeImpl {
    Logging::ILogger::Ptr _logger;
public:
    Json _json;

    ImportedJsonNodeImpl(Logging::ILogger::Ptr logger, Json json) :_logger(logger), _json(json) { }

    void Add(const std::string&, Value) {}

    bool Has(const std::string& key) {
        return _json.contains(key);
    }

    Value Get(const std::string& key) {

        if (_json.is_array() && !_json.contains(key)) {
            //this as array without key elements, like ["1","2"]
            auto js_size = _json.size();
            std::vector<std::string> nodes;
            nodes.reserve(js_size);
            for(auto i = 0; i < js_size; ++i)
                nodes.push_back(_json[i]);
            return nodes;
        }
        try {
            auto&& json = _json[key];

            if (json.is_number_integer())
                return json.get<int>();
            if (json.is_number_float())
                return json.get<double>();
            if (json.is_boolean())
                return json.get<bool>();
            if (json.is_string())
                return json.get<std::string>();
            if (json.is_object())
                return JsonNode::Create(std::make_shared<ImportedJsonNodeImpl>(_logger, json.get<Json>()));
            if (json.is_array())
            {
                auto jsons = json.get<std::vector<Json>>();
                std::vector<JsonNode::Ptr> nodes;
                nodes.reserve(jsons.size());
                for (auto js : jsons)
                    nodes.push_back(JsonNode::Create(std::make_shared<ImportedJsonNodeImpl>(_logger, js)));
                return nodes;
            }
        }
        catch (std::exception e) {
            _logger->LogMessage(e.what());
        }
        
        return  Value();
    }

    Value Get() {

        if (_json.is_boolean())
            return _json.get<bool>();
        if (_json.is_number_integer())
            return _json.get<int>();
        if (_json.is_number_float())
            return _json.get<double>();
        if (_json.is_string())
            return _json.get<std::string>();
        if (_json.is_array())
            return _json.get<std::vector<std::string>>();
        if (_json.is_object())
            return JsonNode::Create(std::make_shared<ImportedJsonNodeImpl>(_logger, _json.get<Json>()));

        return  Value();
    }

    Json Build() {
        return _json;
    }

    std::list<std::string> FindTagValuesRecursive(Json& json, const std::string& key) {

        if (json.contains(key))
            return { json[key].get<std::string>() };

        std::list<std::string> result;
        if (json.is_array()) {
            
            for (auto i = 0; i < json.size(); ++i) {
                result.merge(FindTagValuesRecursive(json[i], key));
            }
        }

        if (json.is_object()) {
            for (auto it : json) {
                result.merge(FindTagValuesRecursive(it, key));
            }
        }

        return result;
    }

    std::list<std::string> FindTagValues(const std::string& key) override {

        return FindTagValuesRecursive(_json, key);
    }
};

JsonNode::JsonNode(Logging::ILogger::Ptr logger) : _impl(new CleanJsonNodeImpl(logger)){

}

void 
JsonNode::Add(const std::string& key, int value) {

    _impl->Add(key, value);
}

void 
JsonNode::Add(const std::string& key, const std::string& value) {

    _impl->Add(key, value);
}

void 
JsonNode::Add(const std::string& key, double value) {

    _impl->Add(key, value);
}

void 
JsonNode::Add(const std::string& key, bool value) {

    _impl->Add(key, value);
}

void 
JsonNode::Add(const std::string& key, Ptr value) {

    _impl->Add(key, value);
}

bool 
JsonNode::Get(const std::string& key, int& value) {

    auto v = _impl->Get(key);
    try
    {
        value = std::get<int>(v);
    }
    catch (const std::bad_variant_access&)
    {
        return false;
    }
    return true;
}

bool
JsonNode::Get(const std::string& key, std::string& value) {

    auto v = _impl->Get(key);
    try
    {
        value = std::get<std::string>(v);
    }
    catch (const std::bad_variant_access&)
    {
        return false;
    }
    return true;
}

bool
JsonNode::Get(const std::string& key, double& value) {

    auto v = _impl->Get(key);
    try
    {
        value = std::get<double>(v);
    }
    catch (const std::bad_variant_access&)
    {
        return false;
    }
    return true;
}

bool
JsonNode::Get(const std::string& key, bool& value) {

    auto v = _impl->Get(key);
    try
    {
        value = std::get<bool>(v);
    }
    catch (const std::bad_variant_access&)
    {
        return false;
    }
    return true;
}

bool
JsonNode::Get(const std::string& key, Ptr& value) {

    auto v = _impl->Get(key);
    try
    {
        value = std::get<Ptr>(v);
    }
    catch (const std::bad_variant_access&)
    {
        return false;
    }
    return true;
}

bool
JsonNode::Get(const std::string& key, std::vector<Ptr>& value) {

    auto v = _impl->Get(key);
    try
    {
        value = std::get<std::vector<Ptr>>(v);
    }
    catch (const std::bad_variant_access&)
    {
        return false;
    }
    return true;
}

int
JsonNode::GetInt(const std::string& key) {

    auto v = _impl->Get(key);
    try
    {
        return std::get<int>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return std::numeric_limits<int>::infinity();
}

std::string
JsonNode::GetString(const std::string& key) {

    auto v = _impl->Get(key);
    try
    {
        return std::get<std::string>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return "";
}

double
JsonNode::GetDouble(const std::string& key) {

    auto v = _impl->Get(key);
    try
    {
        return std::get<double>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return std::numeric_limits<double>::quiet_NaN();
}

bool
JsonNode::GetBool(const std::string& key) {

    auto v = _impl->Get(key);
    try
    {
        return std::get<bool>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
        throw std::invalid_argument(key);
    }
}

JsonNode::Ptr
JsonNode::GetPointer(const std::string& key) {

    auto v = _impl->Get(key);
    try
    {
        return std::get<JsonNode::Ptr>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return nullptr;
}

std::vector<JsonNode::Ptr>
JsonNode::GetArray(const std::string& key) {

    auto v = _impl->Get(key);
    try
    {
        return std::get<std::vector<JsonNode::Ptr>>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return std::vector<JsonNode::Ptr>();
}

std::vector<std::string> 
JsonNode::GetArrayOfString(const std::string& key) {

    auto v = _impl->Get(key);
    try
    {
        return std::get<std::vector<std::string>>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return std::vector<std::string>();
}

int 
JsonNode::GetInt() {

    auto v = _impl->Get();
    try
    {
        return std::get<int>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return std::numeric_limits<int>::quiet_NaN();
}

std::string 
JsonNode::GetString() {

    auto v = _impl->Get();
    try
    {
        return std::get<std::string>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return "";
}

double 
JsonNode::GetDouble() {

    auto v = _impl->Get();
    try
    {
        return std::get<double>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return std::numeric_limits<double>::quiet_NaN();
}

bool
JsonNode::GetBool() {

    auto v = _impl->Get();
    try
    {
        return std::get<bool>(v);
    }
    catch (const std::bad_variant_access&)
    {
        // TODO:
        throw std::invalid_argument("");
    }
}

JsonNode::Ptr
JsonNode::GetPointer() {

    auto v = _impl->Get();
    try
    {
        return std::get<JsonNode::Ptr>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return nullptr;
}

std::vector<JsonNode::Ptr>
JsonNode::GetArray() {

    auto v = _impl->Get();
    try
    {
        return std::get<std::vector<JsonNode::Ptr>>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return {};
}

std::vector<std::string> 
JsonNode::GetArrayOfString() {

    auto v = _impl->Get();
    try
    {
        return std::get<std::vector<std::string>>(v);
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return {};
}

std::string
JsonNode::GetAsString(const std::string& key) {

    auto v = _impl->Get(key);
    try
    {
        //variant<int, std::string, double, bool, JsonNode::Ptr, std::vector<JsonNode::Ptr>, std::vector<std::string>> Value;
        switch (v.index()) {
        case 0:
            return std::to_string(std::get<int>(v));
        case 1:
            return std::get<std::string>(v);
        case 2:
            return std::to_string(std::get<double>(v));
        default:
            return "";
        }
    }
    catch (const std::bad_variant_access&)
    {
        //TODO: logging
    }
    return "";
}

bool 
JsonNode::Has(const std::string& key) {
    return _impl->Has(key);
}

std::string 
JsonNode::ToString() {

    return _impl->Build().dump();
}

std::vector<std::string> 
JsonNode::FindTagRecursive(const std::string& key) {

    auto result = _impl->FindTagValues(key);
    std::vector<std::string> ret;
    ret.reserve(result.size());
    ret.insert(ret.end(), result.begin(), result.end());
    return ret;
}

JsonNode::Ptr
JsonNode::Parse(Logging::ILogger::Ptr logger, const std::string& json) {

    Ptr node = Create(logger);
    node->_impl = std::make_shared<ImportedJsonNodeImpl>(logger, Json::parse(json));

    return node;
}

JsonNode::Ptr
JsonNode::Create(Logging::ILogger::Ptr logger) {
    return std::shared_ptr<JsonNode>(new JsonNode(logger));
}

JsonNode::Ptr
JsonNode::Create(std::shared_ptr<JsonNodeImpl> ptr) {
    auto node = std::shared_ptr<JsonNode>(new JsonNode(nullptr));
    node->_impl = ptr;
    return node;
}
