#include "src/include/util/sorting.h"
#include "src/include/util/common.h"
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

namespace libUtil {

SORT_TEMPLATE_ARGS
void cas(std::vector<T> &vec, size_t i, size_t j, Comp &c, bool inverted){
  T tmp = T(vec[i]); //copy construct
  if (inverted == (vec[i] < vec[j]))
  {
    vec[i] = vec[j];
    vec[j] = tmp;
  } else {
    vec[i] = tmp;
    vec[j] = vec[j];
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

/*
 * Recursive Bitonic Sort
 */
int powerTwo(size_t n) { // find smallest larger power of two
  if (n && !(n & (n - 1))) { return n; }

  size_t p = 1;
  while (p < n) { p <<= 1; }

  return p;
}

SORT_TEMPLATE_ARGS
void bitonicMerge(std::vector<T> &vec, Comp &c, size_t begin, size_t end, bool flag) {
  size_t n = end - begin;

  if (n <= 1) {
    // return original array unchanged
  } else if (n == 2) {
    T elemI = T(vec[begin]);
    T elemJ = T(vec[begin + 1]);
    bool isLess = c(elemI, elemJ);

    // switch elements if necessary
    if ((!flag && isLess) || (flag && !isLess)) {
      vec[begin] = elemJ;
      vec[begin + 1] = elemI;
    }
  } else {
    size_t cols = powerTwo(ceil(sqrt(n))); // round sqrt of vector up to nearest power of 2
    size_t rows = n / cols + (n % cols != 0 ? 1 : 0);

    // get smallest element
    T smallestElement = vec[end - 1];
    if (c(vec[begin], vec[end - 1])) {
      smallestElement = vec[begin];
    }

    // create matrix
    std::vector<std::vector<T> > K(rows, std::vector<T>(cols, smallestElement));
    std::vector<std::vector<T> > J(cols, std::vector<T>(rows, smallestElement));

    // fill matrix
    size_t curr = begin;
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        // transposed matrix
        J[j][i] = vec[curr];
        curr++;

        if (curr >= end)
          break;
      }
    }

    // *change to run in parallel
    #pragma omp parallel for
    for (size_t col = 0; col < cols; col++) {
      bitonicMerge(J[col], c, 0, rows, flag);
    }

    // transpose matrix
    #pragma omp parallel for collapse(2)
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        K[i][j] = J[j][i];
      }
    }

    // *change to run in parallel
    #pragma omp parallel for
    for (size_t r = 0; r < rows; r++) {
      bitonicMerge(K[r], c, 0, cols, flag);
    }

    // concatenate into a vector (need to improve)
    size_t num_filler = rows * cols - n;
    std::vector<T> temp (rows * cols, smallestElement);
    curr = 0;
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        temp[curr] = K[i][j];
        curr++;
      }
    }

    // filler should be the smallest values in the vector
    for (size_t i = num_filler; i < temp.size(); i++) {
      vec[i - num_filler + begin] = temp[i];
    }
  }
}

SORT_TEMPLATE_ARGS
void SORT_TYPE::RecBitonicSort(std::vector<T> &vec, size_t begin, size_t end, bool flag){
  Comp c;
  size_t n = end - begin;

  if (n > 1) {
    size_t mid = begin + n / 2;
    
    // Fork
    #pragma omp parallel
    {
      RecBitonicSort(vec, begin, mid, flag);
      RecBitonicSort(vec, mid, end, !flag);
    }
    // Join

    bitonicMerge(vec, c, begin, end, flag);
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
template class Sorting<Padded<int>>;
}
