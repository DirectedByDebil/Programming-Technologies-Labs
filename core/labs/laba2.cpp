#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>
#include <tuple>
#include <string>

#include "laba.h"
#include "../algorithms/vectorExtensions.h"

const int MAX_VECTOR_SIZE = 100000;

std::vector<int> getSizes (const int maxOrder);
std::tuple<int, std::string> getInputParams (int argc, char* argv[]);
void writeResults (std::string filename, std::string results);

int cpp_labs::startLab2 (int argc, char* argv[])
{
    
    auto[maxOrder, csvFileName] = getInputParams(argc, argv);

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
        so::QuickIterative,
        so::RadixMSD,
        so::RadixLSD
    };

    std::ostringstream stringStream;
    //stringStream << std::fixed << std::setprecision(10);
    stringStream << std::fixed;
    stringStream <<"size,algorithm,case,time\n";

    std::vector<int> sizes = getSizes(maxOrder);
    for (const int size: sizes)
    {
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

            auto[sortedBest, bestTime] = vec::sort<T>(best, sorting);
            stringStream << size << "," << vec::toString(sorting) << ",best," << bestTime << "\n";
            sizeStream << "Time: " << bestTime << "\n";
            vec::putToStream<T>(sizeStream, sortedBest);
            sizeStream << "\n";
            
            auto[sortedWorst, worstTime] = vec::sort<T>(worst, sorting);
            stringStream << size << "," << vec::toString(sorting) << ",worst," << worstTime << "\n";
            sizeStream << "Time: " << worstTime << "\n";
            vec::putToStream<T>(sizeStream, sortedWorst);
            sizeStream << "\n";
            
            auto[sortedAverage, averageTime] = vec::sort<T>(average, sorting);
            stringStream << size << "," << vec::toString(sorting) << ",average," << averageTime << "\n";
            sizeStream << "Time: " << averageTime << "\n";
            vec::putToStream<T>(sizeStream, sortedAverage);
            sizeStream << "\n\n";
        }

        writeResults("alg_" + std::to_string(size) +".txt", sizeStream.str());
    }

    std::string csvResults = stringStream.str();
    writeResults(csvFileName, csvResults);

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

std::tuple<int, std::string> getInputParams(int argc, char* argv[])
{
    int maxOrder = 1;
    std::string filename = "laba2_results.csv"; 
    
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
    }
    
    std::cout << "Max order: " << maxOrder << std::endl;
    std::cout << "Output file: " << filename << std::endl;
    
    return {maxOrder, filename};
}

void writeResults (std::string filename, std::string results)
{
    std::ofstream file(filename);

    if (file.is_open()) {
        file << results;
        
        file.close(); 
    } else {
        std::cerr << "Unable to open file";
    }
}