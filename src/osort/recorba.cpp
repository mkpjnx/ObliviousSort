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

    // elemPerBucket is half of bucket size
    size_t elemPerBucket = local.size()/2;

    // fill each bucket to half capacity
    for (bucket_id_t bid = 0; bid < this->buckets_.NumBuckets; bid++) {
      
      //zero out readbuf
      std::fill(local.begin(), local.end(), Labeled<T>());

      //load elems into bucket
      for(size_t offset = 0; offset < local.size(); offset++) {

        //TODO overflow check
        elem_id_t eid = bid * elemPerBucket + offset;
        local[offset].Label = generator(); // get random label
        local[offset].Type = libUtil::ItemType::FILLER; // default to filler

        //load an actual element up until bucket is half full
        if (eid < this->data_.Size && offset < elemPerBucket) {
          // load into labeled container
          this->data_.ReadElement(eid, local[offset].Elem);
          local[offset].Type = libUtil::ItemType::NORMAL;
        }
      }

      // write bucket to storage device
      this->buckets_.WriteBucket(bid, local);
    }

  }

  template <typename T>
  void RecORBA<T>::saveData() {
    std::vector<Labeled<T>> local;
    local.resize(this->buckets_.BucketSize);

    size_t eid = 0;
    for(auto &itr : bucketOrder_) {
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
  
    bucketOrder_.resize(this->buckets_.NumBuckets);
    tempBuckets_.resize(this->buckets_.NumBuckets);
    // the initial bucket order is sequential, this changes over time
    // via recursively smaller transposes
    #pragma omp parallel for
    for (bucket_id_t bid = 0; bid < this->buckets_.NumBuckets; bid++) {
      bucketOrder_[bid] = bid;
    }

    bool err = false;
    #pragma omp parallel shared(bucketOrder_)
    {
      #pragma omp single
      try {
        shuffleHelper(0 , gamma, 0, bucketOrder_.size());
      } catch(libUtil::binOverflowException &e) {
        err = true;
      }
    }
    if (err) throw libUtil::binOverflowException();

    saveData();
    return true;
  }

  /*********************
   * UTILITY FUNCTIONS *
   *********************/

  /**
   * @brief return log_2(n), rounded down
   * 
   * @param n 
   * @return size_t 
   */
  inline size_t log2(size_t n){
    size_t res = 0;
    for (; n > 1; n >>= 1, ++res);
    return res;
  }

  inline bool isPow2(size_t n){
    return (1UL << log2(n)) == n;
  }

  /**
   * @brief Get the Group assignment of an object
   * 
   * @param label label of the object
   * @param offset offset of the label to use to calculate group 
   * @param numBuckets number of buckets to distribute group assignments over
   *                   determines how many bits of the label are used
   * @return size_t the group number calculated
   */
  size_t getGroup(libUtil::label_t label, size_t offset, size_t numBuckets){
    size_t mask = (~(0UL)) << log2(numBuckets);
    return (label >> offset) & ~mask;
  }

  template <typename T>
  void RecORBA<T>::transpose(size_t begin, size_t end, size_t rows){
    size_t cols = (end - begin)/rows;
    // copy to temp
    //#pragma omp parallel for
    for(size_t i = begin; i < end; i++) {
      tempBuckets_[i] = bucketOrder_[i];
    }

    // transpose from temp and store into bucketOrder
    libUtil::recTranspose<bucket_id_t>(tempBuckets_, rows, cols, begin,
      bucketOrder_, 0, 0, rows, cols);
  }

  template <typename T>
  void RecORBA<T>::shuffleHelper(size_t offset, size_t gamma, size_t begin, size_t end) {
    assert(isPow2(end - begin));
    size_t numBuckets = end - begin;

    //invoke BinPlace if beta <= gamma
    if(numBuckets <= gamma){
      //assign group tag based on label
      // ... //
      std::vector<Labeled<T>> localStorage;
      auto temp = std::vector<bucket_id_t>(
        bucketOrder_.begin() + begin, bucketOrder_.begin() + end);
      
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

    bool success;
    
    success = true;
    //recurse every row
    #pragma omp taskloop shared(bucketOrder_, success)
    for (size_t b1 = 0; b1 < beta1; b1 ++) {
      auto rowBegin = begin + b1 * beta2;
      auto rowEnd = rowBegin + beta2;
  
      try{
        shuffleHelper(offset, gamma, rowBegin, rowEnd);
      } catch(libUtil::binOverflowException &e) {
        success = false;
      }
  
    }

    if (!success) throw libUtil::binOverflowException();


    //transpose bucketOrder subrange
    transpose(begin, end, beta1);

    success = true;
    //recurse every column
    #pragma omp taskloop shared(bucketOrder_, success)
    for (size_t b2 = 0; b2 < beta2; b2 ++) {
      auto rowBegin = begin + b2 * beta1;
      auto rowEnd = rowBegin + beta1;
      try{
        shuffleHelper(offset + log2(beta2), gamma, rowBegin, rowEnd);
      } catch(libUtil::binOverflowException &e) {
        success = false;
      }
    }
    if (!success) throw libUtil::binOverflowException();
  }

template class RecORBA<int>;
} // namespace libORBA
