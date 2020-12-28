#include <vector>
#include <functional>

namespace libUtil {
#define BITONIC_TEMPLATE_ARGS template <typename T, typename Comp>
#define BITONIC_TYPE BitonicSort<T, Comp>

template <typename T, typename Comp = std::less<T>>
class BitonicSort {
  public:
    static void Sort(std::vector<T> &vec, size_t begin, size_t end);

};

}