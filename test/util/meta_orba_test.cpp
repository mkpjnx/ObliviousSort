#include "src/include/util/sorting.h"
#include "src/include/util/meta_orba.h"
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include "gtest/gtest.h"
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


TEST(SortTest, TestMetaOrba){
  std::vector<int> vec1 = randomVec(128);
  std::vector<std::vector<int>> out = libUtil::meta_orba(vec1,16,16);
  
  for (size_t i = 0; i < 16; i++) {
    for (size_t j = 0; j < 16; j++) {
      printf("%d ", out[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}