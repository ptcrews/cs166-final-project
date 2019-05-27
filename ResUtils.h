#ifndef ResUtils_Included
#define ResUtils_Included
/* ResUtils.h
 *
 */
#include <unistd.h>
#include <vector>

class ResUtils {
    public:
        ResUtils(size_t delta);
        void testUnbalancedMerge();

    private:
        size_t delta;

        std::vector<size_t> UnbalancedMerge(std::vector<size_t> A, std::vector<size_t> B);
};
#endif