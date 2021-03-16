#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <climits>
#include <omp.h>
#include <chrono>
#include <iostream>
#include "src/include/storage/vectorStore.h"
#include "src/include/osort/recorba.h"
#include "src/include/osort/quicksort.h"
#include "src/include/util/common.h"

bool testNum = false;
bool testThr = false;

size_t ttestN = 10000;
size_t ttestTrials = 3;

size_t ntestScale = 2;
size_t ntestMin = 1000;
size_t ntestMax = 100000;
size_t ntestTrials = 3;

size_t bucketSize = 512;

//Returns the time taken to run Rec-ORBA using the following specifications
double timeOrba(size_t N, size_t numThreads, size_t bucketSize){
  auto elems = libStorage::VectorElementStore<int>(N);
  size_t paddedN = 1UL << (size_t)(std::ceil(std::log2(N)));

  size_t numBuckets = paddedN / (bucketSize / 2);
  numBuckets = numBuckets ? numBuckets : 1;
  auto storage = libStorage::VectorBucketStore<libUtil::Labeled<int>>(numBuckets, bucketSize);

  // set gamma = log_2(n), padded to pow2
  size_t gamma = (size_t)(std::ceil(std::log2(N)));
  gamma =  1UL << (size_t)(std::ceil(std::log2(gamma)));
  omp_set_num_threads(omp_get_num_procs());
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

void numScaling(){
  size_t numThreads = (size_t)omp_get_num_procs();
  std::cout << "**********************\n"
            << "**  N Scaling test  **\n"
            << "**********************\n"
            << "T = " << numThreads << "\n"
            << "Running " << ntestTrials << " trials each\n\n" 
            << "N, time(s)\n";
  for(size_t N = ntestMin; N < ntestMax; N*=2){
    double t = 0;
    for(size_t trial = 0; trial < ntestTrials; trial++){
      t += timeOrba(N, numThreads, bucketSize);
    }
    std::cout << N << "," <<  t / ntestTrials << "\n";
  }
  std::cout << "\nDone!\n";
}

void thrScaling(){
  std::cout << "**********************\n"
            << "** Thr Scaling test **\n"
            << "**********************\n"
            << "N = " << ttestN << "\n"
            << "Running " << ttestTrials << " trials each\n\n" 
            << "#threads, time(s)\n";
  for(int numThreads = 1; numThreads <= omp_get_num_procs(); numThreads++){
    double t = 0;
    for(size_t trial = 0; trial < ttestTrials; trial++){
      t += timeOrba(ttestN, (size_t) numThreads, bucketSize);
    }
    std::cout << numThreads << "," <<  t / ttestTrials << "\n";
  }
  std::cout << "\nDone!\n";
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

void printUsage(char **argv){
  std::cout << "Usage: " << argv[0]
            << " [-n min max trials] [-t n trials] [-b bucket size]\n";
  std::cout << "\t-n\tTest scaling with respect to array size. Doubling each time\n"
            << "\t-t\tTest scaling with respect to number of threads\n";
  exit(0);
}

int main(int argc, char **argv) {
  if (argc == 1) printUsage(argv);
  for (int i = 1; i < argc; i++){
    if (std::string(argv[i]) == "-n") {
      testNum = true;
      if(i+3 >= argc) printUsage(argv);

      ntestMin = atoi(argv[++i]);
      ntestMax = atoi(argv[++i]);
      ntestTrials = atoi(argv[++i]);
      if(!ntestMin || ntestMax < ntestMin || !ntestTrials) printUsage(argv);
    } else if (std::string(argv[i]) == "-t") {
      testThr = true;
      if(i+2 >= argc) printUsage(argv);
      ttestN = atoi(argv[++i]);
      ttestTrials = atoi(argv[++i]);
      if(!ttestTrials || !ttestN) printUsage(argv);
    } else {
      printUsage(argv);
    }
  }

  if (testNum) numScaling();
  if (testThr) thrScaling();
  return 0;
}
