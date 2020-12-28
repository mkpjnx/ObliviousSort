#include <vector>
#include "gtest/gtest.h"
#include "libUtil/src/include/bitonic_sort.h"

TEST(libUtilTest, TestBitonicBasic){
  std::vector<int> vec {7,6,5,4,3,2,1,0};
  libUtil::BitonicSort<int>::Sort(vec,0,8);
  for (size_t i = 0; i < vec.size() - 1; i++)
  {
    EXPECT_LE(vec[i], vec[i+1]);
  }
  
}