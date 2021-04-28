#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <climits>
#include <omp.h>
#include <chrono>
#include "gtest/gtest.h"
#include "src/include/storage/vectorStore.h"
#include "src/include/osort/recorba.h"
#include "src/include/osort/quicksort.h"
#include "src/include/util/common.h"

/**
 * @brief Tests quicksort via the ElementStore interface is correct
 */
TEST(OSortTest, TestQuickSort){
  size_t beta = 2048; //number of buckets
  size_t Z = 512; //bucket size
  size_t numElems = beta * Z / 2;
  
  auto elems = libStorage::VectorElementStore<int>(numElems);
  auto storage = libStorage::VectorBucketStore<libUtil::Labeled<int>>(beta, Z);

  for (size_t eid = 0; eid < numElems; eid++) {
    int num = eid;
    elems.WriteElement(eid, num);
  }

  auto orba = libOSort::RecORBA<int>(elems, storage);
  orba.Shuffle(beta);
  auto start = std::chrono::steady_clock::now();
  #pragma omp parallel
  {
    #pragma omp single
    libOSort::ElemQuicksort<int>::Sort(elems, 0, numElems);
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "***\t\tQuickSort on " << numElems << " elems took: "
            << elapsed_seconds.count() << " secs\n";

  // check sorted
  int prev = -1;
  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    EXPECT_EQ(prev, num-1);
    prev = num;
  }
  
}

/**
 * @brief Tests oblivious sort (orba + elem sort)
 */
TEST(OSortTest, TestOSort){
  size_t beta = 2048; //number of buckets
  size_t Z = 512; //bucket size
  size_t gamma = 32;
  size_t numElems = beta * Z / 2;
  
  auto elems = libStorage::VectorElementStore<int>(numElems);
  auto storage = libStorage::VectorBucketStore<libUtil::Labeled<int>>(beta, Z);

  for (size_t eid = 0; eid < numElems; eid++) {
    int num = eid;
    elems.WriteElement(eid, num);
  }
  auto orba = libOSort::RecORBA<int>(elems, storage);

  
  auto start = std::chrono::steady_clock::now();
  #pragma omp parallel
  {
    #pragma omp single
    orba.Shuffle(gamma);
    #pragma omp single
    libOSort::ElemQuicksort<int>::Sort(elems, 0, numElems);
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "***\t\tBucket Oblivious Sort on " << numElems << " elems took: "
            << elapsed_seconds.count() << " secs\n";

  // check sorted
  int prev = -1;
  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    EXPECT_EQ(prev, num-1);
    prev = num;
  }
  
}

