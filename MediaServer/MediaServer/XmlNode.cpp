#include <XmlNode.h>

#include <exception>
#include <stdexcept>
#include <sstream>

XmlNode::XmlNode(const std::string& elementName)
: _element(elementName), _is_value_created(false) {
}

XmlNode::XmlNode(const std::string& elementName, const std::string& value)
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
XmlNode::AddChild(const std::string& element, const std::string& value) {

    if (_is_value_created)
        throw std::logic_error(element);

    _children.push_back(XmlNode(element, value));
    return _children.back();
}

void
XmlNode::AddAttribute(const std::string& attr, const std::string& value) {
    _attributes[attr] = value;
}

static std::string 
tabs(int n) {
    return std::string(n, '\t');
}

std::string
XmlNode::Dump(int tab) {

    std::stringstream stream;

    stream << tabs(tab) << "<" << _element;
    for (auto&& item : _attributes) {
        stream << " " << item.first << "=\"" << item.second << "\"";
    }
    stream << ">";

    if (_is_value_created)
        stream << _value << "</" << _element << ">" << std::endl;
    else {
        stream << std::endl;
        for (auto var : _children)
        {
            stream << var.Dump(tab + 1);
        }

        stream << tabs(tab) << "</" << _element << ">" << std::endl;
    }

    return stream.str();
}
