#include <vector>
#include <functional>
#include "src/include/util/common.h"


namespace libUtil {
#define SORT_TEMPLATE_ARGS template <typename T, typename Comp>
#define SORT_TYPE Sorting<T, Comp>

template <typename T, typename Comp = std::less<T>>
class Sorting {
  public:
    static void BitonicSort(std::vector<T> &vec, size_t begin, size_t end);
    static void RecBitonicSort(std::vector<T> &vec, size_t begin, size_t end, bool flag);
    static void OddEvenMergeSort(std::vector<T> &vec, size_t begin, size_t end);

};

}