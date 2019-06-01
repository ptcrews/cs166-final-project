#ifndef ResUtils_Included
#define ResUtils_Included
/* ResUtils.h
 *
 */
#include <unistd.h>
#include <vector>
#include <utility>

class ResUtils {
    public:
        ResUtils(size_t delta);

        std::vector<size_t> ResilientMerge(std::vector<size_t> X, std::vector<size_t> Y);

        std::vector<size_t> ResilientSort(std::vector<size_t> I);

        void testAllSort();
        void testAllMerge();

    private:

        typedef std::vector<size_t> (ResUtils::*MergeFunction)(std::vector<size_t>, std::vector<size_t>);

        typedef std::vector<size_t> (ResUtils::*SortFunction)(std::vector<size_t>);

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
        void testUnbalancedMerge();
        void testPurifyingMerge();
        void testNaiveSort();

        std::vector<size_t> genRandomVector(size_t vec_size, size_t max_elem);

};
#endif