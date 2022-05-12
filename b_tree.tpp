

#include "b_tree.h"

template<typename Value>
BTree<Value>::BTree(size_t degree): head(nullptr), degree(degree) {

}
template<typename Value>
Result<bool> BTree<Value>::insert(DataShared<Value> data) {

  // If head is null.
  if (head == nullptr){
    head = Node<Value>::make_node(degree);
    head->push_back_data(data);
  }

  // Else, find the leaf node where the new data will insert.
  else{
    Pointer<Value> current_node = head;
    while (!current_node->is_leaf()){
      auto index = current_node->search(data).unwrap();
      current_node = current_node->get_pointer(index).unwrap();
    }
    auto index = current_node->search(data).unwrap();
    current_node->insert_data(index, data).unwrap();

    if (current_node->get_pointer_count() <= degree){
      return Ok(true);
    } else {
      auto cur = current_node.get();
      do {
        auto sub_root = split(current_node).unwrap();
        current_node->clear();
        current_node->push_back_data(sub_root.data).unwrap();
        current_node->set_pointer(0, sub_root.l_child);
        current_node->set_pointer(1, sub_root.r_child);
        current_node->make_no_leaf();
        if (current_node->get_parent() != nullptr){
          //merge with parent.
          auto parent = cur->get_parent();
          auto p_index = parent->search(cur->get_data(0).unwrap()).unwrap();
          parent->insert_data(p_index, cur->get_data(0).unwrap()).unwrap();
          parent->set_pointer(p_index, cur->get_pointer(0).unwrap()).unwrap();
          parent->set_pointer(p_index + 1, cur->get_pointer(1).unwrap()).unwrap();

          cur = cur->get_parent();
        }
      } while (cur->get_parent() != nullptr);
    }
  }

  return Ok(true);
}
template<typename Value>
Result<SubRoot<Value>> BTree<Value>::split(Pointer<Value> node) {
  int index = node->get_pointer_count() / 2 - 1;
  DataShared<Value> e = node->get_data(index).unwrap();
  Pointer<Value> l_child = Node<Value>::make_node(degree);
  Pointer<Value> r_child = Node<Value>::make_node(degree);

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
