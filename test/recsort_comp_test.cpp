#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include "gtest/gtest.h"
#include <climits>
#include "src/include/storage/vectorStore.h"
#include "src/include/osort/recorba.h"
#include "src/include/osort/recsort.h"
#include "omp.h"

std::vector<int> randomVec(size_t len){
  std::vector<int> vec;
  vec.reserve(len);
  for (size_t i = 0; i < len; i++){
    vec.push_back(i);
  }

  std::random_device rd;
  std::mt19937 g(rd());
 
  std::shuffle(vec.begin(), vec.end(), g);
  
  return vec;
}

void recBitonicIter(size_t numElems) {
  std::vector<int> vec = randomVec(numElems);
  libUtil::Sorting<int>::RecBitonicSort(vec,0,numElems,true);
  
  for (size_t i = 0; i < vec.size() - 1; i++)
  {
    EXPECT_LE(vec[i], vec[i+1]);
  }
}

void recORBAiter(size_t beta, size_t Z) {
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

  double c = 3;
  auto recstorage = libStorage::VectorBucketStore<libUtil::Padded<int>>(16, numElems*c/16);
  auto recsort = libOSort::RecSort<int>(elems, recstorage, c);
  recsort.Sort(2);
  
  int prev = INT_MIN;
  for (size_t eid = 0; eid < numElems; eid++) {
    int num;
    elems.ReadElement(eid, num);
    EXPECT_LE(prev, num);
    prev = num;
  }
  printf("\n");
}

TEST(SortTest, TestRedBitonicComp){
  size_t beta = 64; //number of buckets
  size_t Z = 256; //bucket size
  
  recBitonicIter(beta * Z);
}

TEST(SortTest, TestRecORBAComp){
  size_t beta = 64; //number of buckets
  size_t Z = 256; //bucket size
  
  recORBAiter(beta, Z);
}
