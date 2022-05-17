//
// Created by 김태현 on 2022/05/11.
//

#include <gtest/gtest.h>
#include <random>
#include "../b_tree.h"

TEST(BTreeTest, InsertTest){
  auto b_tree = BTree<int>(5);
  auto data1 = std::make_shared<Data<int>>(3, 3);
  auto data2 = std::make_shared<Data<int>>(4, 4);
  auto data3 = std::make_shared<Data<int>>(2, 2);
  auto data4 = std::make_shared<Data<int>>(7, 7);
  auto data5 = std::make_shared<Data<int>>(12, 12);
  auto data6 = std::make_shared<Data<int>>(13, 13);
  auto data7 = std::make_shared<Data<int>>(6, 6);
  auto data8 = std::make_shared<Data<int>>(8, 8);
  auto data9 = std::make_shared<Data<int>>(18, 18);
  auto data10 = std::make_shared<Data<int>>(20, 10);
  auto data11 = std::make_shared<Data<int>>(21, 10);
  auto data12 = std::make_shared<Data<int>>(22, 10);
  auto data13 = std::make_shared<Data<int>>(19, 10);
  auto data14 = std::make_shared<Data<int>>(14, 10);
  auto data15 = std::make_shared<Data<int>>(15, 10);
  auto data16 = std::make_shared<Data<int>>(11, 10);
  auto data17 = std::make_shared<Data<int>>(23, 10);
  auto data18 = std::make_shared<Data<int>>(27, 10);
  auto data19 = std::make_shared<Data<int>>(24, 10);
  auto data20 = std::make_shared<Data<int>>(30, 10);
  auto data21 = std::make_shared<Data<int>>(31, 10);
  auto data22 = std::make_shared<Data<int>>(28, 10);
  auto data23 = std::make_shared<Data<int>>(25, 10);
  auto data24 = std::make_shared<Data<int>>(29, 10);
  auto data25 = std::make_shared<Data<int>>(26, 10);
  auto data26 = std::make_shared<Data<int>>(26, 21);
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
  b_tree.insert(data11).unwrap();
  b_tree.insert(data12).unwrap();
  b_tree.insert(data13).unwrap();
  b_tree.insert(data14).unwrap();
  b_tree.insert(data15).unwrap();
  b_tree.insert(data16).unwrap();
  b_tree.insert(data17).unwrap();
  b_tree.insert(data18).unwrap();
  b_tree.insert(data19).unwrap();
  b_tree.insert(data20).unwrap();
  b_tree.insert(data21).unwrap();
  b_tree.insert(data22).unwrap();
  b_tree.insert(data23).unwrap();
  b_tree.insert(data24).unwrap();
  b_tree.insert(data25).unwrap();
  b_tree.insert(data26).unwrap();

  b_tree.print();
}

TEST(BTreeTest, RandomInsertTest){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(0, 999999);
  auto b_tree = BTree<int>(20);
  for (int i = 0; i < 100000; i++) {
    int k = dis(gen);
    b_tree.insert(std::make_shared<Data<int>>(k, k));
  }
  b_tree.print();
}

TEST(BTreeTest, DeleteTest){
  int integers[] = {3, 4, 2, 7, 12, 13, 6, 8, 18, 20, 21, 22, 19, 14, 15, 11, 23, 27, 24, 30, 31, 28, 25, 29, 26};
  auto data = std::vector<DataShared<int>>();
  for (auto& i: integers){
    data.push_back(std::make_shared<Data<int>>(i, i));
  }

  auto b_tree = BTree<int>(5);

  for (auto& d: data){
    b_tree.insert(d).unwrap();
  }

  b_tree.remove(29);
  b_tree.remove(30);

  b_tree.print();
}