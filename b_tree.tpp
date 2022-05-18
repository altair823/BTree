

#include "b_tree.h"

template<typename Value>
BTree<Value>::BTree(size_t degree): head(nullptr), degree(degree), depth(1), min_data_count((size_t)ceil((double)degree / 2) - 1) {
}
template<typename Value>
Result<bool> BTree<Value>::insert(DataShared<Value> data) {

  // If head is null.
  if (head == nullptr){
    head = new Node<Value>(degree);
    head->push_back(data, nullptr);
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
        current_node->push_back(sub_root.data).unwrap();
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
        auto deleted_node = remove_in_branch(current_node, index).unwrap();
        current_node = deleted_node;
        solve(current_node);
        break;
      }
      current_node = current_node->get_pointer(index).get();
    }
    auto index = current_node->search(key).unwrap();
    if (index < current_node->get_data_count() && current_node->get_data(index)->get_key() == key) {
      // found the key in leaf node.
      current_node->erase_data(index);
      current_node->erase_pointer(index);
      solve(current_node);
    }
    // not found.
    return Ok(false);
  }
}
template<typename Value>
Result<Node<Value>*> BTree<Value>::remove_in_branch(Node<Value> *node, int index) {
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
          return Ok(smaller_node);
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
          return Ok(bigger_node);
        }
      }
    }
  }
  return Err(static_cast<Node<Value>*>(nullptr), "Cannot delete the node in branch!");
}
template<typename Value>
void BTree<Value>::solve(Node<Value> *node) {
  if (node->get_data_count() >= min_data_count){
    return;
  }
  auto parent = node->get_parent();
  auto target_node_index = parent->search_node(node).unwrap();

  if (node->get_data_count() < min_data_count) {
    switch (how_to_solve(parent, target_node_index)) {
      case DeletionType::CounterClockwiseSpin:
        spin_counterclockwise(parent, target_node_index);
        break;
      case DeletionType::ClockwiseSpin:
        spin_clockwise(parent, target_node_index);
        break;
      case DeletionType::MergeLeft: // Merge may cause lack of pointers in parent.
        merge_with_left(parent, target_node_index);
        break;
      case DeletionType::MergeRight:
        merge_with_right(parent, target_node_index);
        break;
      default:assert("wrong type!");
    }
  }
  if (parent != head && parent->get_data_count() < min_data_count){
    solve(parent);
  }
}
template<typename Value>
DeletionType BTree<Value>::how_to_solve(Node<Value> *parent, int target_node_index) {
  if (target_node_index != parent->get_pointer_count() - 1 && parent->get_pointer(target_node_index + 1)->get_data_count() >= min_data_count + 1) {
    return DeletionType::CounterClockwiseSpin;
  } else if (target_node_index != 0 && parent->get_pointer(target_node_index - 1)->get_data_count() >= min_data_count + 1) {
    return DeletionType::ClockwiseSpin;
  } else if (target_node_index != 0 && parent->get_pointer(target_node_index - 1)->get_data_count() <= min_data_count) {
    return DeletionType::MergeLeft;
  } else if (target_node_index != parent->get_pointer_count() - 1 && parent->get_pointer(target_node_index + 1)->get_data_count() <= min_data_count) {
    return DeletionType::MergeRight;
  }
}
template<typename Value>
void BTree<Value>::spin_clockwise(Node<Value> *parent, int target_node_index) {
  auto target = parent->get_pointer(target_node_index).get();
  auto sibling = parent->get_pointer(target_node_index - 1).get();
  auto p_data = parent->get_data(target_node_index - 1);
  auto s_pointer = sibling->get_pointer(sibling->get_pointer_count() - 1);
  target->insert_data(0, p_data).unwrap();
  target->set_pointer(0, s_pointer).unwrap();
  auto s_data = sibling->get_data(sibling->get_data_count() - 1);
  parent->set_data(target_node_index - 1, s_data);
  sibling->erase_data(sibling->get_data_count() - 1);
  sibling->erase_pointer(sibling->get_pointer_count() - 1);
}
template<typename Value>
void BTree<Value>::spin_counterclockwise(Node<Value> *parent, int target_node_index) {
  auto target = parent->get_pointer(target_node_index).get();
  auto sibling = parent->get_pointer(target_node_index + 1).get();
  auto p_data = parent->get_data(target_node_index);
  auto s_pointer = sibling->get_pointer(0);
  target->push_back(p_data, s_pointer).unwrap();
  auto s_data = sibling->get_data(0);
  parent->set_data(target_node_index, s_data);
  sibling->erase_data(0);
  sibling->erase_pointer(0);
}
template<typename Value>
void BTree<Value>::merge_with_right(Node<Value> *parent, int target_node_index) {
  auto target = parent->get_pointer(target_node_index).get();
  auto sibling = parent->get_pointer(target_node_index + 1).get();
  auto p_data = parent->get_data(target_node_index);
  auto data_to_merge = std::vector<DataShared<Value>>();
  auto pointer_to_merge = std::vector<Pointer<Value>>();
  data_to_merge.push_back(p_data);
  for (int i = 0; i < sibling->get_data_count(); i++){
    data_to_merge.push_back(sibling->get_data(i));
    pointer_to_merge.push_back(sibling->get_pointer(i));
  }
  pointer_to_merge.push_back(sibling->get_pointer(sibling->get_pointer_count() - 1));
  for (int i = 0; i < data_to_merge.size(); i++){
    target->push_back(data_to_merge[i], pointer_to_merge[i]).unwrap();
  }
  parent->erase_data(target_node_index);
  parent->erase_pointer(target_node_index + 1);
}
template<typename Value>
void BTree<Value>::merge_with_left(Node<Value> *parent, int target_node_index) {
  auto target = parent->get_pointer(target_node_index).get();
  auto sibling = parent->get_pointer(target_node_index - 1).get();
  auto p_data = parent->get_data(target_node_index - 1);
  auto data_to_merge = std::vector<DataShared<Value>>();
  auto pointer_to_merge = std::vector<Pointer<Value>>();
  for (int i = 0; i < sibling->get_data_count(); i++){
    data_to_merge.push_back(sibling->get_data(i));
    pointer_to_merge.push_back(sibling->get_pointer(i));
  }
  data_to_merge.push_back(p_data);
  pointer_to_merge.push_back(sibling->get_pointer(sibling->get_pointer_count() - 1));
  std::reverse(data_to_merge.begin(), data_to_merge.end());
  std::reverse(pointer_to_merge.begin(), pointer_to_merge.end());
  for (int i = 0; i < data_to_merge.size(); i++){
    target->insert_data(0, data_to_merge[i]);
    target->set_pointer(0, pointer_to_merge[i]);
  }
  parent->erase_data(target_node_index - 1);
  parent->erase_pointer(target_node_index - 1);
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
