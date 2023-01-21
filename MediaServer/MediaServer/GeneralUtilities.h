#pragma once

#include <string>

class GeneralUtilities
{
public:
    static void RightTrim(std::wstring& str);
    static void RightTrim(std::string& str);
    static void LeftTrim(std::wstring& str);
    static void LeftTrim(std::string& str);
    static bool AreEqualCaseInsensitive(std::string str1, std::string str2);
    static bool AreEqualCaseInsensitive(std::wstring str1, std::wstring str2);
    static std::wstring Convert(const std::string& str);
    static std::string Convert(const std::wstring& str);
    static bool Contains(const std::wstring& in, const std::wstring& what);
};

