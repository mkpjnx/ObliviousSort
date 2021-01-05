namespace libUtil {

enum ItemTag {NONE, NORMAL, EXCESS};
enum class ItemType {NORMAL, FILLER};

template <typename T>
class Item {
  public:
    T Elem;
    ItemType Type {ItemType::FILLER};
    size_t Group;
    Item(T elem, size_t g, ItemType t) : Elem(elem), Type(t), Group(g) {}
    
    static void BinAssign(std::vector<Item> &vec, size_t beta, size_t Z);

    friend bool operator < (const Item& lhs, const Item& rhs) {
      //filter excess
      if (rhs.tag == ItemTag::EXCESS || lhs.tag == ItemTag::EXCESS) {
        return rhs.tag == ItemTag::EXCESS;
      }

      //group ordering
      if (lhs.Group != rhs.Group) {
        return lhs.Group < rhs.Group;
      }

      //in the same group, filler always last
      return rhs.Type == ItemType::FILLER;
    }
  //private:
    ItemTag tag;
    size_t offset;
};

} // namespace libUtil