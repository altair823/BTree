

#include "b_tree.h"

template<typename Value>
BTree<Value>::BTree(size_t degree): head(nullptr), degree(degree) {

}
template<typename Value>
Result<bool> BTree<Value>::insert(DataShared<Value> data) {

  // If head is null.
  if (head == nullptr){
    head = std::make_shared<Node<Value>>(degree);
    head->push_back_data(data);
  }

  // Else, find the leaf node where the new data will insert.
  else{
    Pointer<Value> previous_node = nullptr;
    Pointer<Value> current_node = head;
    while (current_node != nullptr){
      previous_node = current_node;
      auto index = current_node->search(data).unwrap();
      current_node = current_node->get_pointer(index).unwrap();
    }
    auto index = previous_node->search(data).unwrap();
    previous_node->insert_data(index, data);
  }

  return Ok(true);
}
