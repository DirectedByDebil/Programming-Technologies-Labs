#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

namespace file_utils {

    struct OutputData
    {
        std::string fileName;
        std::string directory;
        std::string output;
    };
    
    void writeOutput (const OutputData& data);
};

#endif