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

TEST(QuickSortTest, TestParallel){
  size_t numElems = 1024 * 1024 * 16;
  std::cout << "sorting " << numElems << " elems\n";
  auto elems = libStorage::VectorElementStore<int>(numElems);
  
  std::random_device rd;
  auto seed = rd();
  std::mt19937 g;

  g.seed(seed);
  #pragma omp parallel for shared(elems) schedule(static)
  for (size_t eid = 0; eid < numElems; eid++) {
    int num = g();
    elems.WriteElement(eid, num);
  }

  auto start = std::chrono::steady_clock::now();
  #pragma omp parallel
  { 
    #pragma omp single
    libOSort::ElemQuicksort<int>::Sort(elems, 0, numElems);
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "parallel elapsed time: " << elapsed_seconds.count() << "s\n";
  
  //do it again
  g.seed(seed);
  #pragma omp parallel for shared(elems) schedule(static)
  for (size_t eid = 0; eid < numElems; eid++) {
    int num = g();
    elems.WriteElement(eid, num);
  }

  auto ser_start = std::chrono::steady_clock::now();
  libOSort::ElemQuicksort<int>::Sort(elems, 0, numElems);
  auto ser_end = std::chrono::steady_clock::now();
  std::chrono::duration<double> ser_seconds = ser_end-ser_start;
  std::cout << "serial elapsed time: " << ser_seconds.count() << "s\n";

  int prev = INT_MIN;
  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    EXPECT_LE(prev, num);
    prev = num;
  }
}

TEST(ORBATest, TestParallel){
  size_t beta = 2048; //number of buckets
  size_t Z = 512; //bucket size
  size_t numElems = beta * Z / 2;
  
  auto elems = libStorage::VectorElementStore<int>(numElems);
  auto storage = libStorage::VectorBucketStore<libUtil::Labeled<int>>(beta, Z);
  
  std::vector<int> localStorage;
  localStorage.resize(Z);

  #pragma omp parallel for shared(elems) schedule(static)
  for (size_t eid = 0; eid < numElems; eid++) {
    int num = eid;
    elems.WriteElement(eid, num);
  }

  auto orba = libOSort::RecORBA<int>(elems, storage);

  auto parastart = std::chrono::steady_clock::now();
  #pragma omp parallel
  {
    #pragma omp single
    orba.Shuffle(4);
  }
  auto paraend = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed_seconds = paraend-parastart;
  std::cout << "parallel elapsed time: " << elapsed_seconds.count() << "s\n";

  auto serstart = std::chrono::steady_clock::now();
  orba.Shuffle(4);
  auto serend = std::chrono::steady_clock::now();

  elapsed_seconds = serend-serstart;
  std::cout << "serial elapsed time: " << elapsed_seconds.count() << "s\n";
  
  #pragma omp parallel
  { 
    #pragma omp single
    libOSort::ElemQuicksort<int>::Sort(elems, 0, numElems);
  }

  int prev = -1;
  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    EXPECT_EQ(prev+1, num);
    prev = num;
  }
  
}

