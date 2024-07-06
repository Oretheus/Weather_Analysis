#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

CSVReader::CSVReader(const std::string& filePath) : filePath(filePath) {}

std::vector<std::vector<std::string>> CSVReader::getData() {
    std::ifstream file(filePath);
    std::vector<std::vector<std::string>> dataList;
    std::string line = "";
    
    while (getline(file, line)) {
        std::vector<std::string> vec;
        std::stringstream sstream(line);
        std::string word;
        while (getline(sstream, word, ',')) {
            vec.push_back(word);
        }
        dataList.push_back(vec);
    }
    
    file.close();
    return dataList;
}
