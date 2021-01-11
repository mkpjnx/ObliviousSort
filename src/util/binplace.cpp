#include "src/include/util/sorting.h"
#include "src/include/util/binplace.h"

namespace libUtil {

template <typename T>
void BinAssign(std::vector<BinItem<T>> &vec, size_t beta, size_t Z){
  vec.reserve(vec.size() + beta * Z);
  for(size_t b = 0; b < beta; b++){
    for (size_t ind = 0; ind < Z; ind++)
    {
      vec.push_back(BinItem<T>(T(), b, ItemType::FILLER));
    }
  }
  Sorting<BinItem<T>>::OddEvenMergeSort(vec,0,vec.size());

  //map to 1's and zero's for scan
  vec[0].offset_ = 0;
  for(size_t i = 1; i < vec.size(); i++){
    vec[i].offset_ = vec[i].Group == vec[i-1].Group ? 1 : 0;
  }

  //segmented scan forward and mark excess
  //TODO: check if normal is excessive
  //TODO: make parallel
  for(size_t i = 0; i < vec.size(); i++){
    vec[i].offset_ = vec[i].offset_ == 0 ? 0 : vec[i-1].offset_ + 1;
    vec[i].tag_ = vec[i].offset_ < Z ? ItemTag::NORMAL : ItemTag::EXCESS;
  }

  //prune out excess
  Sorting<BinItem<T>>::OddEvenMergeSort(vec,0,vec.size());
  vec.resize(beta*Z);

}

template class BinItem<int>;
template class BinItem<Labeled<int>>;
template void BinAssign<int>(std::vector<BinItem<int>> &vec, size_t beta, size_t Z);
}