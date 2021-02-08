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
} // namespace libUtil
