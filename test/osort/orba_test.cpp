#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include "gtest/gtest.h"
#include "src/include/storage/vectorStore.h"
#include "src/include/osort/recorba.h"
#include "src/include/util/common.h"

TEST(OrbaTest, TestORBA){
  size_t beta = 8; //number of buckets
  size_t Z = 64; //bucket size
  size_t numElems = beta * Z / 2;
  
  auto elems = libStorage::VectorElementStore<int>(numElems);
  auto storage = libStorage::VectorBucketStore<libUtil::Labeled<int>>(beta, Z);

  std::vector<int> localStorage;
  localStorage.resize(Z);
  for (size_t eid = 0; eid < numElems; eid++) {
    int num = eid;
    elems.WriteElement(eid, num);
  }

  auto orba = libOSort::RecORBA<int>(elems, storage);
  orba.Shuffle(2);

  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    printf("%d\t", num);
  }
  printf("\n");

}
