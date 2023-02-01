#pragma once

#include "Macros.h"
#include <CommonDefines.h>

class DLL GeneralUtilities
{
public:
    static void RightTrim(string& str);
    static void RightTrim(std::string& str);
    static void LeftTrim(string& str);
    static void LeftTrim(std::string& str);
    static bool AreEqualCaseInsensitive(std::string str1, std::string str2);
    static bool AreEqualCaseInsensitive(string str1, string str2);
    static string Convert(const std::string& str);
    static std::string Convert(const string& str);
    static bool Contains(const string& in, const string& what);
    static int ConvertToYear(const string& str);
    static int ConvertToYear(const std::string& str);
    static int HammingDistance(const string& str1, const string& str2);
    static string ConvertLatinToRomanNumbers(const string& str1);
    static bool HasNumber(const string& str);
};

