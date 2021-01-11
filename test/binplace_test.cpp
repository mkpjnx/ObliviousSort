#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include "gtest/gtest.h"
#include "src/include/util/binplace.h"
 
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

TEST(BinplaceTest, TestBinplaceBasic){
  size_t n = 32;
  size_t beta = 8;
  size_t Z = 4;
  std::vector<int> vec = randomVec(n);
  std::vector<libUtil::BinItem<int>> itemized;
  itemized.reserve(n);
  
  for(auto &itr : vec){
    itemized.push_back(libUtil::BinItem<int>(itr, size_t(itr) % beta, libUtil::ItemType::NORMAL));
  }

  libUtil::BinAssign<int>(itemized, beta, Z);
  for (size_t i = 0; i < beta * Z; i++)
  {
    printf("index: %ld \t item: %d \t bin: %ld\n", i, itemized[i].Elem, itemized[i].Group);
    EXPECT_EQ(itemized[i].Group, i / Z );
  }
}

TEST(BinplaceTest, TestBinplaceTrimOverflow){
  size_t n = 50;
  size_t beta = 10;
  size_t Z = 2;
  std::vector<int> vec = randomVec(n);
  std::vector<libUtil::BinItem<int>> itemized;
  itemized.reserve(n);
  
  for(auto &itr : vec){
    itemized.push_back(libUtil::BinItem<int>(itr, size_t(itr) % beta, libUtil::ItemType::NORMAL));
  }

  libUtil::BinAssign<int>(itemized, beta, Z);
  for (size_t i = 0; i < beta * Z; i++)
  {
    printf("index: %ld \t item: %d \t bin: %ld\n", i, itemized[i].Elem, itemized[i].Group);
    EXPECT_EQ(itemized[i].Group, i / Z );
  }
}
