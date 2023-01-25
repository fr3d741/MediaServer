#include <Utility/GeneralUtilities.h>

#include <algorithm>
#include <locale>
#include <regex>
#include <array>
#include <format>
#include <codecvt>

void
GeneralUtilities::RightTrim(string& str) {

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
GeneralUtilities::LeftTrim(string& str) {

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
GeneralUtilities::AreEqualCaseInsensitive(string str1, string str2) {
    return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](wchar_t a, wchar_t b) { return ::tolower(a) == ::tolower(b); });
}

string 
GeneralUtilities::Convert(const std::string& str) {

    //std::setlocale(LC_ALL, "");
    //std::locale::global(std::locale(""));
    //std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> converter;
    //return converter.from_bytes(str);

    std::wstring_convert<std::codecvt_utf8<char_t>, char_t> conv_utf8_utf32;
    string unicode_codepoints = conv_utf8_utf32.from_bytes(str);
    return unicode_codepoints;
}

std::string
GeneralUtilities::Convert(const string& str) {

    //std::setlocale(LC_ALL, "");
    //std::locale::global(std::locale(""));
    //std::vector<char> char_array(str.size() * 2);

    //auto res = std::wcstombs(char_array.data(), str.c_str(), str.size());
    //if (res == static_cast<std::size_t>(-1))
    //    res = 0;
    //return std::string(char_array.data(), res);
    std::wstring_convert<std::codecvt_utf8<char_t>, char_t> conv_utf8_utf32;
    std::string unicode_codepoints = conv_utf8_utf32.to_bytes(str);
    return unicode_codepoints;
}

bool
GeneralUtilities::Contains(const string& in, const string& what) {

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

int 
GeneralUtilities::ConvertToYear(const string& str) {

    const rgx wregex(U"[0-9]{4}");
    rgx_match wmatch;
    if (std::regex_search(str, wmatch, wregex) == false)
    {
        return std::numeric_limits<int>::max();
    }

    return stoi(Convert(wmatch.str()));
}

int 
GeneralUtilities::ConvertToYear(const std::string& str) {

    const std::regex regex("[0-9]{4}");
    std::smatch match;
    if (std::regex_search(str, match, regex) == false)
    {
        return std::numeric_limits<int>::max();
    }

    return stoi(match.str());
}

int
GeneralUtilities::HammingDistance(const string& str1, const string& str2) {

    int dist_counter = 0;
    int i = 0;
    for (; i < str1.size() && i < str2.size(); ++i) {

        if (str1[i] == str2[i])
            continue;
        dist_counter++;
    }

    return dist_counter + static_cast<int>(str1.size() - i) + static_cast<int>(str2.size() - i);
}

string
GeneralUtilities::ConvertLatinToRomanNumbers(const string& str1) {

    constexpr std::array<string_view,21> roman_numbers = {
        U"",
        U"I",
        U"II",
        U"III",
        U"IV",
        U"V",
        U"VI",
        U"VII",
        U"VIII",
        U"IX",
        U"X",
        U"XI",
        U"XII",
        U"XIII",
        U"XIV",
        U"XV",
        U"XVI",
        U"XVII",
        U"XVIII",
        U"XIX",
        U"XX",
    };

    const rgx wregex(U"[0-9]{1,2}");
    rgx_match wmatch;
    if (std::regex_search(str1, wmatch, wregex) == false)
        return str1;

    auto i_value = stoi(Convert(wmatch.str()));
    if (i_value < 1 || 20 < i_value)
        return str1;

    auto prefix = wmatch.prefix().str();
    auto suffix = wmatch.suffix().str();

    string answer;
    answer.append(prefix);
    answer.append(roman_numbers[i_value]);
    answer.append(suffix);

    return answer;
}

bool 
GeneralUtilities::HasNumber(const string& str) {

    const rgx wregex(U"[0-9]");
    rgx_match wmatch;
    return std::regex_search(str, wmatch, wregex);
}
