#include <gtest/gtest.h>

#include <initializer_list>
#include <set>
#include <string>

#include "../s21_containers.h"

TEST(TestsSet, Empty) {
  s21::set<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(TestsSet, InsertAndSize) {
  s21::set<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(30);

  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 3);
}

TEST(TestsSet, InsertDuplicate) {
  s21::set<int> s;
  s.insert(10);
  s.insert(10);
  EXPECT_EQ(s.size(), 1);
}

TEST(TestsSet, Find) {
  s21::set<int> s;

  for (int i = 1; i <= 10; ++i) {
    s.insert(i);
  }

  for (int i = 1; i <= 10; ++i) {
    auto it = s.find(i);
    EXPECT_NE(it, s.end());
    EXPECT_EQ(*it, i);
  }

  auto it = s.find(100);
  EXPECT_EQ(it, s.end());

  it = s.find(-5);
  EXPECT_EQ(it, s.end());
}

TEST(TestsSet, Swap) {
  s21::set<int> s1;
  s1.insert(10);
  s1.insert(20);

  s21::set<int> s2;
  s2.insert(30);
  s2.insert(40);

  s1.swap(s2);

  EXPECT_EQ(s1.size(), 2);
  EXPECT_EQ(*s1.find(30), 30);
  EXPECT_EQ(*s1.find(40), 40);

  EXPECT_EQ(s2.size(), 2);
  EXPECT_EQ(*s2.find(10), 10);
  EXPECT_EQ(*s2.find(20), 20);
}

TEST(TestsSet, Insert) {
  s21::set<int> s;

  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);

  s.insert(10);
  s.insert(5);
  s.insert(15);

  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 3);

  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(5));
  EXPECT_TRUE(s.contains(15));

  s.insert(10);
  EXPECT_EQ(s.size(), 3);
}

TEST(TestsSet, Erase) {
  s21::set<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(30);

  auto it = s.find(20);
  s.erase(it);

  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(*s.find(10), 10);
  EXPECT_EQ(*s.find(30), 30);
  EXPECT_EQ(s.find(20), s.end());
}

TEST(TestsSet, Clear) {
  s21::set<int> s;

  for (int i = 1; i <= 10; ++i) {
    s.insert(i);
  }

  EXPECT_EQ(s.size(), 10);

  s.clear();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(TestsSet, Iterators) {
  s21::set<int> s;

  for (int i = 1; i <= 5; ++i) {
    s.insert(i);
  }

  int expected_value = 1;
  for (auto it = s.begin(); it != s.end(); ++it) {
    EXPECT_EQ(*it, expected_value);
    ++expected_value;
  }

  auto it = s.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);

  --it;
  EXPECT_EQ(*it, 1);
}

TEST(TestsSet, CopyConstructor) {
  s21::set<int> s1;

  for (int i = 1; i <= 10; ++i) {
    s1.insert(i);
  }

  s21::set<int> s2(s1);

  EXPECT_EQ(s1.size(), s2.size());

  for (int i = 1; i <= 10; ++i) {
    EXPECT_TRUE(s2.contains(i));
  }
}

TEST(TestsSet, CopyAssignment) {
  s21::set<int> s1;

  for (int i = 1; i <= 10; ++i) {
    s1.insert(i);
  }

  s21::set<int> s2 = s1;

  EXPECT_EQ(s1.size(), s2.size());

  for (int i = 1; i <= 10; ++i) {
    EXPECT_TRUE(s2.contains(i));
  }
}

TEST(TestsSet, InitializerListConstructor) {
  s21::set<int> s = {1, 2, 3, 4, 5};

  EXPECT_EQ(s.size(), 5);

  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(2));
  EXPECT_TRUE(s.contains(3));
  EXPECT_TRUE(s.contains(4));
  EXPECT_TRUE(s.contains(5));

  EXPECT_FALSE(s.contains(6));
}

TEST(TestsSet, MoveAssignmentOperator) {
  s21::set<int> s1 = {1, 2, 3, 4, 5};

  s21::set<int> s2;
  s2 = std::move(s1);

  EXPECT_EQ(s2.size(), 5);
  EXPECT_TRUE(s2.contains(1));
  EXPECT_TRUE(s2.contains(2));
  EXPECT_TRUE(s2.contains(3));
  EXPECT_TRUE(s2.contains(4));
  EXPECT_TRUE(s2.contains(5));

  EXPECT_EQ(s1.size(), 0);
  EXPECT_TRUE(s1.empty());

  EXPECT_FALSE(s1.contains(1));
  EXPECT_FALSE(s1.contains(2));
  EXPECT_FALSE(s1.contains(3));
  EXPECT_FALSE(s1.contains(4));
  EXPECT_FALSE(s1.contains(5));
}

TEST(TestsSet, InsertManyTest) {
  s21::set<int> s;
  auto results = s.insert_many(1, 2, 3, 2, 1, 4);

  EXPECT_EQ(results.size(), 6);

  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_TRUE(results[2].second);
  EXPECT_FALSE(results[3].second);
  EXPECT_FALSE(results[4].second);
  EXPECT_TRUE(results[5].second);

  std::vector<int> expected = {1, 2, 3, 4};
  int idx = 0;
  for (const auto& elem : s) {
    EXPECT_EQ(elem, expected[idx++]);
  }
}