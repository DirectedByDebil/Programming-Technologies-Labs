#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <utility>
#include <functional>
#include <chrono>
#include <algorithm>
#include <type_traits>

#include "vectorExtensions.h"
#include "../utils/mathUtils.h"

namespace extensions
{

    const int MAX_VECTOR_PRINT_SIZE = 20;

    template <typename T>
    std::vector<T> VectorExtensions::generateVector (const int size, const VectorExtensions::SortCase sortCase)
    {
        std::vector<T> vector;
        vector.reserve(size);

        switch (sortCase)
        {
            case VectorExtensions::SortCase::Best:
                for (int i = 0; i < size; i++)
                {
                    vector.push_back((T)i);
                }
            break;
            
            case VectorExtensions::SortCase::Worst:
                for (int i = size; i > 0; i--)
                {
                    vector.push_back((T)i);
                }
            break;

            case VectorExtensions::SortCase::Average:
                for (int i = 0; i < size; i++)
                {
                    T num = i % 2 == 0 ? 
                    i / 2:
                    size - i / 2 - 1;
                    vector.push_back(num);
                }
            break;


        default: // random
            for (int i = 0; i < size; i++)
            {
                T num = math_utils::randomNumber<T>(0, 100);   
                vector.push_back(num);
            }
            break;
        }
                
        return vector;
    }
    
