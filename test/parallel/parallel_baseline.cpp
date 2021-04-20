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

void parRec(std::vector<int> &vec, size_t itr, size_t offset, size_t N, size_t threshold) {
  if (N <= threshold) {
    worker(vec, itr, offset, N);
    return;
  }
  #pragma omp parallel num_threads(2)
  {
    int num = omp_get_thread_num();
    parRec(vec, itr, offset + num * N / 2, N/2, threshold);
    #pragma omp barrier
  }
}

void taskRec(std::vector<int> &vec, size_t itr, size_t offset, size_t N, size_t threshold) {
  if (N <= threshold) {
    worker(vec, itr, offset, N);
    return;
  }
  #pragma omp task
    taskRec(vec, itr, offset, N/2, threshold);
  #pragma omp task
    taskRec(vec, itr, offset + N / 2, N/2, threshold);
  #pragma omp taskwait
}

double timeParrec(size_t itr, size_t total, size_t numThreads, size_t task_size){
  std::vector<int> vec;
  vec.resize(total);
  omp_set_nested(omp_get_max_active_levels());
  #pragma omp parallel for
  for (size_t i = 0; i < total; i++) {
    vec[i] = (int)i;
  }

  auto parastart = std::chrono::steady_clock::now();
  
  parRec(vec, itr, 0, total, task_size);
  
  auto paraend = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = paraend-parastart;
  return elapsed_seconds.count();
}


double timeTaskrec(size_t itr, size_t total, size_t numThreads, size_t task_size){
  std::vector<int> vec;
  vec.resize(total);
  omp_set_num_threads(numThreads);
  #pragma omp parallel for
  for (size_t i = 0; i < total; i++) {
    vec[i] = (int)i;
  }

  auto parastart = std::chrono::steady_clock::now();
  #pragma omp parallel
  {
    #pragma omp single
    taskRec(vec, itr, 0, total, task_size);
  }
  auto paraend = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = paraend-parastart;
  return elapsed_seconds.count();
}

//Returns the time taken to run Rec-ORBA using the following specifications
double timeParfor(size_t itr, size_t total, size_t numThreads, size_t task_size){
  std::vector<int> vec;
  vec.resize(total);

  #pragma omp parallel for
  for (size_t i = 0; i < total; i++) {
    vec[i] = (int)i;
  }

  omp_set_num_threads(numThreads);
  auto parastart = std::chrono::steady_clock::now();
  #pragma omp parallel for
  for (size_t i = 0; i < total/task_size; i++) {
    worker(vec, itr, i * task_size, task_size);
  }
  
  auto paraend = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = paraend-parastart;
  return elapsed_seconds.count();
}

//Returns the time taken to run Rec-ORBA using the following specifications
double timeTaskloop(size_t itr, size_t total, size_t numThreads, size_t task_size) {
  std::vector<int> vec;
  vec.resize(total);

  #pragma omp parallel for
  for (size_t i = 0; i < total; i++) {
    vec[i] = (int)i;
  }

  omp_set_num_threads(numThreads);
  auto parastart = std::chrono::steady_clock::now();
  #pragma omp parallel
  {
    #pragma omp single
    {
      #pragma omp taskloop
      for (size_t i = 0; i < total/task_size; i++) {
        worker(vec, itr, i, task_size);
      }
    }
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
  size_t maxN = 1UL << 15;
  size_t itr = 100;

  int procs = omp_get_num_procs();
  std::cout << "N,serial,singleParfor,multiParfor,singleTask,multiTask\n";
  for (size_t N = minN; N <= maxN; N *= 2) {
    double serial = timeSerial(itr, N);
    double singleParFor = timeParfor(itr, N, 1, N);
    double multiParfor = timeParfor(itr, N, procs, N / procs);
    double singleTask = timeTaskloop(itr, N, 1, N);
    double multiTask = timeTaskloop(itr, N, procs, N / procs);
    std::cout << N << "," << serial << ","
              << singleParFor << "," << multiParfor << ","
              << singleTask << "," << multiTask << "\n";
  }
  std::cout << "\n";

}

void granScaling(){
  size_t N = 1UL << 20;
  size_t min_task_size = 1UL << 6;
  int procs = omp_get_num_procs();
  size_t max_task_size = N/procs;
  size_t itr = 10;

  std::cout << "taskSize,serial,parfor,taskloop\n";
  for (size_t tsize = min_task_size; tsize <= max_task_size; tsize *= 2) {
    double serial = timeSerial(itr, N);
    double parfor = timeParfor(itr, N, procs, tsize);
    double taskloop = timeTaskloop(itr, N, procs, tsize);
    std::cout << tsize << "," << serial << "," << parfor << "," << taskloop << "\n";
  }
  std::cout << "\n";

}

void recScaling(){
  size_t N = 1UL << 20;
  size_t min_task_size = 1UL << 6;
  int procs = omp_get_num_procs();
  size_t max_task_size = N/procs;
  size_t itr = 10;

  std::cout << "taskSize,serial,parfor,taskloop\n";
  for (size_t tsize = min_task_size; tsize <= max_task_size; tsize *= 2) {
    double serial = timeSerial(itr, N);
    double parrec = timeParrec(itr, N, procs, tsize);
    double taskrec = timeTaskrec(itr, N, procs, tsize);
    std::cout << tsize << "," << serial << "," << parrec << "," << taskrec << "\n";
  }
  std::cout << "\n";
}

int main(int argc, char **argv) {
  problemScaling();
  granScaling();
  recScaling();
  return 0;
}
