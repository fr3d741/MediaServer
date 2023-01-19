#pragma once

#include <string>

class GeneralUtilities
{
public:
    static void RightTrim(std::string& str);
    static void LeftTrim(std::string& str);
    static bool AreEqualCaseInsensitive(std::string str1, std::string str2);
};

