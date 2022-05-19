//
// Created by 김태현 on 2022/05/10.
//

#ifndef B_TREE__NODE_H_
#define B_TREE__NODE_H_

#include "../data.h"
#include "../result.h"
#include <vector>
#include <algorithm>
#include <ostream>

#define LEAF true
#define NO_LEAF false

static int node_count = 0;

template <typename Value>
class Node;

template <typename Value>
using Pointer = std::shared_ptr<Node<Value>>;

template <typename Value>
class Node {
 public:
  Node();
  static Pointer<Value> make_node();
  int get_node_num(){return node_num;}

  Pointer<Value> get_pointer (int index) const;
  DataShared<Value> get_data(int index);
  [[nodiscard]] int get_pointer_count() const {return pointers.size();}
  int get_data_count(){return data.size();}
  void set_data(int index, DataShared<Value> new_data);
  Result<bool> set_pointer(int index, Pointer<Value> pointer);
  void set_parent(Node<Value>* parent);
  Node<Value>* get_parent(){return parent;};
  Result<bool> insert_data(int index, DataShared<Value> new_data);
  Result<bool> push_back(DataShared<Value> new_data, Pointer<Value> pointer = nullptr);
  void erase_data(int index);
  void erase_pointer(int index);
  void clear();
  Result<long> search(Key key);
  Result<int> search_node(Node<Value> *node);
  void set_leaf(bool leaf){is_node_leaf = leaf;}
  bool is_leaf(){return is_node_leaf;};
  void from(const Node<Value>* node);

  friend std::ostream &operator<<(std::ostream &os, const Node &node) {

    os << "node num: " << node.node_num;
    os << "\ndata:          ";
    for (auto &d : node.data) {
      if (d == nullptr) {
        os << "null";
      } else {
        os << d->get_key();
      }
      os << "        ";
    }
    os << "\npointers: ";
    for (auto &p : node.pointers) {
      if (p == nullptr) {
        os << "null";
      } else {
        os << p->node_num;
      }
      os << "     ";
    }
    return os;
  }



 private:

  int node_num;
  Node<Value>* parent;
  std::vector<Pointer<Value>> pointers;
  std::vector<DataShared<Value>> data;
  bool is_node_leaf;
};

#include "node.tpp"

#endif //B_TREE__NODE_H_
