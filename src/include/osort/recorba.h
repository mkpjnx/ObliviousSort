#include "src/include/util/common.h"
#include "src/include/osort/orba.h"
#include <omp.h>
#include <random>

namespace libOSort
{
  template <typename T>
  class RecORBA : public ORBA<T>{
    public:
      RecORBA(libStorage::ElementStorage<T> &data, libStorage::BucketStorage<Labeled<T>> &buckets);
      bool Shuffle(size_t gamma) override;
    private:
      void loadData();
      void saveData();
      void shuffleHelper(size_t offset, size_t gamma, size_t begin, size_t end);
      void transpose(size_t begin, size_t end, size_t rows);
      std::vector<bucket_id_t> bucketOrder_;
      std::vector<bucket_id_t> tempBuckets_;
  };
} // namespace libORBA