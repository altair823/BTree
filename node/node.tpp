
#include "node.h"

template<typename Value>
Node<Value>::Node(size_t degree)
  : parent(nullptr), max_pointer_count(degree), max_data_count(degree - 1), is_node_leaf(LEAF) {
  node_num = node_count;
  node_count++;
  pointers.push_back(nullptr);
}

template<typename Value>
Pointer<Value> Node<Value>::get_pointer(int index) {
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
Result<bool> Node<Value>::insert_pointer(int index, Pointer<Value> pointer) {
  if (pointers.size() <= index) {
    return Err(false, "Pointer vector index out of bound error!");
  } else {
    pointers.insert(pointers.begin() + index, pointer);
    if (pointer != nullptr){
      pointer->set_parent(this);
    }
    return Ok(true);
  }
}

template<typename Value>
Result<bool> Node<Value>::push_back_data(DataShared<Value> new_data) {
  if (data.size() == max_data_count) {
    return Err(false, "The data vector is full!");
  } else {
    data.push_back(new_data);
    pointers.push_back(nullptr);
    return Ok(true);
  }
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
Pointer<Value> Node<Value>::make_node(size_t degree) {
  return std::make_shared<Node<Value>>(degree);
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
