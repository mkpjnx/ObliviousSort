#pragma once
#include <cmath>
#include <cstdint>
#include <exception>
#include <vector>

namespace libUtil {
using label_t = unsigned long;
enum class ItemTag { NONE, NORMAL, EXCESS };
enum class ItemType { NORMAL, FILLER };

/**
 * @brief exception thrown when bin assignment algorithm detects an overflow
 */
class binOverflowException : public std::runtime_error {
  virtual const char *what() const throw() { return "Bin overflow detected"; }
};

template <typename T> class Labeled;

/**
 * @brief Oblivious bin assignment
 * 
 * @pre         vec.size() = beta * Z, elems are labeled with groups 0 to Z-1
 * @param vec   vector to sort int bins
 * @param beta  bin size
 * @param Z     number of bins
 */
template <typename T>
void BinAssign(std::vector<Labeled<T>> &vec, size_t beta, size_t Z);
template <typename T> class Labeled {
public:
  T Elem;
  // TODO: make this adjustable
  label_t Label; // Used to store the actual assigned random label
  size_t Group;  // Used to calculate group in bin placement
  ItemType Type{ItemType::FILLER};

  friend bool operator<(const Labeled &lhs, const Labeled &rhs) {
    // filter excess
    if (rhs.tag_ == ItemTag::EXCESS || lhs.tag_ == ItemTag::EXCESS) {
      return rhs.tag_ == ItemTag::EXCESS;
    }
    // group ordering
    if (lhs.Group != rhs.Group) {
      return lhs.Group < rhs.Group;
    }
    // sort within bucket
    if (lhs.Type == ItemType::NORMAL && rhs.Type == ItemType::NORMAL) {
      return lhs.Label < rhs.Label;
    }
    // in the same group, filler always last
    return lhs.Type < rhs.Type;
  }

  friend void BinAssign<T>(std::vector<Labeled<T>> &vec, size_t beta, size_t Z);

private:
  ItemTag tag_;
  size_t offset_;
};

template <typename T> struct Padded {
  T Elem;
  bool real;

  friend bool operator<(const Padded<T> &lhs, const Padded<T> &rhs) {
    if (lhs.real && rhs.real) {
      return lhs.Elem < rhs.Elem;
    }
    return lhs.real;
  }
};

/**
 * @brief Recursive transpose
 *
 * @tparam T
 * @param A       Source vec
 * @param rows    height of the matrix
 * @param cols    width of the matrix
 * @param offset  offset into both vectors where the matrix is stored
 * @param B       Destination vec
 * @param i       beginning row index of submatrix
 * @param j       beginning column index of submatrix
 * @param h       height of the submatrix
 * @param w       width of the submatrix
 */
template <typename T>
void recTranspose(std::vector<T> &A, size_t rows, size_t cols, size_t offset,
                  std::vector<T> &B, size_t i, size_t j, size_t h, size_t w) {
  if (!w || !h)
    return;
  if (w == 1 && h == 1) {
    B[j * rows + i + offset] = A[i * cols + j + offset];
  } else if (w > h) {
    recTranspose(A, rows, cols, offset, B, i, j, h, w / 2);
    recTranspose(A, rows, cols, offset, B, i, j + w / 2, h, w - w / 2);
  } else {
    recTranspose(A, rows, cols, offset, B, i, j, h / 2, w);
    recTranspose(A, rows, cols, offset, B, i + h / 2, j, h - h / 2, w);
  }
}
} // namespace libUtil
