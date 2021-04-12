#pragma once
#include "src/include/storage/bucketStorage.h"
#include "src/include/storage/elementStorage.h"
#include "src/include/util/binplace.h"
#include "src/include/util/common.h"
#include <cassert>
#include <cmath>

namespace libOSort {
using libStorage::bucket_id_t;
using libStorage::elem_id_t;
using libUtil::Labeled;

/**
 * @brief Interface of the ORBA algorithm
 * 
 * @tparam T type of element
 */
template <typename T> class ORBA {
public:
  /**
   * @brief Construct a new ORBA object
   * 
   * @param data    the element store containing the elements to shuffle
   * @param buckets the bucket store to be used to perform ORBA 
   */
  ORBA(libStorage::ElementStorage<T> &data,
       libStorage::BucketStorage<Labeled<T>> &buckets)
      : data_(data), buckets_(buckets) {
    // ensure there is enough space
    assert(data_.Size <= buckets_.NumBuckets * buckets_.BucketSize / 2);

    // ensure number of buckets is a power of 2
    assert(std::ceil(std::log2(buckets_.NumBuckets)) ==
           std::floor(std::log2(buckets_.NumBuckets)));
  }

  /**
   * @brief Perform the ORBA algorithm
   * 
   * @param gamma branching factor
   * @return true on success
   * @return false on failure
   */
  virtual bool Shuffle(size_t gamma) = 0;

protected:
  libStorage::ElementStorage<T> &data_;
  libStorage::BucketStorage<Labeled<T>> &buckets_;
};
} // namespace libOSort
