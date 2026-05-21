#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include "fileUtils.h"

void file_utils::writeOutput (const file_utils::OutputData& data)
{
    if (!std::filesystem::exists(data.directory)) {
        std::filesystem::create_directory(data.directory);
    }

    std::filesystem::path filePath = std::filesystem::path(data.directory) / data.fileName;

    std::ofstream file(filePath);
    
    if (file.is_open()) {
        file << data.output;
        
        file.close(); 
    } else {
        std::cerr << "Unable to open file";
    }
}