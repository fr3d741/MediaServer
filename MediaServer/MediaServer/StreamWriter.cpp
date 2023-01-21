#include <StreamWriter.h>

#include <fstream>

void 
StreamWriter::Write(const std::string& path, const std::string& content) {

    std::ofstream output;
    output.open(path, std::ios::ios_base::out);
    output << content << std::endl;
    output.close();
}

void 
StreamWriter::Write(const std::wstring& path, const std::wstring& content) {

    std::wofstream output;
    output.open(path, std::ios::ios_base::out);
    output << content << std::endl;
    output.close();

}

void 
StreamWriter::Write(const std::string& path, const std::wstring& content) {

    std::wofstream output;
    output.open(path, std::ios::ios_base::out);
    output << content << std::endl;
    output.close();
}
