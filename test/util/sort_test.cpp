#include "src/include/util/sorting.h"

#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <unordered_set>
#include "gtest/gtest.h"
#include "omp.h"

/**
 * @brief Generate a shuffled vector containing 0 through len - 1 
 * 
 * @param len 
 * @return shuffled array
 */
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

static size_t factorial(size_t n){
  int acc = 1;
  for(int curr = n; curr; acc *= curr--);
  return acc;
}

/**
 * @brief generate a permutation of initial given an index indicating the
 * particular permutation to generate. Note that the inital vector is no longer
 * usable after this call
 * 
 * @param index used to uniquely identify a given permutation 
 * @param initial the set of vectors to permute
 * @return A vector corresponding to the indexed permutation 
 */
static std::vector<int> get_perm(size_t index, std::vector<int> &initial) {
  // base case
  if (initial.size() == 0) return std::vector<int>(0);

  size_t to_rem = index % initial.size();
  size_t next_index = index / initial.size();

  std::vector<int> newvec{initial[to_rem]};
  initial.erase(initial.begin() + to_rem);
  auto res = get_perm(next_index, initial);
  newvec.insert(newvec.end(), res.begin(), res.end());
  return newvec;
}

/**
 * @brief Test bitonic sort on all 40,320 permutations of 8 elements
 */
TEST(SortTest, TestBitonicExhaustive){
  size_t n = 8;
  size_t nfac = factorial(n);
  for (size_t index = 0; index < nfac; index++) {
    // generate the permutation
    std::vector<int> tmp(n);
    for (size_t i = 0; i < n; i ++) tmp[i] = i;
    auto vec = get_perm(index, tmp);

    // sort it
    libUtil::Sorting<int>::BitonicSort(vec,0,n);
    
    // check that it is sorted AND the initial elems
    int last = -1;
    for (size_t i = 0; i < vec.size(); i++)
    {
      EXPECT_EQ(vec[i], last+1);
      last = vec[i];
    }
  }
}

/**
 * @brief Test sorting a million elements, repeatedly
 */
TEST(SortTest, TestBitonicBasic){
  size_t num_trials = 5;
  size_t n = 1024 * 1024;
  for (size_t trial = 0; trial < num_trials; trial++) {
    std::vector<int> vec = randomVec(n);
    libUtil::Sorting<int>::BitonicSort(vec,0,n);
    int last = -1;
    for (size_t i = 0; i < vec.size(); i++)
    {
      EXPECT_EQ(vec[i], last+1);
      last = vec[i];
    }
  }
}


TEST(SortTest, DISABLED_TestRecBitonicBasic){
  std::vector<int> vec = randomVec(1024 * 128);
  libUtil::Sorting<int>::RecBitonicSort(vec,0,1024 * 128,true);
  
  int last = -1;
  for (size_t i = 0; i < vec.size() - 1; i++)
  {
    EXPECT_EQ(vec[i], last+1);
    last = vec[i];
  }
}


TEST(DISABLED_SortTest, TestOddEvenBasic){
  std::vector<int> vec = randomVec(100);
  libUtil::Sorting<int>::OddEvenMergeSort(vec,0,100);
  int last = -1;
  for (size_t i = 0; i < vec.size(); i++)
  {
    EXPECT_EQ(vec[i], last+1);
    last = vec[i];
  }
  
}