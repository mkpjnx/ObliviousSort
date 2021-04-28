#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <omp.h>
#include "gtest/gtest.h"
#include "src/include/util/common.h"
 
void printMatrix(std::vector<int> M, size_t h){
  size_t w = M.size()/h;
  for(size_t i = 0; i < h; i++){
    for(size_t j = 0; j < w; j++){
      std::cout << M[i*w + j] << ",\t";
    }
    std::cout << "\n";
  }
}

TEST(BinplaceTest, TestBinplaceBasic){
  size_t h = 300;
  size_t w = 300;
  std::vector<int> M;
  M.resize(h*w);

  #pragma omp parallel for
  for(size_t i = 0; i < h*w; i++){
    M[i] = i;
  }
  std::vector<int> N;
  N.resize(M.size());
  libUtil::recTranspose<int>(M, h,w,0,N,0,0,h,w);

  #pragma omp parallel for
  for(size_t i = 0; i < h*w; i++){
    size_t r = i/w;
    size_t c = i%w;
    EXPECT_EQ(M[r*w + c],N[c*h + r]);
  }
}

