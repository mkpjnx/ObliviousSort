#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include "gtest/gtest.h"
#include "src/include/util/common.h"
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
  size_t beta = 8;
  size_t Z = 4;
  size_t n = beta * Z;
  std::vector<int> vec = randomVec(n);
  std::vector<libUtil::Labeled<int>> itemized;
  itemized.reserve(n);
  
  for(auto &itr : vec){
    auto item = libUtil::Labeled<int>();
    item.Elem = itr;
    item.Group = size_t(itr) % beta;
    item.Type = libUtil::ItemType::NORMAL;
    itemized.push_back(item);
  }

  libUtil::BinAssign<int>(itemized, beta, Z);
  for (size_t i = 0; i < beta * Z; i++)
  {
    printf("index: %ld \t item: %d \t bin: %lld\n", i, itemized[i].Elem, itemized[i].Group);
    EXPECT_EQ(itemized[i].Group, i / Z );
  }
}

// Test that we throw an exception on bin overflow
TEST(BinplaceTest, TestBinplaceOverflow){
  // note: for bitonic sort to work, n + beta * z must be a power of 2
  size_t n = 108;
  size_t beta = 10;
  size_t Z = 2;
  std::vector<int> vec = randomVec(n);
  std::vector<libUtil::Labeled<int>> itemized;
  itemized.reserve(n);
  
  for(auto &itr : vec){
    auto item = libUtil::Labeled<int>();
    item.Elem = itr;
    item.Group = size_t(itr) % beta;
    item.Type = libUtil::ItemType::NORMAL;
    itemized.push_back(item);
  }
  EXPECT_THROW({
    libUtil::BinAssign<int>(itemized, beta, Z);
  }, libUtil::binOverflowException );
}
