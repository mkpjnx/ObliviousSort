#pragma once
#include "src/include/storage/bucketStorage.h"
#include "src/include/storage/elementStorage.h"
#include "src/include/util/common.h"
#include <cassert>

namespace libStorage {

/**
 * @brief Implementation of the elementStorage interface using vectors
 * 
 * @tparam T type of element
 */
template <typename T> class VectorElementStore : public ElementStorage<T> {
public:
  VectorElementStore(size_t size) : ElementStorage<T>(size) {
    vec_.resize(size);
  }
  void ReadElement(elem_id_t eid, T &dst) {
    assert(eid < this->Size);
    dst = vec_[eid];
  }

  void WriteElement(elem_id_t eid, T &src) {
    assert(eid < this->Size);
    vec_[eid] = src;
  }

private:
  std::vector<T> vec_;
};

/**
 * @brief Implementation of the bucketStorage interface using vectors
 * 
 * @tparam T type of element
 */
template <typename T> class VectorBucketStore : public BucketStorage<T> {
public:
  VectorBucketStore(size_t numBuckets, size_t bucketSize);

  void ReadBucket(bucket_id_t bid, std::vector<T> &dst) override;
  void ReadBuckets(std::vector<bucket_id_t> &bids,
                   std::vector<T> &dst) override;

  void WriteBucket(bucket_id_t bid, std::vector<T> &src) override;
  void WriteBuckets(std::vector<bucket_id_t> &bids,
                    std::vector<T> &src) override;

private:
  std::vector<T> vec_;
};

} // namespace libStorage
