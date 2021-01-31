#pragma once
#include <vector>

namespace libStorage {

using elem_id_t = size_t;

template<typename T>
class ElementStorage {
  public:
    const size_t Size;

    ElementStorage(size_t Size) : Size(Size) {}
  
    virtual void ReadElement(elem_id_t eid, T& dst) = 0;
    virtual void WriteElement(elem_id_t eid, T& src) = 0;
};

} //libStorage