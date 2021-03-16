#pragma once
#include<cstdint>
#include<cmath>
#include<vector>
#include<exception>

namespace libUtil
{
  using label_t = unsigned long;
  enum class ItemTag {NONE, NORMAL, EXCESS};
  enum class ItemType {NORMAL, FILLER};

  class binOverflowException: public std::exception{
    virtual const char* what() const throw()
    {
      return "Bin overflow detected";
    }
  };

  template <typename T> class Labeled;
  template <typename T> void BinAssign(std::vector<Labeled<T>> &vec, size_t beta, size_t Z);
  template <typename T>
  class Labeled {
    public:
      T Elem;
      //TODO: make this adjustable
      label_t Label; //Used to store the actual assigned random label
      size_t Group; //Used to calculate group in bin placement
      ItemType Type {ItemType::FILLER};

      friend bool operator < (const Labeled& lhs, const Labeled& rhs) {
        //filter excess
        if (rhs.tag_ == ItemTag::EXCESS || lhs.tag_ == ItemTag::EXCESS) {
          return rhs.tag_ == ItemTag::EXCESS;
        }
        //group ordering
        if (lhs.Group != rhs.Group) {
          return lhs.Group < rhs.Group;
        }
        //sort within bucket
        if (lhs.Type == ItemType::NORMAL && rhs.Type == ItemType::NORMAL) {
          return lhs.Label < rhs.Label;
        }
        //in the same group, filler always last
        return lhs.Type < rhs.Type;
      }

      friend void BinAssign<T>(std::vector<Labeled<T>> &vec, size_t beta, size_t Z);
    private:
      ItemTag tag_;
      size_t offset_;
  };

  template <typename T>
  struct Padded {
    T Elem;
    bool real;

    friend bool operator <(const Padded<T>& lhs, const Padded<T>& rhs) {
      if(lhs.real && rhs.real) {
        return lhs.Elem < rhs.Elem;
      }
      return lhs.real;
    }
  };

  template <typename T>
  void recTranspose(std::vector<T> &A, size_t rows, size_t cols, size_t offset,
                    std::vector<T> &B, size_t i, size_t j, size_t h, size_t w){
    if (!w || !h) return;
    if (w == 1 && h == 1) {
      B[j*rows + i + offset] = A[i * cols + j + offset];
    } else if (w > h) {
      recTranspose(A, rows, cols, offset, B, i, j, h, w/2);
      recTranspose(A, rows, cols, offset, B, i, j+w/2 , h, w - w/2);
    } else {
      recTranspose(A, rows, cols, offset, B, i, j, h/2, w);
      recTranspose(A, rows, cols, offset, B, i + h/2, j , h - h/2, w);
    }
  }
} // namespace libUtil
