#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <climits>
#include "gtest/gtest.h"
#include "src/include/storage/vectorStore.h"
#include "src/include/osort/recorba.h"
#include "src/include/osort/recsort.h"
#include "src/include/util/common.h"

TEST(RecSortTest, TestRecSort){
  size_t beta = 512; //number of buckets
  size_t Z = 32; //bucket size
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
  orba.Shuffle(4);

  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    printf("%d\t", num);
  }
  printf("\n");

  double c = 3;
  auto recstorage = libStorage::VectorBucketStore<libUtil::Padded<int>>(16, numElems*c/16);
  auto recsort = libOSort::RecSort<int>(elems, recstorage, c);
  recsort.Sort(2);
  
  int prev = INT_MIN;
  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    printf("%d\t", num);
    elems.ReadElement(eid, num);
    EXPECT_LE(prev, num);
    prev = num;
  }
  printf("\n");
  
}
