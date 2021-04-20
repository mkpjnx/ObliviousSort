#pragma once
#include "src/include/util/common.h"
#include "src/include/util/sorting.h"
#include <vector>
#include <iostream>

namespace libUtil {

template <typename T>
void BinAssign(std::vector<Labeled<T>> &vec, size_t beta, size_t Z){
  // double the bins and add beta fillers per bin
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

  // sort into groups with no tag
  Sorting<Labeled<T>>::BitonicSort(vec,0,vec.size());

  // detect and mark excess elements
  size_t offset = 0;
  for(size_t i = 1; i < vec.size(); i++){
    if(vec[i].Group == vec[i-1].Group){
      offset ++;
    } else {
      offset = 0;
    }

    // A normal item is marked excess: this is a bin overflow error
    if(offset >= Z && vec[i].Type == ItemType::NORMAL){
      throw libUtil::binOverflowException();
    }

    // otherwise, mark this as excess to be pruned out
    if (offset >= Z) {
      vec[i].Type = ItemType::EXCESS;
    }
  }

  //prune out excess
  Sorting<Labeled<T>>::BitonicSort(vec,0,vec.size());
  vec.resize(beta*Z);

}

} // namespace libUtil