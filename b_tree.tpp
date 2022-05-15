

#include "b_tree.h"

template<typename Value>
BTree<Value>::BTree(size_t degree): head(nullptr), degree(degree), depth(1) {

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
      auto index = current_node->search(data->get_key()).unwrap();
      if (index < current_node->get_data_count() && current_node->get_data(index)->get_key() == data->get_key()) {
        current_node->set_data(index, data);
        return Ok(true);
      }
      current_node = current_node->get_pointer(index).get();
    }
    auto index = current_node->search(data->get_key()).unwrap();
    if (index < current_node->get_data_count() && current_node->get_data(index)->get_key() == data->get_key()) {
      current_node->set_data(index, data);
      return Ok(true);
    }
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
          auto p_index = parent->search(current_node->get_data(0)->get_key()).unwrap();
          parent->insert_data(p_index, current_node->get_data(0)).unwrap();
          parent->set_pointer(p_index, current_node->get_pointer(0)).unwrap();
          parent->set_pointer(p_index + 1, current_node->get_pointer(1)).unwrap();

          current_node = current_node->get_parent();
        } else {
          depth++;
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
  int index = node->get_pointer_count() / 2 - 1;
  DataShared<Value> e = node->get_data(index);
  Pointer<Value> l_child = Node<Value>::make_node(degree);
  l_child->set_leaf(is_leaf);
  Pointer<Value> r_child = Node<Value>::make_node(degree);
  r_child->set_leaf(is_leaf);

  for (int i = 0; i < index; i++){
    l_child->insert_data(i, node->get_data(i)).unwrap();
  }
  for (int i = 0; i <= index; i++){
    l_child->set_pointer(i, node->get_pointer(i)).unwrap();
  }

  for (int i = index + 1; i < node->get_pointer_count() - 1; i++){
    r_child->insert_data(i - index - 1, node->get_data(i)).unwrap();
  }
  for (int i = index + 1; i < node->get_pointer_count(); i++){
    r_child->set_pointer(i - index - 1, node->get_pointer(i)).unwrap();
  }


  auto sub_root = SubRoot<Value>();
  sub_root.data = e;
  sub_root.l_child = l_child;
  sub_root.r_child = r_child;
  return Ok(sub_root);
}
template<typename Value>
Result<bool> BTree<Value>::remove(Key key) {
  if (head == nullptr){
    return Ok(false);
  }
  else{
    Node<Value>* current_node = head;
    while (!current_node->is_leaf()){
      auto index = current_node->search(key).unwrap();
      if (index < current_node->get_data_count() && current_node->get_data(index)->get_key() == key) {
        // found the key in branch node.
        auto deleted_node = remove_in_branch(current_node, index);
        current_node = deleted_node.node;
        remove_in_leaf(deleted_node.node);
        break;
      }
      current_node = current_node->get_pointer(index).get();
    }
    auto index = current_node->search(key).unwrap();
    if (index < current_node->get_data_count() && current_node->get_data(index)->get_key() == key) {
      // found the key in leaf node.
      remove_in_leaf(current_node);
    }
    // not found.
    return Ok(false);
  }
}
template<typename Value>
DeletedNode<Value> BTree<Value>::remove_in_branch(Node<Value> *node, int index) {
  auto current_node = node;
  while (!current_node->is_leaf()) {
    {
      auto smaller_node = current_node->get_pointer(index).get();
      auto biggest_index = smaller_node->get_data_count() - 1;
      if (smaller_node != nullptr) {
        auto to_up = smaller_node->get_data(biggest_index);
        current_node->set_data(index, to_up);
        smaller_node->set_data(biggest_index, nullptr);
        if (!smaller_node->is_leaf()) {
          return remove_in_branch(smaller_node, biggest_index);
        } else{
          smaller_node->erase_data(biggest_index);
          smaller_node->erase_pointer(biggest_index + 1);
          return {smaller_node, biggest_index};
        }
      }
    }
    {
      auto bigger_node = current_node->get_pointer(index + 1).get();
      auto smallest_index = 0;
      if (bigger_node != nullptr) {
        auto to_up = bigger_node->get_data(smallest_index);
        current_node->set_data(index, to_up);
        bigger_node->set_data(smallest_index, nullptr);
        if (!bigger_node->is_leaf()) {
          return remove_in_branch(bigger_node, smallest_index);
        } else{
          bigger_node->erase_data(smallest_index);
          bigger_node->erase_pointer(smallest_index);
          return {bigger_node, smallest_index};
        }
      }
    }
  }
}
template<typename Value>
void BTree<Value>::remove_in_leaf(Node<Value> *node) {
  switch (degree % 2) {
    case 0:
      if (node->get_data_count() >= degree / 2 - 1) {
        return;
      }
    case 1:
      if (node->get_data_count() >= degree / 2){
        return;
      }
  }
  auto parent = node->get_parent();
  auto i = parent->search_node(node).unwrap();
  // if the node is in the very left.
  if (i == 0){

  }
}
template<typename Value>
void BTree<Value>::spin_clockwise(Node<Value> *parent, int target_node_index) {

}
template<typename Value>
void BTree<Value>::spin_counterclockwise(Node<Value> *parent, int target_node_index) {
  auto p_data = parent->get_data(target_node_index);
  parent->get_pointer(target_node_index)->push_back_data(p_data).unwrap();
  auto s_data = parent->get_pointer(target_node_index + 1)->get_data(0);
  parent->set_data(target_node_index, s_data);
  parent->get_pointer(target_node_index + 1)->erase_data(0);
}
template<typename Value>
void print_node(Pointer<Value> node, size_t depth) {
  if (node == nullptr){
    return;
  }
  using namespace std;
  cout << "Level: " << depth << endl;
  cout << "key: ";
  for (int i = 0; i < node->get_data_count(); i++){
     cout << node->get_data(i)->get_key() << "  ";
  }
  cout << endl;
  cout << endl;
  depth++;
  for (int i = 0; i < node->get_pointer_count() && node->is_leaf() == NO_LEAF; i++){
    print_node(node->get_pointer(i), depth);
  }
}
