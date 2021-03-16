#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <climits>
#include <omp.h>
#include <chrono>
#include "src/include/storage/vectorStore.h"
#include "src/include/osort/recorba.h"
#include "src/include/osort/quicksort.h"
#include "src/include/util/common.h"

size_t ntestScale = 2;
size_t ntestMin = 1000;
size_t ntestMax = 10000000;
size_t numTrials = 3;

double timeOrba(size_t N, size_t numThreads, size_t bucketSize, size_t gamma){
  auto elems = libStorage::VectorElementStore<int>(N);
  size_t numBuckets = N / (bucketSize / 2) + 1;
  numBuckets = 1UL << (size_t)(std::ceil(std::log2(numBuckets)));
  auto storage = libStorage::VectorBucketStore<libUtil::Labeled<int>>(numBuckets, bucketSize);

  omp_set_num_threads(omp_get_max_threads());
  #pragma omp parallel for shared(elems) schedule(static)
  for (size_t eid = 0; eid < elems.Size; eid++) {
    int num = eid;
    elems.WriteElement(eid, num);
  }

  auto orba = libOSort::RecORBA<int>(elems, storage);
  //randomElems(elems);

  omp_set_num_threads(numThreads);
  auto parastart = std::chrono::steady_clock::now();
  #pragma omp parallel
  {
    #pragma omp single
    orba.Shuffle(gamma);

    #pragma omp single
    libOSort::ElemQuicksort<int>::Sort(elems, 0, N);
  }
  auto paraend = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = paraend-parastart;
  return elapsed_seconds.count();
}

void numScaling(size_t numThreads, size_t bucketSize, size_t gamma){
  printf("N, time(s)\n");
  for(size_t N = ntestMin; N < ntestMax; N*=2){
    double t = 0;
    for(size_t trial = 0; trial < numTrials; trial++){
      t += timeOrba(N, numThreads, bucketSize, 4);
    }
    printf("N= %lu, t = %f\n", N, t / numTrials);
  }
}

void thrScaling(size_t N, size_t bucketSize, size_t gamma){
  printf("#threads, time(s)\n");
  for(size_t numThreads = 1; numThreads <= omp_get_max_threads(); numThreads++){
    double t = 0;
    for(size_t trial = 0; trial < numTrials; trial++){
      t += timeOrba(N, numThreads, bucketSize, 4);
    }
    printf("%lu, %f\n", N, t / numTrials);
  }
}


void randomElems(libStorage::VectorElementStore<int> &elems){
  std::random_device rd;
  auto seed = rd();
  std::mt19937 g;
  g.seed(seed);

  #pragma omp parallel for shared(elems) schedule(static)
  for (size_t eid = 0; eid < elems.Size; eid++) {
    int num = g();
    elems.WriteElement(eid, num);
  }
}


int main(int argc, char **argv) {
  numScaling(8, 512, 4);
  thrScaling(1000000, 512, 4);
  return 0;
}
