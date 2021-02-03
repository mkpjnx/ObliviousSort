#include "src/include/util/common.h"
#include "src/include/storage/elementStorage.h"
#include "src/include/storage/bucketStorage.h"
#include <vector>

namespace libOSort
{
  using libStorage::bucket_id_t;
  using libStorage::elem_id_t;
  using libUtil::Padded;

  size_t padToPow2(size_t size){
    return (1UL) << static_cast<size_t>(std::ceil(std::log2(size)));
  }

  template <typename T>
  class RecSort {
    public:
      RecSort(libStorage::ElementStorage<T> &data,
        libStorage::BucketStorage<Padded<T>> &buckets, double overflowC);
      bool Sort(size_t gamma);
    private:
      void generatePivots();
      void loadData();
      void saveData(std::vector<bucket_id_t> result);
      void sortBase(std::vector<bucket_id_t>& buckets, std::vector<Padded<T>>& pivots);
      std::vector<bucket_id_t> sortRecursive(
        std::vector<bucket_id_t>& buckets, std::vector<Padded<T>>& pivots, size_t gamma);
    private:
      std::vector<Padded<T>> pivots_;
      libStorage::ElementStorage<T> &data_;
      libStorage::BucketStorage<Padded<T>> &buckets_;
      double overflowC_;
  };
} // namespace libOSort
