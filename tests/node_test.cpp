//
// Created by 김태현 on 2022/05/11.
//
#include <gtest/gtest.h>
#include "../node/node.h"

TEST(NodeTest, FieldTest){

  auto node = Node<int>(20);
  auto node2 = std::make_shared<Node<int>>(20);
  auto node3 = std::make_shared<Node<int>>(20);
  auto data1 = std::make_shared<Data<int>>(4, 4);
  node.push_back(data1, nullptr).unwrap();
  node.set_pointer(0, node2).unwrap();
  std::cout << node;
}

TEST(NodeTest, SearchTest){
  auto node = Node<int>(20);
  auto node2 = std::make_shared<Node<int>>(20);
  auto node3 = std::make_shared<Node<int>>(20);
  auto data1 = std::make_shared<Data<int>>(4, 4);
  node.push_back(data1, nullptr).unwrap();
  node.set_pointer(0, node2).unwrap();
  node.set_pointer(1, node3).unwrap();
  std::cout << node;
}