#include <cassert>
#include <cmath>
#include "src/include/util/common.h"
#include "src/include/util/binplace.h"
#include "src/include/storage/elementStorage.h"
#include "src/include/storage/bucketStorage.h"

namespace libORBA
{
  using libUtil::Labeled;
  using libStorage::bucket_id_t;
  using libStorage::elem_id_t;

  template <typename T>
  class ORBA {
    public:
      ORBA(libStorage::ElementStorage<T> &data, libStorage::BucketStorage<Labeled<T>> &buckets) 
       : data_(data), buckets_(buckets) {
         assert(data_.Size <= buckets_.NumBuckets * buckets_.BucketSize / 2);
         assert(std::ceil(std::log2(buckets_.NumBuckets)) == std::floor(std::log2(buckets_.NumBuckets)));
      }
      
      virtual bool Shuffle(size_t gamma) = 0;
    protected:
      libStorage::ElementStorage<T> &data_;
      libStorage::BucketStorage<Labeled<T>> &buckets_;

  };
} // namespace libORBA
