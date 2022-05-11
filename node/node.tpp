
#include "node.h"

template<typename Value>
Node<Value>::Node(size_t degree)
  : max_pointer_count(degree), max_data_count(degree - 1), is_leaf(LEAF) {
  node_num = node_count;
  node_count++;
  pointers.push_back(nullptr);
}

template<typename Value>
Result<Pointer<Value>> Node<Value>::get_pointer(int index) {
  if (pointers.size() <= index) {
    return Err(static_cast<Pointer<Value>>(nullptr), "Pointer index out of bound error!");
  } else {
    return Ok(this->pointers[index]);
  }
}

template<typename Value>
Result<DataShared<Value>> Node<Value>::get_data(int index) {
  if (data.size() <= index) {
    return Err(static_cast<Pointer<Value>>(nullptr), "Data index out of bound error!");
  } else {
    return Ok(this->data[index]);
  }
}

template<typename Value>
Result<bool> Node<Value>::insert_data(int index, DataShared<Value> new_data) {
  if (max_data_count <= index) {
    return Err(false, "Data index vector out of bound error!");
  }
  data.insert(data.begin() + index, new_data);
  pointers.push_back(nullptr);
  return Ok(true);
}

template<typename Value>
Result<bool> Node<Value>::insert_pointer(int index, Pointer<Value> pointer) {
  if (pointers.size() <= index) {
    return Err(false, "Pointer vector index out of bound error!");
  } else {
    pointers.insert(pointers.begin() + index, pointer);
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
Result<bool> Node<Value>::push_back_pointer(Pointer<Value> pointer) {
  if (pointers.size() == max_pointer_count) {
    return Err(false, "The pointer vector is full!");
  } else {
    pointers.push_back(pointer);
    return Ok(true);
  }
}
template<typename Value>
void Node<Value>::make_leaf() {
  is_leaf = LEAF;
}
template<typename Value>
void Node<Value>::make_no_leaf() {
  is_leaf = NO_LEAF;
}

template<typename Value>
Result<long> Node<Value>::search(DataShared<Value> new_data) {
  auto p = std::lower_bound(data.begin(), data.end(), new_data,
                            less_data_shared<Value>);
  auto index = std::distance(data.begin(), p);
  return Ok(index);
}
template<typename Value>
Result<bool> Node<Value>::set_data(int index, std::shared_ptr<Data<Value>> new_data) {
  if (data.size() <= index){
    return Err(false, "Data vector index out of bound error!");
  } else{
    data[index] = new_data;
    return Ok(true);
  }
}
template<typename Value>
Result<bool> Node<Value>::set_pointer(int index, Pointer<Value> pointer) {
  if (pointers.size() <= index){
    return Err(false, "Pointer vector index out of bound error!");
  } else{
    pointers[index] = pointer;
    return Ok(true);
  }
}

