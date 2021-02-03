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
    std::vector<bucket_id_t> result = shuffleHelper(toSort, 0 , gamma);
    saveData(result);
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


  //recursive helper function
  //returns the sorted ordering of buckets
  template <typename T>
  std::vector<bucket_id_t> RecORBA<T>::shuffleHelper(std::vector<bucket_id_t>& buckets, size_t offset, size_t gamma) {
    assert(isPow2(buckets.size()));

    //invoke BinPlace if beta <= gamma
    if(buckets.size() <= gamma){
      //assign group tag based on label
      std::vector<Labeled<T>> localStorage;
      localStorage.resize(buckets.size() * this->buckets_.BucketSize);
      this->buckets_.ReadBuckets(buckets, localStorage);

      //assign group labels for binplace
      for (auto &itr : localStorage){
        itr.Group = getGroup(itr.Label, offset, buckets.size());
      }
      libUtil::BinAssign(localStorage, buckets.size(), this->buckets_.BucketSize);

      this->buckets_.WriteBuckets(buckets, localStorage);
      return buckets;
      //TODO overflow checks
    }

    //calculating betas
    size_t beta1 = static_cast<size_t>(std::ceil(std::log2(std::sqrt(buckets.size()))));
    assert(beta1 < 64);
    beta1 = 1UL << beta1;
    size_t beta2 = buckets.size() / beta1;

    //store the "matrix" ordering of the buckets
    std::vector<bucket_id_t> firstPass;
    std::vector<bucket_id_t> secondPass;

    //recurse every row
    for (size_t b1 = 0; b1 < beta1; b1 ++) {
      auto begin = buckets.begin() + b1 * beta2;
      auto end = begin + beta2;
      auto toSort = std::vector<bucket_id_t>(begin, end);
      auto result = shuffleHelper(toSort, offset, gamma);
      firstPass.insert(firstPass.end(), result.begin(), result.end());
    }

    //recurse every column
    for (size_t b2 = 0; b2 < beta2; b2 ++) {
      auto toSort = std::vector<bucket_id_t>();
      for (size_t b1 = 0; b1 < beta1; b1 ++) {
        toSort.push_back(firstPass[b1 * beta2 + b2]);
      }
      auto result = shuffleHelper(toSort, offset + log2(beta2), gamma);
      secondPass.insert(secondPass.end(), result.begin(), result.end());
    }

    return secondPass;

  }

template class RecORBA<int>;
} // namespace libORBA
