#include "src/include/storage/vectorStore.h"

namespace libStorage
{

template<typename T>
VectorBucketStore<T>::VectorBucketStore(size_t NumBuckets, size_t BucketSize)
  : BucketStorage<T>(NumBuckets, BucketSize){
  vec_.resize(NumBuckets*BucketSize);
}

template<typename T>
void VectorBucketStore<T>::ReadBucket(bucket_id_t bid, std::vector<T>& dst){
  assert(dst.size() >= this->BucketSize); // enough space in dst
  assert(bid < this->NumBuckets); // valid bucket id
  size_t offset = bid * this->BucketSize;
  for(size_t ind = 0; ind < this->BucketSize; ind++) {
    dst[ind] = vec_[ind + offset];
  }
}

template<typename T>
void VectorBucketStore<T>::ReadBuckets(std::vector<bucket_id_t> &bids, std::vector<T>& dst){
  assert(dst.size() >= this->BucketSize * bids.size());

  for(size_t b_index = 0; b_index < bids.size(); b_index++) {
    size_t bid = bids[b_index];
    assert(bid < this->NumBuckets);

    size_t vec_offset = bid * this->BucketSize;
    size_t dst_offset = b_index * this->BucketSize;

    for(size_t ind = 0; ind < this->BucketSize; ind++) {
      dst[ind + dst_offset] = vec_[ind + vec_offset];
    }
  }
}

template<typename T>
void VectorBucketStore<T>::WriteBucket(bucket_id_t bid, std::vector<T>& src){
  assert(src.size() >= this->BucketSize);
  assert(bid < this->NumBuckets);
  size_t offset = bid * this->BucketSize;
  for(size_t ind = 0; ind < this->BucketSize; ind++) {
    vec_[ind + offset] = src[ind];
  }
}

template<typename T>
void VectorBucketStore<T>::WriteBuckets(std::vector<bucket_id_t> &bids, std::vector<T>& src){
  assert(src.size() >= this->BucketSize * bids.size());

  for(size_t b_index = 0; b_index < bids.size(); b_index++) {
    size_t bid = bids[b_index];
    assert(bid < this->NumBuckets);

    size_t vec_offset = bid * this->BucketSize;
    size_t src_offset = b_index * this->BucketSize;

    for(size_t ind = 0; ind < this->BucketSize; ind++) {
      vec_[ind + vec_offset] = src[ind + src_offset];
    }
  }
}

template class VectorBucketStore<int>;

using libUtil::Labeled;
using libUtil::Padded;
template class VectorBucketStore<Labeled<int>>;
template class VectorBucketStore<Padded<int>>;

} // namespace libStorage