#include <iostream>
#include "node/node.h"

int main() {
  auto node = Node<int>();
  auto node2 = std::make_shared<Node<int>>();
  auto node3 = std::make_shared<Node<int>>();
  auto data1 = std::make_shared<Data<int>>(4, 4);
  node.insert_data(0, data1).unwrap();
  node.insert_pointer(0, node2).unwrap();
  node.insert_pointer(20, node3).unwrap();
  std::cout << node;
}
