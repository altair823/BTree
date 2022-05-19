
#include "node.h"

template<typename Value>
Node<Value>::Node()
  : parent(nullptr), is_node_leaf(LEAF) {
  node_num = node_count;
  node_count++;
  pointers.push_back(nullptr);
}

template<typename Value>
Pointer<Value> Node<Value>::get_pointer(int index) const {
  if (pointers.empty() || pointers.size() <= index || index < 0){
    return nullptr;
  }
  return this->pointers[index];
}

template<typename Value>
DataShared<Value> Node<Value>::get_data(int index) {
  if (data.empty() || data.size() <= index || index < 0){
    return nullptr;
  }
  return this->data[index];
}

template<typename Value>
Result<bool> Node<Value>::insert_data(int index, DataShared<Value> new_data) {\
  data.insert(data.begin() + index, new_data);
  pointers.insert(pointers.begin() + index, nullptr);
  return Ok(true);
}

template<typename Value>
Result<bool> Node<Value>::push_back(DataShared<Value> new_data, Pointer<Value> pointer) {
  data.push_back(new_data);
  pointers.push_back(pointer);
  if (pointer != nullptr){
    pointer->set_parent(this);
  }
  return Ok(true);
}

template<typename Value>
Result<long> Node<Value>::search(Key key) {
  auto temp_data = std::make_shared<Data<Value>>(key, 0);
  auto p = std::lower_bound(data.begin(), data.end(), temp_data,
                            less_data_shared<Value>);
  auto index = std::distance(data.begin(), p);
  temp_data.reset();
  return Ok(index);
}

template<typename Value>
Result<bool> Node<Value>::set_pointer(int index, Pointer<Value> pointer) {
    pointers[index] = pointer;
    if (pointer != nullptr){
      pointer->set_parent(this);
    }
    return Ok(true);
}
template<typename Value>
Pointer<Value> Node<Value>::make_node() {
  return std::make_shared<Node<Value>>();
}
template<typename Value>
void Node<Value>::clear() {
  data.clear();
  pointers.clear();
  pointers.push_back(nullptr);
  is_node_leaf = NO_LEAF;
}
template<typename Value>
void Node<Value>::set_parent(Node<Value>* new_parent) {
  this->parent = new_parent;
}
template<typename Value>
void Node<Value>::set_data(int index, DataShared<Value> new_data) {
  data[index] = new_data;
}
template<typename Value>
void Node<Value>::erase_data(int index) {
  if (index < 0 || index >= data.size()){
    std::cout<<"index out of bound!";
    return;
  }
  data.erase(data.begin() + index);
}
template<typename Value>
void Node<Value>::erase_pointer(int index) {
  if (index < 0 || index >= pointers.size()){
    std::cout<<"index out of bound!";
    return;
  }
  pointers.erase(pointers.begin() + index);
}
template<typename Value>
Result<int> Node<Value>::search_node(Node<Value> *node) {
  for (int i = 0; i < pointers.size(); i++){
    if (pointers[i].get() == node){
      return Ok(i);
    }
  }
  return Err(-1, "Cannot found the node #" + std::to_string(node->get_node_num()));
}
template<typename Value>
void Node<Value>::from(const Node<Value> *node) {
  //node_num = node->node_num;
  parent = node->parent;
  pointers.assign(node->pointers.begin(), node->pointers.end());
  if (!node->is_node_leaf) {
    for (int i = 0; i < node->get_pointer_count(); i++) {
      node->get_pointer(i)->set_parent(this);
    }
  }
  data.assign(node->data.begin(), node->data.end());
  is_node_leaf = node->is_node_leaf;
}
