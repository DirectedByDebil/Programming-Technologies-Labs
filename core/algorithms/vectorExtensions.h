#ifndef VECTOR_EXTENSIONS_H
#define VECTOR_EXTENSIONS_H

#include <vector>
#include <tuple>
#include <fstream>

namespace extensions
{

    class VectorExtensions
    {
    public:

        enum class SortCase {Best, Worst, Average, Random};

        enum class Sortings
        {
            Bubble,
            Insert,
            Shell,
            QuickRecursive,
            QuickIterative,
            RadixMSD,
            RadixLSD
        };

        VectorExtensions() = delete;

        template <typename T>
        static std::vector<T> generateVector (const int size, const SortCase sortCase = SortCase::Random);

        template <typename T>
        static void printVector(const std::vector<T>&);

        static std::string toString(const Sortings sorting);

        template <typename T>
        static std::ostream& putToStream(std::ostream& os, const std::vector<T>& v);

        template <typename T>
        static std::tuple<std::vector<T>, double> sort (const std::vector<T>&, const Sortings sorting = Sortings::QuickRecursive);

        template <typename T>
        static std::vector<T> sortBubble (std::vector<T>);

        template <typename T>
        static std::vector<T> sortInsert (std::vector<T>);

        template <typename T>
        static std::vector<T> sortShell (std::vector<T>);

        template <typename T>
        static std::vector<T> sortQuickRecursive (std::vector<T>);

        template <typename T>
        static std::vector<T> sortQuickIterative (std::vector<T>);
        
        //! Radix sort works correctly only with integer types (int, unsigned, etc.)
        //! For floating-point types, behavior is undefined.
        template <typename T>
        static std::vector<T> sortRadixMSD (std::vector<T>);

        template <typename T>
        static std::vector<T> sortRadixLSD (std::vector<T>);
        
    private:        
        template <typename T, typename Y>
        static std::vector<Y> toType(const std::vector<T>&);
    };
}


#endif // !VECTOR_EXTENSIONS_H
