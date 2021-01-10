#include <vector>
#include <functional>

namespace libUtil {
#define SORT_TEMPLATE_ARGS template <typename T, typename Comp>
#define SORT_TYPE Sorting<T, Comp>

template <typename T, typename Comp = std::less<T>>
class Sorting {
  public:
    static void BitonicSort(std::vector<T> &vec, size_t begin, size_t end);
    static void OddEvenMergeSort(std::vector<T> &vec, size_t begin, size_t end);

};

}