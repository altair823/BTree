

#include "b_tree.h"

template<typename Value>
BTree<Value>::BTree(size_t degree): head(nullptr), degree(degree) {

}
template<typename Value>
Result<bool> BTree<Value>::insert(DataShared<Value> data) {

  // If head is null.
  if (head == nullptr){
    head = new Node<Value>(degree);
    head->push_back_data(data);
  }

  // Else, find the leaf node where the new data will insert.
  else{
    Node<Value>* current_node = head;
    while (!current_node->is_leaf()){
      auto index = current_node->search(data).unwrap();
      current_node = current_node->get_pointer(index).unwrap().get();
    }
    auto index = current_node->search(data).unwrap();
    current_node->insert_data(index, data).unwrap();

    if (current_node->get_pointer_count() <= degree){
      return Ok(true);
    } else {
      do {
        auto sub_root = split(current_node).unwrap();
        current_node->clear();
        current_node->push_back_data(sub_root.data).unwrap();
        current_node->set_pointer(0, sub_root.l_child);
        current_node->set_pointer(1, sub_root.r_child);
        current_node->set_leaf(NO_LEAF);
        if (current_node != head){
          auto parent = current_node->get_parent();
          auto p_index = parent->search(current_node->get_data(0).unwrap()).unwrap();
          parent->insert_data(p_index, current_node->get_data(0).unwrap()).unwrap();
          parent->set_pointer(p_index, current_node->get_pointer(0).unwrap()).unwrap();
          parent->set_pointer(p_index + 1, current_node->get_pointer(1).unwrap()).unwrap();

          current_node = current_node->get_parent();
        } else {
          head = current_node;
          break;
        }
      } while (current_node->get_pointer_count() > degree);
    }
  }

  return Ok(true);
}
template<typename Value>
Result<SubRoot<Value>> BTree<Value>::split(Node<Value>* node) {
  bool is_leaf = node->is_leaf();
  int depth = node->get_depth();
  int index = node->get_pointer_count() / 2 - 1;
  DataShared<Value> e = node->get_data(index).unwrap();
  Pointer<Value> l_child = Node<Value>::make_node(degree);
  l_child->set_leaf(is_leaf);
  l_child->set_depth(depth + 1);
  Pointer<Value> r_child = Node<Value>::make_node(degree);
  r_child->set_leaf(is_leaf);
  r_child->set_depth(depth + 1);

  for (int i = 0; i < index; i++){
    l_child->insert_data(i, node->get_data(i).unwrap()).unwrap();
  }
  for (int i = 0; i <= index; i++){
    l_child->set_pointer(i, node->get_pointer(i).unwrap()).unwrap();
  }

  for (int i = index + 1; i < node->get_pointer_count() - 1; i++){
    r_child->insert_data(i - index - 1, node->get_data(i).unwrap()).unwrap();
  }
  for (int i = index + 1; i < node->get_pointer_count(); i++){
    r_child->set_pointer(i - index - 1, node->get_pointer(i).unwrap()).unwrap();
  }


  auto sub_root = SubRoot<Value>();
  sub_root.data = e;
  sub_root.l_child = l_child;
  sub_root.r_child = r_child;
  return Ok(sub_root);
}
