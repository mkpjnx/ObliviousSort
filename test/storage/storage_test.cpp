#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include "gtest/gtest.h"
#include "src/include/storage/vectorStore.h"

TEST(StorageTest, TestVectorStoreSingleBucket){
  size_t beta = 8; //number of buckets
  size_t Z = 4; //bucket size
  
  auto storage = libStorage::VectorBucketStore<int>(beta, Z);
  
  std::vector<int> localStorage;
  localStorage.resize(Z);
  for (size_t bid = 0; bid < beta; bid++) {
    for (size_t ind = 0; ind < Z; ind ++) {
      localStorage[ind] = bid;
    }
    storage.WriteBucket(bid, localStorage);
  }

  for (size_t bid = 0; bid < beta; bid++) {
    storage.ReadBucket(bid, localStorage);
    for (size_t ind = 0; ind < Z; ind ++) {
      EXPECT_EQ(localStorage[ind],bid);
    }
  }

}
