

#include "b_tree.h"

template<typename Value>
BTree<Value>::BTree(size_t degree): head(nullptr), degree(degree), depth(1), min_data_count((size_t)ceil((double)degree / 2) - 1) {
}
template<typename Value>
Result<bool> BTree<Value>::insert(DataShared<Value> data) {

  // If head is null.
  if (head == nullptr){
    head = Node<Value>::make_node();
    head->push_back(data, nullptr);
  }

  // Else, find the leaf node where the new data will insert.
  else{
    Node<Value>* current_node = head.get();
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
        if (current_node != head.get()){
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
  Pointer<Value> l_child = Node<Value>::make_node();
  l_child->set_leaf(is_leaf);
  Pointer<Value> r_child = Node<Value>::make_node();
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
    Node<Value>* current_node = head.get();
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
    if (current_node == head.get() && current_node->get_data_count() == 0){
      head.reset();
    }
    // not found.
    return Ok(false);
  }
}
template<typename Value>
Result<Node<Value>*> BTree<Value>::remove_in_branch(Node<Value> *node, int index) {
  auto current_node = node;
  while (!current_node->is_leaf()) {

    if (current_node->get_pointer(index) != nullptr) {
      auto smaller_node = current_node->get_pointer(index).get();
      while (!smaller_node->is_leaf()) {
        smaller_node = smaller_node->get_pointer(smaller_node->get_pointer_count() - 1).get();
      }
      auto biggest_data = smaller_node->get_data(smaller_node->get_data_count() - 1);
      current_node->set_data(index, biggest_data);
      smaller_node->erase_data(smaller_node->get_data_count() - 1);
      smaller_node->erase_pointer(smaller_node->get_pointer_count() - 1);
      return Ok(smaller_node);
    }

    if (current_node->get_pointer(index + 1) != nullptr) {
      auto bigger_node = current_node->get_pointer(index + 1).get();
      while (!bigger_node->is_leaf()) {
        bigger_node = bigger_node->get_pointer(0).get();
      }
      auto smallest_data = bigger_node->get_data(0);
      current_node->set_data(index, smallest_data);
      bigger_node->erase_data(0);
      bigger_node->erase_pointer(0);
      return Ok(bigger_node);
    }
  }
    return Err(static_cast<Node<Value> *>(nullptr), "Cannot delete the node in branch!");
}
template<typename Value>
void BTree<Value>::solve(Node<Value> *node) {
  if (node->get_data_count() >= min_data_count || (node == head.get() && node->is_leaf())){
    return;
  }
  auto parent = node->get_parent();
  auto target_node_index = parent->search_node(node).unwrap();

  if (node->get_data_count() < min_data_count) {
    switch (BTree<Value>::how_to_solve(parent, target_node_index)) {
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
  if (parent != head.get() && parent->get_data_count() < min_data_count){
    solve(parent);
  }
  else if (parent == head.get() && parent->get_data_count() == 0){
    node->set_parent(nullptr);
    auto new_head = Node<Value>::make_node();
    new_head->from(node);
    head = new_head;
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
  return DeletionType::None;
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
Result<Value> BTree<Value>::search(Key key) {
  auto current_node = head;
  do {
    auto index = current_node->search(key).unwrap();
    if (index < current_node->get_data_count() && current_node->get_data(index)->get_key() == key){
      return Ok(current_node->get_data(index)->get_value());
    } else{
      current_node = current_node->get_pointer(index);
    }
  }while (current_node != nullptr);
  return Err(-1, "Cannot found the key.");
}
template<typename Value>
bool BTree<Value>::is_empty() {
  if (head == nullptr){
    return true;
  } else{
    return false;
  }
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
