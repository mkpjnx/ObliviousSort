#include "src/include/storage/bucketStorage.h"

namespace libStorage
{

template<typename T>
class VectorBucketStore : public BucketStorage<T> {
  public:
    VectorBucketStore(size_t numBuckets, size_t bucketSize);
    
    void ReadBucket(bucket_id_t bid, std::vector<T>& dst) override;
    void ReadBuckets(std::vector<bucket_id_t> &bids, std::vector<T>& dst) override;

    void WriteBucket(bucket_id_t bid, std::vector<T>& src) override;
    void WriteBuckets(std::vector<bucket_id_t> &bids, std::vector<T>& src) override;
  private:
    std::vector<T> vec_;
};

} // namespace libStorage