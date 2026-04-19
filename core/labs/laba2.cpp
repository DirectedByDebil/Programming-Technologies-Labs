#include <iostream>
#include <vector>

#include "laba.h"
#include "../algorithms/vectorExtensions.h"

int cpp_labs::startLab2 (int argc, char* argv[])
{
    
    //todo input params: sortCasesOutput-filename, O(n)-filename

    using vec = extensions::VectorExtensions<int>;
    std::vector<int> vector = vec::generateVector(10);
    


    std::cout << "c++ test " << std::endl;


    

    //todo results:
        //? table: algorithm, case, size, time
        //? table: algorithm, o(n)

    //todo write results to csv


    return 0;
}