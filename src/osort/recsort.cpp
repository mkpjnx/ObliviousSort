#include "src/include/osort/recsort.h"
#include "src/include/util/sorting.h"

#include <random>
#include <cassert>

namespace libOSort
{

  template <typename T>
  RecSort<T>::RecSort(libStorage::ElementStorage<T> &data,
    libStorage::BucketStorage<Padded<T>> &buckets, double overflowC)
   : data_(data), buckets_(buckets), overflowC_(overflowC) {}
  
  template <typename T>
  void RecSort<T>::generatePivots(){
    std::random_device rd;
    std::mt19937_64 generator(rd());
    double rate = 1.0 / std::log2(data_.Size);
    std::bernoulli_distribution b(rate);
    //sample from data
    std::vector<Padded<T>> sample;
    for(elem_id_t eid = 0; eid < data_.Size; eid++){
      if(b(generator)) {
        Padded<T> dst;
        dst.real = true;
        data_.ReadElement(eid, dst.Elem);
        sample.push_back(dst);
      }
    }
    printf("sampled %lu elements\n", sample.size());

    //pad and sort
    size_t paddedSize = padToPow2(sample.size());
    assert(paddedSize >= sample.size());
    sample.resize(paddedSize); //default false items
    libUtil::Sorting<Padded<T>>::BitonicSort(sample,0,sample.size());

    //pushback every log^2(n) elems
    //TODO edge cases: what if n = 1?
    size_t interval = static_cast<size_t>(std::pow(std::log2(sample.size()), 2));
    for(size_t ind = 0; ind < sample.size(); ind += interval){
      pivots_.push_back(sample[ind]);
    }
    printf("chose %lu pivots\n", pivots_.size());

    //pad again to pow2
    size_t pivot_size = padToPow2(pivots_.size()+1);
    pivots_.resize(pivot_size - 1); //default false items
    printf("padded to %lu\n", pivots_.size());

  }


  template <typename T>
  void RecSort<T>::loadData() {
    //calculate bucket size (C * n / b)
    size_t numBuckets = pivots_.size() + 1;
    size_t bucketSize = std::ceil((overflowC_ * data_.Size) / numBuckets);
    size_t elemPerBucket = std::ceil(((double) data_.Size) / numBuckets);
    printf("bucketstore requirements: numBuckets: %lu, size: %lu\n", numBuckets, bucketSize);
    //size requirement for buckets
    assert(numBuckets <= buckets_.NumBuckets && bucketSize <= buckets_.BucketSize);
    
    //for each bucket, load n/b elems and pad the rest
    std::vector<Padded<T>> localStorage;
    localStorage.resize(buckets_.BucketSize);
    elem_id_t currentElem = 0;
    for(bucket_id_t bid = 0; bid < buckets_.NumBuckets; bid++) {
      size_t bucketCounter = 0;
      for(auto &itr : localStorage){
        itr.real = currentElem < data_.Size && bucketCounter++ < elemPerBucket;
        if (itr.real) {
          data_.ReadElement(currentElem++, itr.Elem);
        }
      }
      buckets_.WriteBucket(bid, localStorage);
    }
  }

  template <typename T>
  void RecSort<T>::saveData(std::vector<bucket_id_t> result) {
    //retrieve all real elems in bucket order specified by result
    std::vector<Padded<T>> local;
    local.resize(this->buckets_.BucketSize);

    size_t eid = 0;
    for(auto &itr : result) {
      this->buckets_.ReadBucket(itr, local);
      for(auto &item : local){
        if(item.real) {
          this->data_.WriteElement(eid++, item.Elem);
        }
      }
    }
  }

  template <typename T>
  bool RecSort<T>::Sort(size_t gamma) {
    //generate pivots
    generatePivots();
    //initialize bucket store
    loadData();
    std::vector<bucket_id_t> allBuckets;
    for(bucket_id_t bid = 0; bid <= pivots_.size(); bid++){
      allBuckets.push_back(bid);
    }
    auto result = sortRecursive(allBuckets, pivots_, gamma);
    //sort based on pivots
    saveData(result);
    return true;
  }

