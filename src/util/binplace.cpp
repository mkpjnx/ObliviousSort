#include "src/include/util/sorting.h"
#include "src/include/util/binplace.h"

namespace libUtil {

template <typename T>
void Item<T>::BinAssign(std::vector<Item<T>> &vec, size_t beta, size_t Z){
  vec.reserve(vec.size() + beta * Z);
  for(size_t b = 0; b < beta; b++){
    for (size_t ind = 0; ind < Z; ind++)
    {
      vec.push_back(Item(T(), b, ItemType::FILLER));
    }
  }
  Sorting<Item<T>>::OddEvenMergeSort(vec,0,vec.size());

  //map to 1's and zero's for scan
  vec[0].offset_ = 0;
  for(size_t i = 1; i < vec.size(); i++){
    vec[i].offset_ = vec[i].Group == vec[i-1].Group ? 1 : 0;
  }

  //scan forward and mark excess
  //TODO: make parallel
  for(size_t i = 0; i < vec.size(); i++){
    vec[i].offset_ = vec[i].offset_ == 0 ? 0 : vec[i-1].offset_ + 1;
    vec[i].tag_ = vec[i].offset_ < Z ? ItemTag::NORMAL : ItemTag::EXCESS;
  }

  //prune out excess
  Sorting<Item<T>>::OddEvenMergeSort(vec,0,vec.size());
  vec.resize(beta*Z);

}

template class Item<int>;
}