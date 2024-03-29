#ifndef ResUtils_Included
#define ResUtils_Included
/* ResUtils.h
 *
 */
#include <unistd.h>
#include <vector>
#include <string>
#include <utility>
#include "Corrupter.h"

#define CORRUPT_ELEMS 0

class ResUtils {
    public:
        ResUtils(size_t delta);

        std::vector<size_t> ResilientMerge(std::vector<size_t> X, std::vector<size_t> Y);

        std::vector<size_t> ResilientSort(std::vector<size_t> I);

        void testAllSort();
        void testAllMerge();

        std::vector<std::pair<std::string, size_t>> benchAllMerge(size_t size_A, size_t size_B, size_t delta);
        std::vector<std::pair<std::string, size_t>> benchAllSort(size_t arr_size, size_t delta);

    private:

        typedef std::vector<size_t> (ResUtils::*MergeFunction)(std::vector<size_t>, std::vector<size_t>);

        typedef std::vector<size_t> (ResUtils::*SortFunction)(std::vector<size_t>);

        Corrupter corrupter;
        size_t delta;

        // Internal sort and merge variants
        std::vector<size_t> NaiveSort(std::vector<size_t> F);
        std::vector<size_t> NaiveMerge(std::vector<size_t> A, std::vector<size_t> B);
        std::pair<std::vector<size_t>, std::vector<size_t>> PurifyingMerge(std::vector<size_t> X, std::vector<size_t> Y);
        std::vector<size_t> UnbalancedMerge(std::vector<size_t> A, std::vector<size_t> B);

        // Generic utilities
        std::vector<size_t> mergeSort(std::vector<size_t> A, MergeFunction mergeFunction);

        // Testing methods and utilities
        void testAllMerge(size_t delta, std::vector<size_t> A, std::vector<size_t> B);
        void testAllSort(size_t delta, std::vector<size_t> A);

        void testUnbalancedMerge();
        void testPurifyingMerge();
        void testNaiveSort();

        std::pair<std::string, size_t> benchRefMerge(std::vector<size_t> A, std::vector<size_t> B);
        std::pair<std::string, size_t> benchNaiveMerge(std::vector<size_t> A, std::vector<size_t> B);
        std::pair<std::string, size_t> benchUnbalancedMerge(std::vector<size_t> A, std::vector<size_t> B);
        std::pair<std::string, size_t> benchPurifyingMerge(std::vector<size_t> A, std::vector<size_t> B);
        std::pair<std::string, size_t> benchResilientMerge(std::vector<size_t> A, std::vector<size_t> B);


        std::pair<std::string, size_t> benchRefSort(std::vector<size_t> A);
        std::pair<std::string, size_t> benchNaiveSort(std::vector<size_t> A);
        std::pair<std::string, size_t> benchResilientSort(std::vector<size_t> A);

        std::vector<size_t> genRandomVector(size_t vec_size, size_t max_elem);

};
#endif