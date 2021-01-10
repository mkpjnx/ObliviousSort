#include <cstdint>
#include <vector>

namespace libStorage {

using bucket_id_t = int32_t;

template<typename T>
class BucketStorage {
  public:
    const size_t NumBuckets;
    const size_t BucketSize;

    BucketStorage(size_t numBuckets, size_t bucketSize) : NumBuckets(numBuckets), BucketSize(bucketSize) {}
  
    virtual void ReadBucket(bucket_id_t bid, std::vector<T>& dst) = 0;
    virtual void ReadBuckets(std::vector<bucket_id_t> &bids, std::vector<T>& dst) = 0;

    virtual void WriteBucket(bucket_id_t bid, std::vector<T>& src) = 0;
    virtual void WriteBuckets(std::vector<bucket_id_t> &bids, std::vector<T>& src) = 0;
};

} //libStorage