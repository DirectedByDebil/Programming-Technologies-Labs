#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>
#include <tuple>
#include <string>
#include <filesystem>
#include <functional>

#include "laba.h"
#include "../algorithms/vectorExtensions.h"

const int MAX_VECTOR_SIZE = 100000;

template <typename T>
using SortFunc = std::function<std::tuple<std::vector<T>, double>()>;


std::vector<int> getSizes (const int maxOrder);
std::tuple<int, std::string, std::string> getInputParams (int argc, char* argv[]);

template<typename T>
void benchmarkSorting(const int size,
    const std::string sortingTitle, const std::string sortCase,
    const SortFunc<T> sortFunc,
    std::ostream& sizeStream, std::ostream& stringStream);

void writeResults (std::string directory, std::string filename, std::string results);

int cpp_labs::startLab2 (int argc, char* argv[])
{
    
    auto[maxOrder, csvFileName, directory] = getInputParams(argc, argv);

    if (maxOrder <= 0 || csvFileName.empty())
    {
        std::cerr << "Max Order must be positive!\nMust include output file name!\n";
        return -1;
    }

    using T = double;
    using vec = extensions::VectorExtensions;

    using so = vec::Sortings;
    const std::vector<so> sortings = 
    {
        so::Bubble,
        so::Insert,
        so::Shell,
        so::QuickRecursive,
        so::QuickIterative
    };

    const int radixBits[] = {1,2,4,8};

    std::ostringstream stringStream;
    //stringStream << std::fixed << std::setprecision(10);
    stringStream << std::fixed;
    stringStream <<"size,algorithm,case,time\n";

    std::vector<int> sizes = getSizes(maxOrder);
    for (const int size: sizes)
    {
        std::cout << "Counting for size: " << size << "...\n";
        std::ostringstream sizeStream;
        //sizeStream << std::fixed << std::setprecision(10);
        sizeStream << std::fixed;
        sizeStream << "Size: " << size << "\n";

        std::vector<T> best = vec::generateVector<T>(size, vec::SortCase::Best);         
        std::vector<T> worst = vec::generateVector<T>(size, vec::SortCase::Worst);         
        std::vector<T> average = vec::generateVector<T>(size, vec::SortCase::Average);         
        
        sizeStream << "Best:\n";
        vec::putToStream<T>(sizeStream, best);
        sizeStream << "\n";

        sizeStream << "Worst:\n";
        vec::putToStream<T>(sizeStream, worst);
        sizeStream << "\n";

        sizeStream << "Average:\n";
        vec::putToStream<T>(sizeStream, average);
        sizeStream << "\n=====================================\n";
        
        for (const so sorting: sortings)
        {        
            sizeStream << vec::toString(sorting)<< ":\n";

            std::string sortingTitle = vec::toString(sorting);
        
            SortFunc<T> bestFunc = std::bind(&vec::sort<T>, best, sorting, 8);
            SortFunc<T> worstFunc = std::bind(&vec::sort<T>, worst, sorting, 8);
            SortFunc<T> averageFunc = std::bind(&vec::sort<T>, average, sorting, 8);
        
            benchmarkSorting(size, sortingTitle, "best", bestFunc, sizeStream, stringStream);
            benchmarkSorting(size, sortingTitle, "worst", worstFunc, sizeStream, stringStream);
            benchmarkSorting(size, sortingTitle, "average", averageFunc, sizeStream, stringStream);
            
            sizeStream << "\n";
        }

        
        for (const int bits: radixBits)
        {
            so sorting = so::RadixMSD;
            std::string sortingTitle = vec::toString(sorting) + "_" + std::to_string(bits);
            sizeStream << sortingTitle << ":\n";

            
            SortFunc<T> bestFunc = std::bind(&vec::sort<T>, best, sorting, bits);
            SortFunc<T> worstFunc = std::bind(&vec::sort<T>, worst, sorting, bits);
            SortFunc<T> averageFunc = std::bind(&vec::sort<T>, average, sorting, bits);
            
            benchmarkSorting(size, sortingTitle, "best", bestFunc, sizeStream, stringStream);
            benchmarkSorting(size, sortingTitle, "worst", worstFunc, sizeStream, stringStream);
            benchmarkSorting(size, sortingTitle, "average", averageFunc, sizeStream, stringStream);
            
            sizeStream << "\n";
            
            
            sorting = so::RadixLSD;
            sortingTitle = vec::toString(sorting) + "_" + std::to_string(bits);
            sizeStream << sortingTitle << ":\n";

            bestFunc = std::bind(&vec::sort<T>, best, sorting, bits);
            worstFunc = std::bind(&vec::sort<T>, worst, sorting, bits);
            averageFunc = std::bind(&vec::sort<T>, average, sorting, bits);
        
            benchmarkSorting(size, sortingTitle, "best", bestFunc, sizeStream, stringStream);
            benchmarkSorting(size, sortingTitle, "worst", worstFunc, sizeStream, stringStream);
            benchmarkSorting(size, sortingTitle, "average", averageFunc, sizeStream, stringStream);
            
            sizeStream << "\n";
        }

        std::cout << "Writing results...\n\n";
        writeResults(directory, "alg_" + std::to_string(size) +".txt", sizeStream.str());
    }

    std::string csvResults = stringStream.str();
    writeResults(directory, csvFileName, csvResults);

    return 0;
}


