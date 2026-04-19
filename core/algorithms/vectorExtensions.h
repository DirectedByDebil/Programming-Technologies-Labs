#ifndef VECTOR_EXTENSIONS_H
#define VECTOR_EXTENSIONS_H

#include <vector>

namespace extensions
{
    template <typename T>
    class VectorExtensions
    {
    public:

        enum class SortCase {Best, Worst, Average, Random};

        VectorExtensions() = delete;

        static std::vector<T> generateVector (const int size, const SortCase sortCase = SortCase::Random);

        static std::vector<T> sortBubble (std::vector<T>);
        static std::vector<T> sortInsert (std::vector<T>);
        static std::vector<T> sortShell (std::vector<T>);
        static std::vector<T> sortQuickRecursive (std::vector<T>);
        static std::vector<T> sortQuickIterative (std::vector<T>);

        //! Radix sort works correctly only with integer types (int, unsigned, etc.)
        //! For floating-point types, behavior is undefined.
        static std::vector<T> sortRadixMSB (std::vector<T>);
        static std::vector<T> sortRadixLSB (std::vector<T>);
    
    private:
        
    };
}


#endif // !VECTOR_EXTENSIONS_H
