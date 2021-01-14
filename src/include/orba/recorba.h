#include "src/include/orba/orba.h"

#include <random>

namespace libORBA
{
  template <typename T>
  class RecORBA : public ORBA<T>{
    public:
      RecORBA(libStorage::ElementStorage<T> data, libStorage::BucketStorage<Labeled<T>> buckets);
      bool Shuffle(size_t gamma) override;
    private:
      void loadData();
      void saveData();
      std::vector<bucket_id_t> shuffleHelper(std::vector<bucket_id_t>& buckets, size_t offset, size_t gamma);
  };
} // namespace libORBA