std::vector<int> getSizes (const int maxOrder)
{

    std::vector<int> sizes;

    if (maxOrder <= 0)
    {
        std::cout << "Max Order must be positive!\n";
        return sizes;
    }

    sizes.reserve(maxOrder * 2);

    int steps[] = {10, 50};
    int length = sizeof(steps) / sizeof(steps[0]); 

    for (int i = 0; i < maxOrder; i++)
    {
        for (int j = 0; j < length; j++)
        {
            int step = steps[j];
            
            if (step > MAX_VECTOR_SIZE)
            {
                std::cout << "Max vector size: " << MAX_VECTOR_SIZE << "\n";
                return sizes;
            }

            sizes.push_back(step);
            steps[j] *= 10;
        }
    }

    return sizes;
}

std::tuple<int, std::string, std::string> getInputParams(int argc, char* argv[])
{
    int maxOrder = 1;
    std::string filename = "laba2_results.csv"; 
    std::string directory = "results"; 
    
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        
        if (arg == "-mo" && i + 1 < argc)
        {
            maxOrder = std::atoi(argv[i + 1]);
            i++;
        }
        else if (arg == "-f" && i + 1 < argc)
        {
            filename = argv[i + 1];
            i++;
        }
        else if (arg == "-d" && i + 1 < argc)
        {
            directory = argv[i + 1];
            i++;
        }
    }
    
    std::cout << "Max order: " << maxOrder << "\n";
    std::cout << "Output file: " << filename << "\n";
    std::cout << "Directory: " << directory << "\n";
    
    return {maxOrder, filename, directory};
}

template<typename T>
void benchmarkSorting(const int size,
    const std::string sortingTitle, const std::string sortCase,
    const SortFunc<T> sortFunc,
    std::ostream& sizeStream, std::ostream& stringStream)
{
    using vec = extensions::VectorExtensions;

    auto[sorted, time] = sortFunc();
    stringStream << size << "," << sortingTitle << "," << sortCase << "," << time << "\n";
    sizeStream << "Time: " << time << "\n";
    vec::putToStream<T>(sizeStream, sorted);
    sizeStream << "\n";
}


void writeResults (std::string directory, std::string filename, std::string results)
{
    if (!std::filesystem::exists(directory)) {
        std::filesystem::create_directory(directory);
    }

    std::filesystem::path filePath = std::filesystem::path(directory) / filename;

    std::ofstream file(filePath);
    
    if (file.is_open()) {
        file << results;
        
        file.close(); 
    } else {
        std::cerr << "Unable to open file";
    }
}