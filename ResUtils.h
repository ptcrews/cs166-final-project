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
        void testUnbalancedMerge();

    private:
        size_t delta;

        std::pair<std::vector<size_t>, std::vector<size_t>> PurifyingMerge(std::vector<size_t> X, std::vector<size_t> Y);
        std::vector<size_t> NaiveSort(std::vector<size_t> F);
        std::vector<size_t> UnbalancedMerge(std::vector<size_t> A, std::vector<size_t> B);
        std::pair<std::vector<size_t>, std::vector<size_t>>
            genRandomVectors(size_t size_A, size_t size_B,
                                       size_t max_elem);
};
#endif