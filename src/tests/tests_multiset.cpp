#include <gtest/gtest.h>

#include "../s21_containersplus.h"

namespace s21 {

TEST(MultisetTest, ConstructorDefault) {
  multiset<int> ms;
  EXPECT_EQ(ms.size(), 0);
  EXPECT_EQ(ms.begin(), ms.end());
}

TEST(MultisetTest, ConstructorInitializerList) {
  multiset<int> ms = {1, 2, 3, 3, 2, 1};
  EXPECT_EQ(ms.size(), 6);
  EXPECT_EQ(ms.count(1), 2);
  EXPECT_EQ(ms.count(2), 2);
  EXPECT_EQ(ms.count(3), 2);
  EXPECT_EQ(ms.count(4), 0);
}

TEST(MultisetTest, CopyConstructor) {
  multiset<int> original = {1, 2, 3};
  multiset<int> copy(original);
  EXPECT_EQ(copy.size(), original.size());
  EXPECT_EQ(copy.count(1), 1);
  EXPECT_EQ(copy.count(2), 1);
  EXPECT_EQ(copy.count(3), 1);
}

TEST(MultisetTest, MoveConstructor) {
  multiset<int> original = {1, 2, 3};
  multiset<int> moved(std::move(original));
  EXPECT_EQ(moved.size(), 3);
  EXPECT_EQ(moved.count(1), 1);
  EXPECT_EQ(moved.count(2), 1);
  EXPECT_EQ(moved.count(3), 1);
  EXPECT_EQ(original.size(), 0);
}

TEST(MultisetTest, Insert) {
  multiset<int> ms;
  ms.insert(5);
  ms.insert(5);
  ms.insert(3);
  EXPECT_EQ(ms.size(), 3);
  EXPECT_EQ(ms.count(5), 2);
  EXPECT_EQ(ms.count(3), 1);
  EXPECT_EQ(ms.count(4), 0);
}

TEST(MultisetTest, Find) {
  multiset<int> ms = {1, 2, 3, 3};
  auto it = ms.find(3);
  ASSERT_NE(it, ms.end());
  EXPECT_EQ(*it, 3);

  it = ms.find(4);
  EXPECT_EQ(it, ms.end());
}

TEST(MultisetTest, Count) {
  multiset<int> ms = {1, 2, 3, 3, 3};
  EXPECT_EQ(ms.count(1), 1);
  EXPECT_EQ(ms.count(2), 1);
  EXPECT_EQ(ms.count(3), 3);
  EXPECT_EQ(ms.count(4), 0);
}

TEST(MultisetTest, EqualRange) {
  multiset<int> ms = {1, 2, 2, 3, 3, 3};
  auto range = ms.equal_range(3);
  EXPECT_NE(range.first, ms.end());
  EXPECT_EQ(*range.first, 3);

  int count = 0;
  for (auto it = range.first; it != range.second; it++) {
    EXPECT_EQ(*it, 3);
    ++count;
  }
  EXPECT_EQ(count, 3);
}

TEST(MultisetTest, LowerBound) {
  multiset<int> ms = {1, 2, 3, 3, 5};
  auto it = ms.lower_bound(3);
  EXPECT_NE(it, ms.end());
  EXPECT_EQ(*it, 3);

  it = ms.lower_bound(5);
  EXPECT_NE(it, ms.end());
  EXPECT_EQ(*it, 5);

  it = ms.lower_bound(6);
  EXPECT_EQ(it, ms.end());
}

TEST(MultisetTest, UpperBound) {
  multiset<int> ms = {1, 2, 3, 3, 5};
  auto it = ms.upper_bound(3);
  EXPECT_NE(it, ms.end());
  EXPECT_EQ(*it, 5);

  it = ms.upper_bound(5);
  EXPECT_EQ(it, ms.end());

  it = ms.upper_bound(2);
  EXPECT_NE(it, ms.end());
  EXPECT_EQ(*it, 3);
}

TEST(MultisetTest, InsertAndIterate) {
  multiset<int> ms;
  ms.insert(10);
  ms.insert(20);
  ms.insert(20);
  ms.insert(30);

  std::vector<int> result;
  for (auto it = ms.begin(); it != ms.end(); ++it) {
    result.push_back(*it);
  }

  std::vector<int> expected = {10, 20, 20, 30};
  EXPECT_EQ(result, expected);
}

TEST(MultisetTest, Swap) {
  s21::multiset<int> ms1 = {1, 2, 2, 3};
  s21::multiset<int> ms2 = {4, 5, 5, 6};

  ms1.swap(ms2);
  EXPECT_EQ(ms1.size(), 4);
  EXPECT_EQ(ms1.count(4), 1);
  EXPECT_EQ(ms1.count(5), 2);
  EXPECT_EQ(ms1.count(6), 1);

  EXPECT_EQ(ms2.size(), 4);
  EXPECT_EQ(ms2.count(1), 1);
  EXPECT_EQ(ms2.count(2), 2);
  EXPECT_EQ(ms2.count(3), 1);
}

TEST(MultisetTest, Merge) {
  s21::multiset<int> ms1 = {1, 2, 2, 3};
  s21::multiset<int> ms2 = {2, 3, 4, 4};

  ms1.merge(ms2);

  EXPECT_EQ(ms1.size(), 8);
  EXPECT_EQ(ms1.count(1), 1);
  EXPECT_EQ(ms1.count(2), 3);
  EXPECT_EQ(ms1.count(3), 2);
  EXPECT_EQ(ms1.count(4), 2);
  EXPECT_TRUE(ms2.empty());
  EXPECT_EQ(ms2.size(), 0);
}

TEST(MultisetTest, MergeSelf) {
  s21::multiset<int> ms = {1, 2, 2, 3};

  ms.merge(ms);

  EXPECT_EQ(ms.size(), 4);
  EXPECT_EQ(ms.count(1), 1);
  EXPECT_EQ(ms.count(2), 2);
  EXPECT_EQ(ms.count(3), 1);
}

TEST(MultisetTest, Clear) {
  s21::multiset<int> ms = {1, 2, 2, 3, 4, 4, 4};

  ms.clear();

  EXPECT_TRUE(ms.empty());
  EXPECT_EQ(ms.size(), 0);

  EXPECT_EQ(ms.count(1), 0);
  EXPECT_EQ(ms.count(2), 0);
  EXPECT_EQ(ms.count(3), 0);
  EXPECT_EQ(ms.count(4), 0);
}

TEST(MultisetTest, SwapWithEmpty) {
  s21::multiset<int> ms1 = {1, 2, 2, 3};
  s21::multiset<int> ms2;

  ms1.swap(ms2);

  EXPECT_TRUE(ms1.empty());
  EXPECT_EQ(ms1.size(), 0);

  EXPECT_EQ(ms2.size(), 4);
  EXPECT_EQ(ms2.count(1), 1);
  EXPECT_EQ(ms2.count(2), 2);
  EXPECT_EQ(ms2.count(3), 1);
}

TEST(MultisetTest, MergeWithEmpty) {
  s21::multiset<int> ms1 = {1, 2, 2, 3};
  s21::multiset<int> ms2;

  ms1.merge(ms2);

  EXPECT_EQ(ms1.size(), 4);
  EXPECT_EQ(ms1.count(1), 1);
  EXPECT_EQ(ms1.count(2), 2);
  EXPECT_EQ(ms1.count(3), 1);

  EXPECT_TRUE(ms2.empty());
}

TEST(MultisetTest, InsertManyTest) {
  multiset<int> ms;
  auto results = ms.insert_many(1, 2, 3, 2, 1, 4);

  EXPECT_EQ(results.size(), 6);

  std::vector<int> expected = {1, 1, 2, 2, 3, 4};
  int idx = 0;
  for (const auto& elem : ms) {
    EXPECT_EQ(elem, expected[idx++]);
  }
}

TEST(MultisetTest, Contains) {
  s21::multiset<int> ms = {1, 2, 2, 3, 4, 4, 4};

  EXPECT_TRUE(ms.contains(1));
  EXPECT_TRUE(ms.contains(2));
  EXPECT_TRUE(ms.contains(3));
  EXPECT_TRUE(ms.contains(4));

  EXPECT_FALSE(ms.contains(0));
  EXPECT_FALSE(ms.contains(5));
}

TEST(MultisetTest, IteratorDecrement) {
  s21::multiset<int> ms = {1, 2, 2, 3, 4, 4, 4};

  auto it = ms.end();
  --it;

  EXPECT_EQ(*it, 4);

  --it;
  EXPECT_EQ(*it, 4);
  --it;
  EXPECT_EQ(*it, 4);
  --it;
  EXPECT_EQ(*it, 3);
  --it;
  EXPECT_EQ(*it, 2);
  --it;
  EXPECT_EQ(*it, 2);
  --it;
  EXPECT_EQ(*it, 1);

  EXPECT_THROW(--it, std::out_of_range);
}

TEST(MultisetTest, IteratorPostDecrement) {
  s21::multiset<int> ms = {1, 2, 2, 3};

  auto it = ms.end();
  it--;

  EXPECT_EQ(*it, 3);

  auto old_it = it--;
  EXPECT_EQ(*old_it, 3);
  EXPECT_EQ(*it, 2);
}

TEST(MultisetTest, MaxSize) {
  s21::multiset<int> ms;

  EXPECT_EQ(ms.max_size(), std::numeric_limits<size_t>::max());
}

TEST(MultisetTest, Erase) {
  s21::multiset<int> ms = {1, 2, 2, 3, 4, 4, 4};

  auto it = ms.find(3);
  ms.erase(it);
  EXPECT_EQ(ms.size(), 6);
  EXPECT_FALSE(ms.contains(3));

  it = ms.find(2);
  ms.erase(it);
  EXPECT_EQ(ms.size(), 5);
  EXPECT_EQ(ms.count(2), 1);

  it = ms.find(4);
  ms.erase(it);
  EXPECT_EQ(ms.size(), 4);
  EXPECT_EQ(ms.count(4), 2);

  it = ms.find(5);
  EXPECT_THROW(ms.erase(it), std::out_of_range);
}

TEST(MultisetTest, MoveAssignmentOperator) {
  s21::multiset<int> ms1 = {1, 2, 2, 3};
  s21::multiset<int> ms2;

  ms2 = std::move(ms1);

  EXPECT_EQ(ms2.size(), 4);
  EXPECT_TRUE(ms2.contains(1));
  EXPECT_TRUE(ms2.contains(2));
  EXPECT_TRUE(ms2.contains(3));
}

TEST(MultisetTest, SelfMoveAssignment) {
  s21::multiset<int> ms = {1, 2, 2, 3};

  ms = std::move(ms);

  EXPECT_EQ(ms.size(), 4);
  EXPECT_TRUE(ms.contains(1));
  EXPECT_TRUE(ms.contains(2));
  EXPECT_TRUE(ms.contains(3));
}

}  // namespace s21
