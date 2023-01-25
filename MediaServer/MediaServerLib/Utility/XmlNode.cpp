#include <Utility/XmlNode.h>
#include <Utility/GeneralUtilities.h>

#include <exception>
#include <stdexcept>
#include <sstream>

using G = GeneralUtilities;

XmlNode::XmlNode(const std::string& elementName)
: _element(elementName), _is_value_created(false) {
}

XmlNode::XmlNode(const std::string& elementName, const string& value)
: _element(elementName), _value(value), _is_value_created(true){
}

XmlNode&
XmlNode::AddChild(const std::string& element) {

    if (_is_value_created)
        throw std::logic_error(element);

    _children.push_back(XmlNode(element));
    return _children.back();
}

XmlNode&
XmlNode::AddChild(const std::string& element, const string& value) {

    if (_is_value_created)
        throw std::logic_error(element);

    _children.push_back(XmlNode(element, value));
    return _children.back();
}

XmlNode&
XmlNode::AddChild(const std::string& element, const std::string& value) {
    return AddChild(element, G::Convert(value));
}

void
XmlNode::AddAttribute(const std::string& attr, const string& value) {
    _attributes[attr] = value;
}

void
XmlNode::AddAttribute(const std::string& attr, const std::string& value) {
    AddAttribute(attr, G::Convert(value));
}

static string 
tabs(int n) {
    return string(n, U'\t');
}

string
XmlNode::Dump(int tab) {

    string_stream stream;

    stream << tabs(tab) << "<" << G::Convert(_element);
    for (auto&& item : _attributes) {
        stream << " " << G::Convert(item.first) << "=\"" << item.second << "\"";
    }
    stream << ">";

    if (_is_value_created)
        stream << _value << "</" << G::Convert(_element) << ">" << std::endl;
    else {
        stream << std::endl;
        for (auto var : _children)
        {
            stream << var.Dump(tab + 1);
        }

        stream << tabs(tab) << "</" << G::Convert(_element) << ">" << std::endl;
    }

    return stream.str();
}
