#include <Utility/StreamWriter.h>

#include <filesystem>

void 
StreamWriter::Write(const std::string& path, const std::string& content) {

    std::ofstream output;
    output.open(path, std::ios::ios_base::out);
    output << content << std::endl;
    output.close();
}

void 
StreamWriter::Write(const string& path, const string& content) {

    file_stream output;
    output.open(path, std::ios::ios_base::out);
    output << content << std::endl;
    output.close();

}

void 
StreamWriter::Write(const std::string& path, const string& content) {

    file_stream output;
    output.open(path, std::ios::ios_base::out);
    output << content << std::endl;
    output.close();
}
