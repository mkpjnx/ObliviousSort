#include "src/include/util/sorting.h"

#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include "gtest/gtest.h"
 
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

TEST(SortTest, TestBitonicBasic){
  std::vector<int> vec = randomVec(128);
  libUtil::Sorting<int>::BitonicSort(vec,0,128);
  for (size_t i = 0; i < vec.size() - 1; i++)
  {
    EXPECT_LE(vec[i], vec[i+1]);
  }
  
}

TEST(SortTest, TestRecBitonicBasic){
  std::vector<int> vec = randomVec(128);
  libUtil::Sorting<int>::RecBitonicSort(vec,0,128,true);

  /*
  for (size_t i = 0; i < vec.size(); i++) {
    printf("%d\n", vec[i]);
  }
  */
 
  for (size_t i = 0; i < vec.size() - 1; i++)
  {
    EXPECT_LE(vec[i], vec[i+1]);
  }
  
}

TEST(SortTest, TestOddEvenBasic){
  std::vector<int> vec = randomVec(100);
  libUtil::Sorting<int>::OddEvenMergeSort(vec,0,100);
  for (size_t i = 0; i < vec.size() - 1; i++)
  {
    EXPECT_LE(vec[i], vec[i+1]);
  }
  
}