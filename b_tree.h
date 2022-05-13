//
// Created by 김태현 on 2022/05/10.
//

#ifndef B_TREE__B_TREE_H_
#define B_TREE__B_TREE_H_

#include <ostream>
#include "node/node.h"

template <typename Value>
struct SubRoot{
  DataShared<Value> data;
  Pointer<Value> l_child;
  Pointer<Value> r_child;
};

template <typename Value>
class BTree{
 public:
  BTree(size_t degree);
  Result<bool> insert(DataShared<Value> data);

  friend std::ostream &operator<<(std::ostream &os, const BTree &tree){
    os << "head: \n";
    os << *tree.head;
    return os;
  }

 private:
  size_t degree; // number of pointers.
  Node<Value>* head;

  Result<SubRoot<Value>> split(Node<Value>* node);
};

#include "b_tree.tpp"

#endif //B_TREE__B_TREE_H_
