#include "src/include/util/sorting.h"
#include "src/include/util/common.h"

namespace libUtil {

SORT_TEMPLATE_ARGS
void cas(std::vector<T> &vec, size_t i, size_t j, Comp &c, bool inverted){
  T elemI = T(vec[i]); //copy construct
  T elemJ = T(vec[j]);
  bool isLess = c(elemI, elemJ);
  if (inverted == isLess)
  {
    vec[i] = elemJ;
    vec[j] = elemI;
  } else {
    vec[i] = elemI;
    vec[j] = elemJ;
  }
}

SORT_TEMPLATE_ARGS
void SORT_TYPE::BitonicSort(std::vector<T> &vec, size_t begin, size_t end){
  Comp c;
  size_t n = end - begin;

  for (size_t k = 2; k <= n; k *= 2) {// bitonic sort beginning at pairwise
    for (size_t j = k/2; j > 0; j /= 2) {// bitonic merge
      for (size_t i = 0; i < n; i++) { // can be parallelized
        size_t l = i ^ j;
        if (i < l) {
          cas(vec, i+begin, l+begin, c, i&k);
        }
      }
    }
  }
}

SORT_TEMPLATE_ARGS
void SORT_TYPE::OddEvenMergeSort(std::vector<T> &vec, size_t begin, size_t end){
  Comp c;
  size_t n = end - begin;
  for (size_t p = 1; p < n; p *= 2) { //partitions of 2p
    for (size_t k = p; k >= 1; k /= 2){ //partner stride size
      for (size_t j = k % p; j < n - k; j += 2*k) { //simultaneous partners
        for (size_t i = 0; i < k; i++){
          if ((i+j)/(p*2) == (i+j+k)/(p*2) && i + j + k < n) {
            cas(vec, begin+i+j, begin+i+j+k, c, false);
          }
        }
      }
    }
  }

}

template class Sorting<int>;
template class Sorting<Labeled<int>>;
}
