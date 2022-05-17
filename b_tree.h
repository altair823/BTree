//
// Created by 김태현 on 2022/05/10.
//

#ifndef B_TREE__B_TREE_H_
#define B_TREE__B_TREE_H_

#include <ostream>
#include "node/node.h"

enum class DeletionType {
  ClockwiseSpin,
  CounterClockwiseSpin,
  MergeLeft,
  MergeRight,
};

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
  size_t degree; // Maximum number of pointers.
  size_t min_pointer_count;
  Node<Value>* head;
  size_t depth;

  Result<SubRoot<Value>> split(Node<Value>* node);
  Result<Node<Value>*> remove_in_branch(Node<Value> *node, int index);
  void remove_in_leaf(Node<Value> *node, int index = -1);
  DeletionType HowToSolveLack(Node<Value> *parent, int target_node_index);
  void spin_clockwise(Node<Value> *parent, int target_node_index);
  void spin_counterclockwise(Node<Value> *parent, int target_node_index);
  void merge_with_right(Node<Value> *parent, int target_node_index);
  void merge_with_left(Node<Value> *parent, int target_node_index);
};

#include "b_tree.tpp"

#endif //B_TREE__B_TREE_H_
