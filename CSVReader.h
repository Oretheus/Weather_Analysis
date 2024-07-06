#pragma once
#include <vector>
#include <string>

class CSVReader {
public:
    CSVReader(const std::string& filePath);
    std::vector<std::vector<std::string>> getData();

private:
    std::string filePath;
};
