#include "src/include/util/common.h"
#include "src/include/storage/elementStorage.h"
#include <vector>

namespace libOSort
{
  using libStorage::elem_id_t;

  template <typename T>
  class ElemQuicksort {
    public:
      static void Sort(libStorage::ElementStorage<T> &elems,
      elem_id_t begin, elem_id_t end){
        if(end - begin <= 1){
          return;
        }
        elem_id_t pivotInd = partition(elems, begin, end);
        Sort(elems, begin, pivotInd);
        Sort(elems, pivotInd+1, end);
      }
    private:
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
