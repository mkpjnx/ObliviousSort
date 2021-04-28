#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "gtest/gtest.h"
#include "src/include/storage/vectorStore.h"
#include "src/include/osort/recorba.h"
#include "src/include/util/common.h"

TEST(OrbaTest, TestORBASmall){
  size_t beta = 8; //number of buckets
  size_t Z = 64; //bucket size
  size_t numElems = beta * Z / 2;
  
  auto counter = std::unordered_map<int, int>();
  auto elems = libStorage::VectorElementStore<int>(numElems);
  auto storage = libStorage::VectorBucketStore<libUtil::Labeled<int>>(beta, Z);

  std::vector<int> localStorage;
  localStorage.resize(Z);
  for (size_t eid = 0; eid < numElems; eid++) {
    int num = eid;
    elems.WriteElement(eid, num);
    counter[eid] = 1;
  }

  auto orba = libOSort::RecORBA<int>(elems, storage);
  orba.Shuffle(2);

  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    std::cout << num << "\t";
    EXPECT_EQ(counter[num], 1);
    counter[num] = 0;
  }
  std::cout << "\n";
}

// test that ORBA sorts n unique elements and does not lose them
TEST(OrbaTest, TestORBALarge){
  size_t beta = 4096; //number of buckets
  size_t Z = 512; //bucket size
  size_t numElems = beta * Z / 2;
  
  auto counter = std::unordered_map<int, int>();
  auto elems = libStorage::VectorElementStore<int>(numElems);
  auto storage = libStorage::VectorBucketStore<libUtil::Labeled<int>>(beta, Z);

  std::vector<int> localStorage;
  localStorage.resize(Z);
  for (size_t eid = 0; eid < numElems; eid++) {
    int num = eid;
    elems.WriteElement(eid, num);
    counter[eid] = 1;
  }

  auto orba = libOSort::RecORBA<int>(elems, storage);
  // time the shuffle
  auto start = std::chrono::steady_clock::now();
  orba.Shuffle(2);
  auto end = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "***\t\tORBA on " << numElems << " elems took: "
            << elapsed_seconds.count() << " secs\n";


  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    EXPECT_EQ(counter[num], 1);
    counter[num] = 0;
  }
}
