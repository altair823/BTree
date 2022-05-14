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
struct DeletedNode{
  Node<Value>* node;
  int null_index;
};

template <typename Value>
void print_node(Pointer<Value> node, size_t depth);

template <typename Value>
class BTree{
 public:
  BTree(size_t degree);
  Result<bool> insert(DataShared<Value> data);
  Result<bool> remove(Key key);


  void print(){
    print_node(std::make_shared<Node<Value>>(*head), 1);
  }

 private:
  size_t degree; // number of pointers.
  Node<Value>* head;
  size_t depth;

  Result<SubRoot<Value>> split(Node<Value>* node);
  DeletedNode<Value> remove_in_branch(Node<Value> *node, int index);
};

#include "b_tree.tpp"

#endif //B_TREE__B_TREE_H_
