#pragma once
#include <vector>

namespace libStorage {

using elem_id_t = size_t;

/**
 * @brief Interface definition for storing individual elements
 * 
 * @tparam T type of element
 */
template <typename T> class ElementStorage {
public:
  const size_t Size;

  /**
   * @brief Construct a new Element Storage object
   * 
   * @param Size capacity of the storage object
   */
  ElementStorage(size_t Size) : Size(Size) {}

  /**
   * @brief Read an element into a destination reference
   * 
   * @param eid element to copy into dst
   * @param dst object location to copy to
   */
  virtual void ReadElement(elem_id_t eid, T &dst) = 0;

  /**
   * @brief Write an element back into storage
   * 
   * @param eid element to write to
   * @param src object to copy from
   */
  virtual void WriteElement(elem_id_t eid, T &src) = 0;
};

} // namespace libStorage