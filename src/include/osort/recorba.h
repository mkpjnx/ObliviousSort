#pragma once
#include "src/include/osort/orba.h"
#include "src/include/util/common.h"
#include <omp.h>
#include <random>

namespace libOSort {

/**
 * @brief Implementation of a recursive version of ORBA 
 * 
 * @tparam T 
 */
template <typename T> class RecORBA : public ORBA<T> {
public:
  RecORBA(libStorage::ElementStorage<T> &data,
          libStorage::BucketStorage<Labeled<T>> &buckets);
  bool Shuffle(size_t gamma) override;

private:
  /**
   * @brief helper for loading data into buckets and padding with fillers
   */
  void loadData();

  /**
   * @brief helper for recovering non-filler elems from the buckets and
   * copying back into element storage
   */
  void saveData();

  /**
   * @brief recursive helper function of the rec-orba algorithm
   *        updates the sub-range of bucket-order with the transposed versions
   * @param offset offset into the random label to obtain group assignment
   * @param gamma  calls bin-assignment when there are < gamma buckets
   * @param begin  begin index of bucket_order, indicating the range of buckets
   *               recorba is performed on
   * @param end    end index (exclusive) of bucket order
   */
  void shuffleHelper(size_t offset, size_t gamma, size_t begin, size_t end);

  /**
   * @brief transpose the specified range of bucket order as if it was a
   * matrix with the specified dimensions
   * 
   * @param begin begin index of bucketOrder to transpose
   * @param end   end index (exclusive) of bucketOrder to transpose
   * @param rows number of rows in the matrix represented by this subrange
   */
  void transpose(size_t begin, size_t end, size_t rows);

  /** @brief global array of bucket ids used to track their ordering */
  std::vector<bucket_id_t> bucketOrder_;
  /** @brief temporary array used during transpose */ 
  std::vector<bucket_id_t> tempBuckets_;
};
} // namespace libOSort