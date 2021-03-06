#include "src/include/util/common.h"
#include "src/include/storage/elementStorage.h"
#include <vector>
#include <omp.h>

#define QSORT_GRANULARITY 4096

namespace libOSort
{
  using libStorage::elem_id_t;

  template <typename T>
  class ElemQuicksort {
    public:
      static void Sort(libStorage::ElementStorage<T> &elems,
      elem_id_t begin, elem_id_t end){
        #pragma omp parallel
        {
          #pragma omp single
          sort_help(elems, begin, end);
        }
      }
    private:
      static void sort_help(libStorage::ElementStorage<T> &elems,
      elem_id_t begin, elem_id_t end){
        if(end - begin <= 1){
          return;
        }
        elem_id_t pivotInd = partition(elems, begin, end);
        if (end - begin > QSORT_GRANULARITY) {
          #pragma omp task shared(elems)
          sort_help(elems, begin, pivotInd);
          #pragma omp task shared(elems)
          sort_help(elems, pivotInd+1, end);
          #pragma omp taskwait
        } else {
          sort_help(elems, begin, pivotInd);
          sort_help(elems, pivotInd+1, end);
        }

      }
      static elem_id_t partition(libStorage::ElementStorage<T> &elems,
      elem_id_t begin, elem_id_t end) {
        T pivot, current, toSwap;
        elems.ReadElement(end - 1, pivot);
        elem_id_t nextLow = begin;
        for (elem_id_t curInd = begin; curInd < end-1; curInd++){
          elems.ReadElement(curInd, current);
          if (current < pivot) {
            elems.ReadElement(nextLow, toSwap);
            elems.WriteElement(nextLow, current);
            elems.WriteElement(curInd, toSwap);
            nextLow++;
          }
        }
        elems.ReadElement(nextLow, toSwap);
        elems.WriteElement(nextLow, pivot);
        elems.WriteElement(end - 1, toSwap);
        return nextLow;
      }
  };
} // namespace libOSort
