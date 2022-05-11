//
// Created by 김태현 on 2022/05/11.
//

#ifndef B_TREE__DATA_H_
#define B_TREE__DATA_H_

#include <memory>

typedef int Key;

template <typename Value>
class Data{
 public:
  Data(Key key, Value value): key(key), value(value) {}
  Key get_key() const {return key;}
  Value get_value() const {return value;}
  bool operator<(const Data &rhs) const {
    return key < rhs.key;
  }
  bool operator>(const Data &rhs) const {
    return rhs.key < key;
  }
  bool operator<=(const Data &rhs) const {
    return rhs.key >= key;
  }
  bool operator>=(const Data &rhs) const {
    return key >= rhs.key;
  }
 private:
  Key key;
  Value value;

};

template <typename Value>
using DataShared = std::shared_ptr<Data<Value>>;

template <typename Value>
bool less_data_shared(const DataShared<Value> lhs, const DataShared<Value> rhs){
  return lhs->get_key() < rhs->get_key();
}

#endif //B_TREE__DATA_H_
