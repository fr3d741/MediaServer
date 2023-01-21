#include <GeneralUtilities.h>

#include <algorithm>
#include <locale>

void
GeneralUtilities::RightTrim(std::wstring& str) {

    for (int i = static_cast<int>(str.size()) - 1; 0 <= i; --i)
        if (str[i] == ' ')
            str.erase(i);
        else
            break;
}

void
GeneralUtilities::RightTrim(std::string& str) {

    for (int i = static_cast<int>(str.size()) - 1; 0 <= i; --i)
        if (str[i] == ' ')
            str.erase(i);
        else
            break;
}

void
GeneralUtilities::LeftTrim(std::wstring& str) {

    for (auto it = str.begin(); it != str.end();)
        if (*it == ' ')
            str.erase(it);
        else
            break;
}

void
GeneralUtilities::LeftTrim(std::string& str) {

    for (auto it = str.begin(); it != str.end();)
        if (*it == ' ')
            str.erase(it);
        else
            break;
}

bool
GeneralUtilities::AreEqualCaseInsensitive(std::string str1, std::string str2) {
    return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](char a, char b) { return ::tolower(a) == ::tolower(b); });
}

bool
GeneralUtilities::AreEqualCaseInsensitive(std::wstring str1, std::wstring str2) {
    return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](wchar_t a, wchar_t b) { return ::tolower(a) == ::tolower(b); });
}

std::wstring 
GeneralUtilities::Convert(const std::string& str) {

    std::setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));
    std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> converter;
    return converter.from_bytes(str);
}

std::string 
GeneralUtilities::Convert(const std::wstring& str) {

    std::setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));
    std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> converter;
    return converter.to_bytes(str);
}

bool
GeneralUtilities::Contains(const std::wstring& in, const std::wstring& what) {

    for (auto it = what.begin(), it2 = in.begin(); it2 != in.end(); ++it2) {

        if (it == what.end())
            return true;

        if (*it != *it2)
            it = what.begin();
        else
            ++it;
    }

    return false;
}
