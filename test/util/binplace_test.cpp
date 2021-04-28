#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <chrono>
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
  // number of buckets
  size_t beta = 1024;
  // bin size
  size_t Z = 1024;
  size_t n = beta * Z;
  std::vector<int> vec = randomVec(n);
  std::vector<libUtil::Labeled<int>> itemized;
  itemized.reserve(n);
  
  // wrap the int in a Labeled<int> object
  for(auto &itr : vec){
    auto item = libUtil::Labeled<int>();
    item.Elem = itr;
    // In this case, let label be the element itself
    item.Label = itr;
    item.Group = item.Label % beta;
    item.Type = libUtil::ItemType::NORMAL;
    itemized.push_back(item);
  }

  // timing the bin assignment algorithm
  // this is sorting a 2-million element array twice
  auto start = std::chrono::steady_clock::now();
  libUtil::BinAssign<int>(itemized, beta, Z);
  auto end = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "Bin Assignment took: " << elapsed_seconds.count() << " secs\n";

  for (size_t i = 0; i < beta * Z; i++) {
    // expect that the item at index belong to the group we expect
    EXPECT_EQ(itemized[i].Group, i / Z );
    // all items should be normal
    EXPECT_EQ(itemized[i].Type, libUtil::ItemType::NORMAL);
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
    item.Label = itr;
    item.Group = item.Label % beta;
    item.Type = libUtil::ItemType::NORMAL;
    itemized.push_back(item);
  }
  EXPECT_THROW({
    libUtil::BinAssign<int>(itemized, beta, Z);
  }, libUtil::binOverflowException );
}