  template <typename T>
  void RecSort<T>::sortBase(std::vector<bucket_id_t>& buckets, std::vector<Padded<T>>& pivots) {
    assert(buckets.size() == pivots.size()+1);
    //load buckets into local storage
    std::vector<Padded<T>> localStorage;
    localStorage.resize(buckets.size() * buckets_.BucketSize);
    localStorage.resize(padToPow2(localStorage.size()));
    
    buckets_.ReadBuckets(buckets, localStorage);

    //sort
    assert(padToPow2(localStorage.size()) == localStorage.size());
    libUtil::Sorting<Padded<T>>::BitonicSort(localStorage,0,localStorage.size());

    //load into newbuckets and pad
    size_t currentInd = 0;
    std::vector<Padded<T>> newBucket;

    for(size_t bid = 0; bid <= pivots.size(); bid++ ){
      newBucket.clear();

      while(true){
        if(currentInd >= localStorage.size() || !localStorage[currentInd].real){
          //no more elems left to put in buckets
          break;
        }
        if(newBucket.size() == buckets_.BucketSize){
          //TODO: bin overflow throw exception? return false?
          break;
        }
        //current item is greater than pivot
        if(pivots[bid] < localStorage[currentInd]) {
          break;
        }
        newBucket.push_back(localStorage[currentInd]);
        currentInd++;
      }

      newBucket.resize(buckets_.BucketSize);
      buckets_.WriteBucket(buckets[bid], newBucket);
    }
  }

  template <typename T>
  std::vector<bucket_id_t> RecSort<T>::sortRecursive(std::vector<bucket_id_t>& buckets,
    std::vector<Padded<T>>& pivots, size_t gamma) {
    assert(buckets.size() == pivots.size()+1);
    assert(padToPow2(buckets.size()) == buckets.size());
    
    //base case
    if(buckets.size() <= gamma){
      sortBase(buckets, pivots);
      return buckets;
    }

    //calculate betas: beta1 rows, beta2 cols
    size_t beta1 = static_cast<size_t>(std::ceil(std::log2(std::sqrt(buckets.size()))));
    assert(beta1 < 64);
    beta1 = 1UL << beta1;
    size_t beta2 = buckets.size() / beta1;


    //first pass
    std::vector<bucket_id_t> firstPass;
    std::vector<Padded<T>> firstPivots;
    //generate pivot' of length beta2-1
    for (size_t pivotInd = beta1 - 1; pivotInd < pivots.size(); pivotInd += beta1) {
      firstPivots.push_back(pivots[pivotInd]);
    }
    assert(firstPivots.size() == beta2 - 1);

    //recursively sort each row with pivot'
    for (size_t b1 = 0; b1 < beta1; b1 ++) {
      auto begin = buckets.begin() + b1 * beta2;
      auto end = begin + beta2;
      auto toSort = std::vector<bucket_id_t>(begin, end);
      auto result = sortRecursive(toSort, firstPivots, gamma);
      firstPass.insert(firstPass.end(), result.begin(), result.end());
    }


    //second pass
    std::vector<bucket_id_t> secondPass;
    
    for (size_t b2 = 0; b2 < beta2; b2 ++) {
      std::vector<bucket_id_t> toSort;
      
      //each column sorts with corresponding beta1-1 pivots
      auto begin = pivots.begin() + b2 * beta1;
      auto end = begin + beta1 - 1;
      auto secondPivots = std::vector<Padded<T>>(begin, end);

      for (size_t b1 = 0; b1 < beta1; b1 ++) {
        toSort.push_back(firstPass[b1 * beta2 + b2]);
      }
      auto result = sortRecursive(toSort, secondPivots, gamma);
      secondPass.insert(secondPass.end(), result.begin(), result.end());
    }
    //concatenate resulting bucket vec
    return secondPass;
  }

  template class RecSort<int>;
} // namespace libORBA
