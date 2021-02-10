#pragma once
#include "src/include/util/common.h"
#include "src/include/util/sorting.h"
#include <vector>

namespace libUtil {

template <typename T>
void BinAssign(std::vector<Labeled<T>> &vec, size_t beta, size_t Z){
  vec.reserve(vec.size() + beta * Z);
  for(size_t b = 0; b < beta; b++){
    for (size_t ind = 0; ind < Z; ind++)
    {
      auto item = Labeled<T>();
      item.Type = ItemType::FILLER;
      item.Group = b;
      vec.push_back(item);
    }
  }

  //clear out tags
  for(auto& itr : vec) {
    itr.tag_ = ItemTag::NONE;
  }

  Sorting<Labeled<T>>::BitonicSort(vec,0,vec.size());

  //map to 1's and zero's for scan
  vec[0].offset_ = 0;
  for(size_t i = 1; i < vec.size(); i++){
    vec[i].offset_ = 0;
    if(vec[i].Group == vec[i-1].Group){
      vec[i].offset_ = vec[i-1].offset_ + 1;
    }
    vec[i].tag_ = vec[i].offset_ < Z ? ItemTag::NORMAL : ItemTag::EXCESS;
    //A normal item is marked excess: this is a bin overflow error
    if(vec[i].tag_ == ItemTag::EXCESS && vec[i].Type == ItemType::NORMAL){
      throw libUtil::binOverflowException();
    }
  }

  //prune out excess
  Sorting<Labeled<T>>::BitonicSort(vec,0,vec.size());
  vec.resize(beta*Z);

}

} // namespace libUtil