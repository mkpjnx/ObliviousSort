#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <climits>
#include <omp.h>
#include <chrono>
#include <iostream>

void worker(std::vector<int> &vec, size_t itr, size_t offset, size_t N) {
  std::mt19937 g(omp_get_thread_num());
  for(size_t i = 0; i < itr; i ++) {
    for(size_t j = 0; j < N; j ++){
      vec[j + offset] = g();
    }
  }
}


//Returns the time taken to run Rec-ORBA using the following specifications
double timeWorkers(size_t itr, size_t total, size_t numThreads, size_t task_size){
  std::vector<int> vec;
  vec.resize(total);

  #pragma omp parallel for
  for (size_t i = 0; i < total; i++) {
    vec[i] = (int)i;
  }

  omp_set_num_threads(numThreads);
  auto parastart = std::chrono::steady_clock::now();
  #pragma omp parallel for
  for (size_t i = 0; i < total - task_size; i+= task_size) {
    worker(vec, itr, i, task_size);
  }
  
  auto paraend = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = paraend-parastart;
  return elapsed_seconds.count();
}


double timeSerial(size_t itr, size_t total){
  std::vector<int> vec;
  vec.resize(total);

  for (size_t i = 0; i < total; i++) {
    vec[i] = (int)i;
  }

  auto parastart = std::chrono::steady_clock::now();
  worker(vec, itr, 0, total);
  auto paraend = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = paraend-parastart;
  return elapsed_seconds.count();
}

void problemScaling(){
  size_t minN = 1UL << 10;
  size_t maxN = 1UL << 20;
  size_t itr = 100;

  int procs = omp_get_num_procs();
  std::cout << "N,serial,single,parallel\n";
  for (size_t N = minN; N <= maxN; N *= 2) {
    double serial = timeSerial(itr, N);
    double single = timeWorkers(itr, N, 1, N);
    double parallel = timeWorkers(itr, N, procs, N / procs);
    std::cout << N << "," << serial << "," << single << "," << parallel << "\n";
  }


}

void granScaling(){
  size_t N = 1UL << 20;
  size_t min_task_size = 1UL << 6;
  int procs = omp_get_num_procs();
  size_t max_task_size = N/procs;
  size_t itr = 10;

  std::cout << "taskSize,serial,single,parallel\n";
  for (size_t tsize = min_task_size; tsize <= max_task_size; tsize *= 2) {
    double serial = timeSerial(itr, N);
    double parallel = timeWorkers(itr, N, procs, tsize);
    std::cout << tsize << "," << serial << "," << parallel << "\n";
  }

}

int main(int argc, char **argv) {
  problemScaling();
  granScaling();
  return 0;
}
