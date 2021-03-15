#include "src/include/osort/recorba.h"

namespace libOSort
{
  using libStorage::elem_id_t;
  using libStorage::bucket_id_t;
  
  template <typename T>
  RecORBA<T>::RecORBA(libStorage::ElementStorage<T> &data,
    libStorage::BucketStorage<Labeled<T>> &buckets)
   : ORBA<T>(data, buckets) {}
  
  template <typename T>
  void RecORBA<T>::loadData() {
    std::random_device rd;
    std::mt19937_64 generator(rd());

    //load all elements into buckets with random label
    std::vector<Labeled<T>> local;
    local.resize(this->buckets_.BucketSize);

    size_t elemPerBucket = local.size()/2;

    for (bucket_id_t bid = 0; bid < this->buckets_.NumBuckets; bid++) {
      //zero out readbuf
      std::fill(local.begin(), local.end(), Labeled<T>());

      //load eid's
      for(size_t offset = 0; offset < local.size(); offset++) {
        //TODO overflow check
        elem_id_t eid = bid * elemPerBucket + offset;
        //load an actual element
        if (eid < this->data_.Size && offset < elemPerBucket) {
          this->data_.ReadElement(eid, local[offset].Elem);
          local[offset].Type = libUtil::ItemType::NORMAL;
        }
        local[offset].Label = generator();
      }
      this->buckets_.WriteBucket(bid, local);
    }

  }

  template <typename T>
  void RecORBA<T>::saveData(std::vector<bucket_id_t> result) {
    std::vector<Labeled<T>> local;
    local.resize(this->buckets_.BucketSize);

    size_t eid = 0;
    for(auto &itr : result) {
      this->buckets_.ReadBucket(itr, local);
      for(auto &item : local){
        if(item.Type == libUtil::ItemType::NORMAL) {
          this->data_.WriteElement(eid++, item.Elem);
        }
      }
    }
  }

  template <typename T>
  bool RecORBA<T>::Shuffle(size_t gamma) {
    loadData();
    //invoke helper function for recursive shuffling
    std::vector<bucket_id_t> toSort;
    toSort.reserve(this->buckets_.NumBuckets);
    for (bucket_id_t bid = 0; bid < this->buckets_.NumBuckets; bid++) {
      toSort.push_back(bid);
    }
    shuffleHelper(toSort, 0 , gamma, 0, toSort.size());
    saveData(toSort);
    return true;
  }

  //some utility functions to find group numberings and sizes
  inline size_t log2(size_t n){
    size_t res = 0;
    for (; n > 1; n >>= 1, ++res);
    return res;
  }

  bool isPow2(size_t n){
    return (1UL << log2(n)) == n;
  }

  size_t getGroup(libUtil::label_t label, size_t offset, size_t numBuckets){
    size_t mask = (~(0UL)) << log2(numBuckets);
    return (label >> offset) & ~mask;
  }

  void transpose(std::vector<bucket_id_t>& buckets, size_t begin, size_t end, size_t rows){
    size_t cols = (end - begin)/rows;
    std::vector<bucket_id_t> temp;
    temp.resize(end - begin);
    for(size_t i = 0; i < rows; i++){
      for(size_t j = 0; j < cols; j++){
        temp[j* rows + i] = buckets[begin + i * cols + j];
      }
    }
    for(size_t i = 0; i < end - begin; i++) {
      buckets[i + begin] = temp[i];
    }
  }

  //recursive helper function
  //returns the sorted ordering of buckets
  template <typename T>
  void RecORBA<T>::shuffleHelper(std::vector<bucket_id_t>& buckets,
    size_t offset, size_t gamma, size_t begin, size_t end) {
    assert(isPow2(end - begin));
    size_t numBuckets = end - begin;

    //invoke BinPlace if beta <= gamma
    if(numBuckets <= gamma){
      //assign group tag based on label
      std::vector<Labeled<T>> localStorage;
      auto temp = std::vector<bucket_id_t>(
        buckets.begin() + begin, buckets.begin() + end);
      
      localStorage.resize(numBuckets * this->buckets_.BucketSize);
      this->buckets_.ReadBuckets(temp, localStorage);
      //assign group labels for binplace
      for (auto &itr : localStorage){
        itr.Group = getGroup(itr.Label, offset, numBuckets);
      }

      libUtil::BinAssign(localStorage, numBuckets, this->buckets_.BucketSize);

      this->buckets_.WriteBuckets(temp, localStorage);
      return;
    }

    //calculating betas
    size_t beta1 = static_cast<size_t>(std::ceil(std::log2(std::sqrt(numBuckets))));
    assert(beta1 < 64);
    beta1 = 1UL << beta1;
    size_t beta2 = numBuckets / beta1;

    //recurse every row
    #pragma omp taskloop shared(buckets)
    for (size_t b1 = 0; b1 < beta1; b1 ++) {
      auto rowBegin = begin + b1 * beta2;
      auto rowEnd = rowBegin + beta2;
      shuffleHelper(buckets, offset, gamma, rowBegin, rowEnd);
    }
    #pragma omp taskwait

    //transpose
    transpose(buckets, begin, end, beta1);

    //recurse every column
    #pragma omp taskloop shared(buckets)
    for (size_t b2 = 0; b2 < beta2; b2 ++) {
      auto rowBegin = begin + b2 * beta1;
      auto rowEnd = rowBegin + beta1;
      shuffleHelper(buckets, offset + log2(beta2), gamma, rowBegin, rowEnd);
    }
    #pragma omp taskwait
  }

template class RecORBA<int>;
} // namespace libORBA
