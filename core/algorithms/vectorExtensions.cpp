#include <vector>

#include "vectorExtensions.h"
#include "../utils/math.h"


namespace extensions
{

    template <typename T>
    std::vector<T> VectorExtensions<T>::generateVector (const int size, const VectorExtensions<T>::SortCase sortCase)
    {
        std::vector<T> vector(size);

        //InitRandom();
        //double test = RandomDouble(0, 100);


        return vector;
    }

    template <typename T>
    std::vector<T> VectorExtensions<T>::sortBubble (std::vector<T> vector)
    {
        return vector;
    }

    template <typename T>    
    std::vector<T> VectorExtensions<T>::sortInsert (std::vector<T> vector)
    {
        return vector;
    }
    
    template <typename T>
    std::vector<T> VectorExtensions<T>::sortShell (std::vector<T> vector)
    {
        return vector;
    }

    template <typename T>
    std::vector<T> VectorExtensions<T>::sortQuickRecursive (std::vector<T> vector)
    {
        return vector;
    }
    
    template <typename T>
    std::vector<T> VectorExtensions<T>::sortQuickIterative (std::vector<T> vector)
    {
        return vector;
    }
    
    //! Radix sort works correctly only with integer types (int, unsigned, etc.)
    //! For floating-point types, behavior is undefined.
    template <typename T>
    std::vector<T> VectorExtensions<T>::sortRadixMSB (std::vector<T> vector)
    {
        return vector;
    }
    
    template <typename T>
    std::vector<T> VectorExtensions<T>::sortRadixLSB (std::vector<T> vector)
    {
        return vector;
    }        

    template class VectorExtensions<int>;
    template class VectorExtensions<double>;
}
