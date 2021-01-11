#pragma once
#include "src/include/util/common.h"
#include <vector>

namespace libUtil {

enum class ItemTag {NONE, NORMAL, EXCESS};
enum class ItemType {NORMAL, FILLER};

template <typename T> class BinItem;
template <typename T> void BinAssign(std::vector<BinItem<T>> &vec, size_t beta, size_t Z);
template <typename T>
class BinItem {
  public:
    T Elem;
    ItemType Type {ItemType::FILLER};
    size_t Group;

    BinItem() = default;
    BinItem(T elem, size_t g, ItemType t) : Elem(elem), Type(t), Group(g) {}

    friend bool operator < (const BinItem& lhs, const BinItem& rhs) {
      //filter excess
      if (rhs.tag_ == ItemTag::EXCESS || lhs.tag_ == ItemTag::EXCESS) {
        return rhs.tag_ == ItemTag::EXCESS;
      }
      //group ordering
      if (lhs.Group != rhs.Group) {
        return lhs.Group < rhs.Group;
      }
      //in the same group, filler always last
      return rhs.Type == ItemType::FILLER;
    }

    friend void BinAssign<T>(std::vector<BinItem<T>> &vec, size_t beta, size_t Z);
  private:
    ItemTag tag_;
    size_t offset_;
};

} // namespace libUtil