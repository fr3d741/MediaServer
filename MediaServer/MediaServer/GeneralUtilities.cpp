#include <GeneralUtilities.h>

#include <algorithm>

void
GeneralUtilities::RightTrim(std::string& str) {

    for (int i = str.size() - 1; 0 <= i; --i)
        if (str[i] == ' ')
            str.erase(i);
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
