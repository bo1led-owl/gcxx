#include "avl.hpp"
#include "gtest/gtest.h"
#include <cstdlib>
#include <gtest/gtest.h>

TEST(AvlTest, LinearInsertion) {
  AVL a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(90 - 1 - i));
  }
}

TEST(AvlTest, ReversedLinearInsertion) {
  AVL a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
}

TEST(AvlTest, LinearReinsertion) {
  AVL a;
  ASSERT_TRUE(a.insert(10));
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.insert(10));
  }
}

TEST(AvlTest, LinearDeletion) {
  AVL a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.remove(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(i));
  }
}

TEST(AvlTest, ReversedLinearDeletion) {
  AVL a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.remove(90 - 1 - i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(90 - 1 - i));
  }
}

TEST(AvlTest, LinearFind) {
  AVL a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_EQ(a.find(i), i);
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.remove(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_EQ(a.find(i), INT_MIN);
  }
}

TEST(AvlTest, MinMaxSingleElement) {
  AVL a;
  a.insert(1);
  EXPECT_EQ(a.min(), 1);
  EXPECT_EQ(a.max(), 1);
}

TEST(AvlTest, MinMaxAfterOperations) {
  AVL a;
  a.insert(5);
  a.insert(3);
  a.insert(7);
  a.insert(2);
  a.insert(4);
  EXPECT_EQ(a.min(), 2);
  EXPECT_EQ(a.max(), 7);
  a.remove(2);
  EXPECT_EQ(a.min(), 3);
  a.remove(7);
  EXPECT_EQ(a.max(), 5);
}

TEST(AvlTest, SelectOnEmpty) {
  AVL a;
  EXPECT_EQ(a.select(1), INT_MIN);
  EXPECT_EQ(a.select(2), INT_MIN);
}

TEST(AvlTest, SelectSingle) {
  AVL a;
  a.insert(1);
  EXPECT_EQ(a.select(0), 1);
  EXPECT_EQ(a.select(2), INT_MIN);
}

TEST(AvlTest, RankOnEmpty) {
  AVL a;
  EXPECT_EQ(a.rank(1), 0);
}

TEST(AvlTest, RankSingle) {
  AVL a;
  a.insert(1);
  EXPECT_EQ(a.rank(0), 0);
  EXPECT_EQ(a.rank(1), 0);
  EXPECT_EQ(a.rank(2), 1);
}

TEST(AvlTest, RankMultiple) {
  AVL a;
  std::vector<int> vals = {5, 3, 8, 1, 4, 7, 9};
  for (int v : vals) {
    a.insert(v);
  }
  EXPECT_EQ(a.rank(0), 0);
  EXPECT_EQ(a.rank(1), 0);
  EXPECT_EQ(a.rank(2), 1);
  EXPECT_EQ(a.rank(3), 1);
  EXPECT_EQ(a.rank(4), 2);
  EXPECT_EQ(a.rank(5), 3);
  EXPECT_EQ(a.rank(6), 4);
  EXPECT_EQ(a.rank(7), 4);
  EXPECT_EQ(a.rank(8), 5);
  EXPECT_EQ(a.rank(9), 6);
  EXPECT_EQ(a.rank(10), 7);
}

TEST(AvlTest, SetFuzz) {
  AVL a;
  std::set<int> s;
  std::srand(std::time(0));

  for (int i = 0; i < 2048; ++i) {
    int op = rand() % 3;
    int val = rand() % 90;
    if (op == 0) {
      bool inserted = a.insert(val);
      EXPECT_EQ(inserted, s.insert(val).second);
    } else if (op == 1) {
      bool removed = a.remove(val);
      EXPECT_EQ(removed, s.erase(val) == 1);
    } else {
      int found = a.find(val);
      auto it = s.find(val);
      if (it != s.end())
        EXPECT_EQ(found, val);
      else
        EXPECT_EQ(found, INT_MIN);
    }
    EXPECT_TRUE(a.verify());
  }
}
