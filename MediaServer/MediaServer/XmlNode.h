#pragma once

#include <map>
#include <string>
#include <list>

class XmlNode
{
    std::map<std::string, std::string> _attributes;
    std::string _element;
    std::string _value;
    std::list<XmlNode> _children;
    bool _is_value_created;
public:
    XmlNode() = default; // To support STL containers
    XmlNode(const std::string& elementName);
    XmlNode(const std::string& elementName, const std::string& value);

    XmlNode& AddChild(const std::string& element);
    XmlNode& AddChild(const std::string& element, const std::string& value);
    void AddAttribute(const std::string& attr, const std::string& value);

    std::string Dump(int tab = 0);
};
