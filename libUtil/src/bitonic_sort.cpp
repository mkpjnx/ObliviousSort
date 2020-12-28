#include "libUtil/src/include/bitonic_sort.h"

namespace libUtil {

BITONIC_TEMPLATE_ARGS
void BITONIC_TYPE::Sort(std::vector<T> &vec, size_t begin, size_t end){
  Comp c;
  size_t n = end - begin;

  for (size_t k = 2; k <= n; k *= 2) {// bitonic sort beginning at pairwise
    for (size_t j = k/2; j > 0; j /= 2) {// bitonic merge
      for (size_t i = 0; i < n; i++) { // can be parallelized
        size_t l = i ^ j;
        if (i < l) {

          T elemI = T(vec[i + begin]);
          T elemL = T(vec[l + begin]);
          bool isLess = c(elemI, elemL);
          bool normal = i & k; //determines if it's reversed

          if (normal == isLess)
          {
            vec[i + begin] = elemL;
            vec[l + begin] = elemI;
          } else {
            vec[i + begin] = elemI;
            vec[l + begin] = elemL;
          }
        }
      }
    }
  }
}

template class BitonicSort<int>;
}
