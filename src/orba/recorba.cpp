#include "src/include/orba/recorba.h"

namespace libORBA
{
  using libStorage::elem_id_t;
  using libStorage::bucket_id_t;
  
  template <typename T>
  RecORBA<T>::RecORBA(libStorage::ElementStorage<T> data,
    libStorage::BucketStorage<Labeled<T>> buckets)
   : ORBA<T>(data, buckets) {}
  
  template <typename T>
  void RecORBA<T>::loadData() {
    std::random_device rd;
    std::mt19937_64 generator(rd());

    //load all elements into buckets with random label
    std::vector<Labeled<T>> local;
    local.resize(this->buckets_.BucketSize);

    for (bucket_id_t bid = 0; bid < this->buckets_.NumBuckets; bid++) {
      //zero out readbuf
      std::fill(local.begin(), local.end(), Labeled<T>());

      //load eid's
      for(size_t offset = 0; offset < local.size(); offset++) {
        //TODO overflow check
        elem_id_t eid = bid * this->buckets._BucketSize + offset;
        //load an actual element
        if (eid < this->data_.Size) {
          this->data_.ReadElement(eid, local[offset]);
          local[offset].real = true;
        }
        local[offset].label = generator();
      }
      this->buckets_.WriteBucket(bid, local);
    }

  }

  template <typename T>
  bool RecORBA<T>::Shuffle(size_t gamma) {
    loadData();
    //invoke helper function for recursive shuffling
    //TODO: catch bin assignment overflow
    return true;
  }

} // namespace libORBA
