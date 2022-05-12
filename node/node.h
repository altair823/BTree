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
  Node(size_t degree);
  static Pointer<Value> make_node(size_t degree);

  Result<Pointer<Value>> get_pointer(int index);
  Result<DataShared<Value>> get_data(int index);
  int get_pointer_count();
  Result<bool> set_data(int index, DataShared<Value> new_data);
  Result<bool> set_pointer(int index, Pointer<Value> pointer);
  void set_parent(Node<Value>* parent);
  Node<Value>* get_parent(){return parent;};
  Result<bool> insert_data(int index, DataShared<Value> new_data);
  Result<bool> insert_pointer(int index, Pointer<Value> pointer);
  Result<bool> push_back_data(DataShared<Value> new_data);
  Result<bool> push_back_pointer(Pointer<Value> pointer);
  void clear();
  Result<long> search(DataShared<Value> new_data);
  void make_leaf();
  void make_no_leaf();
  bool is_leaf(){return is_node_leaf;};
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
  size_t max_pointer_count;
  std::vector<DataShared<Value>> data;
  size_t max_data_count;
  bool is_node_leaf;
};

#include "node.tpp"

#endif //B_TREE__NODE_H_
