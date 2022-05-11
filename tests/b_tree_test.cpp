//
// Created by 김태현 on 2022/05/11.
//

#include <gtest/gtest.h>
#include "../b_tree.h"

TEST(BTreeTest, InitialTest){
  auto b_tree = BTree<int>(5);
  auto data4 = std::make_shared<Data<int>>(7, 7);
  auto data1 = std::make_shared<Data<int>>(3, 3);
  auto data2 = std::make_shared<Data<int>>(4, 4);
  auto data3 = std::make_shared<Data<int>>(2, 2);
  b_tree.insert(data4).unwrap();
  b_tree.insert(data1).unwrap();
  b_tree.insert(data3).unwrap();
  b_tree.insert(data2).unwrap();
  std::cout << b_tree;
}