    template <typename T>
    void VectorExtensions::printVector (const std::vector<T>& vector)
    {   
        if (vector.size() > MAX_VECTOR_PRINT_SIZE)
        {
            std::cout << "Vector size: " << vector.size() << "\n";
            return;
        }

        for (const auto& item : vector)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    std::string VectorExtensions::toString(const Sortings sorting)
    {
        switch (sorting)
        {
            case Sortings::Bubble:
                return "Bubble";
            case Sortings::Insert:
                return "Insert";
            case Sortings::Shell:
                return "Shell";
            case Sortings::QuickRecursive:
                return "QuickRecursive";
            case Sortings::QuickIterative:
                return "QuickIterative";
            case Sortings::RadixMSD:
                return "RadixMSD";
            case Sortings::RadixLSD:
                return "RadixLSD";
            default:
                return "";
        }
    }

    template <typename T>
    std::ostream& VectorExtensions::putToStream(std::ostream& os, const std::vector<T>& v)
    {
        for (const auto& item : v)
            os << item << " ";
        return os;
    }


    template <typename T>
    std::tuple<std::vector<T>, double> VectorExtensions::sort (const std::vector<T>& orig, const typename VectorExtensions::Sortings sorting)
    {
        std::vector<T> vec(orig);
        std::vector<int> vec_int = VectorExtensions::toType<T,int>(vec);
        std::vector<int> vec_radix(vec_int);

        using so = VectorExtensions::Sortings;

        double time = 0;

        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        std::chrono::time_point<std::chrono::high_resolution_clock> end;

        switch(sorting)
        {
            case so::Bubble:
                start = std::chrono::high_resolution_clock::now();
                vec = VectorExtensions::sortBubble(vec);
                end = std::chrono::high_resolution_clock::now();
                break;
                
            case so::Insert:
                start = std::chrono::high_resolution_clock::now();
                vec = VectorExtensions::sortInsert(vec);
                end = std::chrono::high_resolution_clock::now();
                break;
                
            case so::Shell:
                start = std::chrono::high_resolution_clock::now();
                vec = VectorExtensions::sortShell(vec);
                end = std::chrono::high_resolution_clock::now();
                break;
                
            case so::QuickRecursive:
                start = std::chrono::high_resolution_clock::now();
                vec = VectorExtensions::sortQuickRecursive(vec);
                end = std::chrono::high_resolution_clock::now();
                break;
                
            case so::QuickIterative:
                start = std::chrono::high_resolution_clock::now();
                vec = VectorExtensions::sortQuickIterative(vec);
                end = std::chrono::high_resolution_clock::now();
                break;
                
            case so::RadixMSD:
                start = std::chrono::high_resolution_clock::now();
                vec_radix = VectorExtensions::sortRadixMSD(vec_int);
                end = std::chrono::high_resolution_clock::now();
                
                vec = VectorExtensions::toType<int,T>(vec_radix);
                break;
                
            case so::RadixLSD:
                start = std::chrono::high_resolution_clock::now();
                vec_radix = VectorExtensions::sortRadixLSD(vec_int);
                end = std::chrono::high_resolution_clock::now();
                
                vec = VectorExtensions::toType<int,T>(vec_radix);
                break;

            default:
                break;
        }

        //double seconds = std::chrono::duration<double>(end - start).count();
        double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();
        time = milliseconds;

        return {vec, time};
    }

    template <typename T>
    std::vector<T> VectorExtensions::sortBubble (std::vector<T> vector)
    {
        if (vector.empty()) return vector;
        int length = (int)vector.size();

        for (int i = 0; i < length; i++)
        {
            for (int j = 0; j < length-i-1; j++)
            {
                if (vector[j] > vector[j+1])
                {
                    std::swap(vector[j], vector[j+1]);
                }
            }
        }

        return vector;
    }

    template <typename T>    
    std::vector<T> VectorExtensions::sortInsert (std::vector<T> vector)
    {
        if (vector.empty()) return vector;
        int length = (int)vector.size();

        for (int i = 0; i < length; i++)
        {
            for (int j = i; j > 0 && vector[j-1] > vector[j]; j--)
            {
                std::swap(vector[j-1], vector[j]);
            }
        }

        return vector;
    }
    
    template <typename T>
    std::vector<T> VectorExtensions::sortShell (std::vector<T> vector)
    {
        if (vector.empty()) return vector;
        int length = (int)vector.size();

        for (int s = length / 2; s > 0; s /= 2) {
            for (int i = s; i < length; i++) {
                for (int j = i - s; j >= 0 && vector[j] > vector[j + s]; j -= s) {
                    std::swap(vector[j], vector[j+s]);
                }
            }
        }

        return vector;
    }

    template <typename T>
    std::vector<T> VectorExtensions::sortQuickRecursive (std::vector<T> vector)
    {
        if (vector.empty()) return vector;
        int length = (int)vector.size();

        std::function<void(int, int)> quickSort = [&](int left, int right) {
            if (left >= right) return;
            
            int mid = left + (right - left) / 2;
            if (vector[left] > vector[mid]) std::swap(vector[left], vector[mid]);
            if (vector[left] > vector[right]) std::swap(vector[left], vector[right]);
            if (vector[mid] > vector[right]) std::swap(vector[mid], vector[right]);
            std::swap(vector[mid], vector[right]);
            
            T pivot = vector[right];
            int i = left;
            
            for (int j = left; j < right; j++) {
                if (vector[j] <= pivot) {
                    std::swap(vector[i], vector[j]);
                    i++;
                }
            }
            std::swap(vector[i], vector[right]);
            
            quickSort(left, i - 1);
            quickSort(i + 1, right);
        };
        
        quickSort(0, length - 1);

        return vector;
    }
    
    template <typename T>
    std::vector<T> VectorExtensions::sortQuickIterative (std::vector<T> vector)
    {
        if (vector.empty()) return vector;
        int length = (int)vector.size();

        //stack of array borders indexes
        std::stack<std::pair<int, int>> stack;
        stack.push({0, length - 1});
    
        while (!stack.empty()) {
            auto [left, right] = stack.top();
            stack.pop();
            
            if (left >= right) continue;
            
            int mid = left + (right - left) / 2;
            if (vector[left] > vector[mid]) std::swap(vector[left], vector[mid]);
            if (vector[left] > vector[right]) std::swap(vector[left], vector[right]);
            if (vector[mid] > vector[right]) std::swap(vector[mid], vector[right]);
            std::swap(vector[mid], vector[right]);
            
            T pivot = vector[right];
            int i = left;
            
            for (int j = left; j < right; ++j) {
                if (vector[j] <= pivot) {
                    std::swap(vector[i], vector[j]);
                    i++;
                }
            }
            std::swap(vector[i], vector[right]);
            
            if (i + 1 < right) stack.push({i + 1, right});
            if (left < i - 1) stack.push({left, i - 1});
        }

        return vector;
    }
    
    //! Radix sort works correctly only with integer types (int, unsigned, etc.)
    //! For floating-point types, behavior is undefined.
    template <typename T>
    std::vector<T> VectorExtensions::sortRadixMSD (std::vector<T> vector)
    {
        if (vector.empty()) return vector;
        int length = (int)vector.size();

        constexpr int BITS = 8;
        constexpr int RADIX = 1 << BITS;
        constexpr int MASK = RADIX - 1;
        constexpr int PASSES = sizeof(T);
        
        std::function<void(std::vector<T>&, int, int, int)> msdRadix;
        msdRadix = [&](std::vector<T>& arr, int left, int right, int pass)
        {
            if (left >= right || pass < 0) return;
            
            std::vector<int> count(RADIX, 0);
            std::vector<T> output(right - left + 1);
            
            // count same bytes in radix cell 
            for (int i = left; i <= right; ++i) {
                int byte = (arr[i] >> (pass * BITS)) & MASK;
                count[byte]++;
            }
            
            // prefix sums: index of last element in cell
            for (int i = 1; i < RADIX; ++i)
                count[i] += count[i - 1];
            
            // push sorted cells to output
            for (int i = right; i >= left; --i) {
                int byte = (arr[i] >> (pass * BITS)) & MASK;
                count[byte]--;
                output[count[byte]] = arr[i];
            }
            

            for (int i = left; i <= right; ++i)
                arr[i] = output[i - left];
            
            // Sort subarrays
            int start = left;
            for (int b = 0; b < RADIX; ++b) {
                int end = left + count[b] - 1;
                if (start <= end && pass > 0)
                    msdRadix(arr, start, end, pass - 1);
                start = end + 1;
            }
        };
        
        msdRadix(vector, 0, length - 1, PASSES - 1);

        return vector;
    }
    
    template <typename T>
    std::vector<T> VectorExtensions::sortRadixLSD (std::vector<T> vector)
    {
        if (vector.empty()) return vector;
        int length = (int)vector.size();

        constexpr int BITS = 8;
        constexpr int RADIX = 1 << BITS;     // 256
        constexpr int MASK = RADIX - 1;
        constexpr int PASSES = sizeof(T);    // 4 for int32_t
        
        std::vector<T> output(length);
        std::vector<int> count(RADIX); //count values in cell
    
        for (int pass = 0; pass < PASSES; ++pass)
        {
            std::fill(count.begin(), count.end(), 0);
            
            // count same bytes in radix cell 
            for (T val : vector) {
                int byte = (val >> (pass * BITS)) & MASK;
                count[byte]++;
            }
            
            // prefix sums: index of last element in cell
            for (int i = 1; i < RADIX; ++i)
                count[i] += count[i - 1];
            
            // push sorted cells to output
            for (int i = length - 1; i >= 0; --i) {
                int byte = (vector[i] >> (pass * BITS)) & MASK;
                count[byte]--;
                output[count[byte]] = vector[i];
            }
            
            std::swap(vector, output);
        }

        return vector;
    }

    template <typename T, typename Y>
    std::vector<Y> VectorExtensions::toType(const std::vector<T>& vector)
    {
        std::vector<Y> vec;
        if (vector.empty()) return vec;
        
        vec.reserve(vector.size());
        for (const auto& item: vector)
        {
            vec.push_back((Y)item);
        }

        return vec;
    }

    template std::vector<int> VectorExtensions::generateVector<int>(int, VectorExtensions::SortCase);
    template void VectorExtensions::printVector<int>(const std::vector<int>&);
    template std::tuple<std::vector<int>, double> VectorExtensions::sort<int>(const std::vector<int>&, VectorExtensions::Sortings);
    template std::vector<int> VectorExtensions::sortBubble<int>(std::vector<int>);
    template std::vector<int> VectorExtensions::sortInsert<int>(std::vector<int>);
    template std::vector<int> VectorExtensions::sortShell<int>(std::vector<int>);
    template std::vector<int> VectorExtensions::sortQuickRecursive<int>(std::vector<int>);
    template std::vector<int> VectorExtensions::sortQuickIterative<int>(std::vector<int>);
    template std::vector<int> VectorExtensions::sortRadixMSD<int>(std::vector<int>);
    template std::vector<int> VectorExtensions::sortRadixLSD<int>(std::vector<int>);

    template std::vector<double> VectorExtensions::generateVector<double>(int, VectorExtensions::SortCase);
    template void VectorExtensions::printVector<double>(const std::vector<double>&);
    template std::tuple<std::vector<double>, double> VectorExtensions::sort<double>(const std::vector<double>&, VectorExtensions::Sortings);
    template std::vector<double> VectorExtensions::sortBubble<double>(std::vector<double>);
    template std::vector<double> VectorExtensions::sortInsert<double>(std::vector<double>);
    template std::vector<double> VectorExtensions::sortShell<double>(std::vector<double>);
    template std::vector<double> VectorExtensions::sortQuickRecursive<double>(std::vector<double>);
    template std::vector<double> VectorExtensions::sortQuickIterative<double>(std::vector<double>);

    template void VectorExtensions::printVector<std::string>(const std::vector<std::string>&);

    template std::ostream& VectorExtensions::putToStream(std::ostream& os, const std::vector<double>& v);
    template std::ostream& VectorExtensions::putToStream(std::ostream& os, const std::vector<int>& v);

    template std::vector<int> VectorExtensions::toType<double, int>(const std::vector<double>& vector);
    template std::vector<double> VectorExtensions::toType<int, double>(const std::vector<int>& vector);
}
