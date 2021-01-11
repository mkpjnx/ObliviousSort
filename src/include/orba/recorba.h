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
  };
} // namespace libORBA