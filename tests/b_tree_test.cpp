//
// Created by 김태현 on 2022/05/11.
//

#include <gtest/gtest.h>
#include "../b_tree.h"

TEST(BTreeTest, InsertTest){
  auto b_tree = BTree<int>(6);
  auto data1 = std::make_shared<Data<int>>(3, 3);
  auto data2 = std::make_shared<Data<int>>(4, 4);
  auto data3 = std::make_shared<Data<int>>(2, 2);
  auto data4 = std::make_shared<Data<int>>(7, 7);
  auto data5 = std::make_shared<Data<int>>(12, 12);
  auto data6 = std::make_shared<Data<int>>(13, 13);
  auto data7 = std::make_shared<Data<int>>(6, 6);
  auto data8 = std::make_shared<Data<int>>(8, 8);
  auto data9 = std::make_shared<Data<int>>(18, 18);
  auto data10 = std::make_shared<Data<int>>(10, 10);
  b_tree.insert(data4).unwrap();
  b_tree.insert(data5).unwrap();
  b_tree.insert(data1).unwrap();
  b_tree.insert(data3).unwrap();
  b_tree.insert(data2).unwrap();
  b_tree.insert(data6).unwrap();
  b_tree.insert(data7).unwrap();
  b_tree.insert(data8).unwrap();
  b_tree.insert(data9).unwrap();
  b_tree.insert(data10).unwrap();

  std::cout << b_tree;
}