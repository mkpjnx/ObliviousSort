#pragma once
#include <vector>

namespace libStorage {

using bucket_id_t = size_t;

/**
 * @brief Interface definition for storing "buckets" of elements
 *
 * @tparam T type of element
 */
template <typename T> class BucketStorage {
public:
  const size_t NumBuckets;
  const size_t BucketSize;

  /**
   * @brief Construct a new Bucket Storage object
   * 
   * @param numBuckets number of buckets in the storage object
   * @param bucketSize capacity of each bucket
   */
  BucketStorage(size_t numBuckets, size_t bucketSize)
      : NumBuckets(numBuckets), BucketSize(bucketSize) {}

  /**
   * @brief read a bucket into the destination
   * 
   * @param bid id of the bucket to read
   * @param dst destination to copy the contents
   */
  virtual void ReadBucket(bucket_id_t bid, std::vector<T> &dst) = 0;

  /**
   * @brief read a set of buckets specified by a vector of bids
   * 
   * @param bids vector of bucket ids to read from in order
   * @param dst  destination to copy the contents, must be large enough
   */
  virtual void ReadBuckets(std::vector<bucket_id_t> &bids,
                           std::vector<T> &dst) = 0;

  /**
   * @brief write contents back to a specified bucket
   * 
   * @param bid id of the bucket to write
   * @param src source to copy from
   */
  virtual void WriteBucket(bucket_id_t bid, std::vector<T> &src) = 0;

  /**
   * @brief write contents back to a specified bucket
   * 
   * @param bids vector of bucket ids to write to in order
   * @param src source to copy from
   */
  virtual void WriteBuckets(std::vector<bucket_id_t> &bids,
                            std::vector<T> &src) = 0;
};

} // namespace libStorage