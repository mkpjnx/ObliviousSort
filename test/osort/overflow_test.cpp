#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include "gtest/gtest.h"
#include "src/include/storage/vectorStore.h"
#include "src/include/osort/recorba.h"
#include "src/include/util/common.h"

/**
 * @brief Count overflow rate given the parameters
 * 
 * @param numElems number of elements, must be power of 2
 * @param Z bin size, must be power of 2
 * @param runs number of trials to run
 * @return number of times we overflowed 
 */
size_t testBucket(size_t numElems, size_t Z, size_t runs){
  size_t beta = 2*numElems/Z; //number of buckets
  
  auto elems = libStorage::VectorElementStore<int>(numElems);
  auto storage = libStorage::VectorBucketStore<libUtil::Labeled<int>>(beta, Z);

  std::vector<int> localStorage;
  localStorage.resize(Z);
  for (size_t eid = 0; eid < numElems; eid++) {
    int num = eid;
    elems.WriteElement(eid, num);
  }

  auto orba = libOSort::RecORBA<int>(elems, storage);

  size_t count = 0;
  for(size_t i = 0; i < runs; i++) {
    try{
      orba.Shuffle(2);
    } catch(libUtil::binOverflowException &e) {
      count++;
    }
  }
  return count;
}

/**
 * @brief Measure overflow rate with a "small" number of elements
 * 
 */
TEST(OverflowTest, TestOverflow){
  size_t numElems = 1024 * 8;
  size_t maxZ = 2048;
  size_t runs = 100;
  for(size_t Z = 2; Z <= maxZ; Z *= 2) {
    size_t count = testBucket(numElems, Z, runs);
    printf("Z=%ld \t overflow rate: %ld/%ld\n", Z, count, runs);
  }
}