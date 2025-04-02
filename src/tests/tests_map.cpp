#include <gtest/gtest.h>

#include <map>
#include <stdexcept>
#include <string>

#include "../s21_containers.h"

template <typename Key, typename T>
class CombinedMap {
 private:
  s21::map<Key, T> s21_map;
  std::map<Key, T> std_map;

 public:
  void ensure_equality() {
    EXPECT_EQ(s21_map.size(), std_map.size());

    auto s21_it = s21_map.begin();
    auto std_it = std_map.begin();
    while (s21_it != s21_map.end() && std_it != std_map.end()) {
      EXPECT_EQ(s21_it->first, std_it->first);
      EXPECT_EQ(s21_it->second, std_it->second);
      ++s21_it;
      ++std_it;
    }
  }

  CombinedMap() { ensure_equality(); }

  CombinedMap(std::initializer_list<std::pair<const Key, T>> items)
      : s21_map(items), std_map(items) {
    ensure_equality();
  }

  void insert_s21_map(const Key& key, const T& value) {
    s21_map.insert({key, value});
  }

  void insert_std_map(const Key& key, const T& value) {
    std_map.insert({key, value});
  }

  void clear() {
    s21_map.clear();
    std_map.clear();
    ensure_equality();
  }

  void size() { EXPECT_EQ(s21_map.size(), std_map.size()); }

  void empty() { EXPECT_EQ(s21_map.empty(), std_map.empty()); }

  void swap(CombinedMap<Key, T>& other) {
    s21_map.swap(other.s21_map);
    std_map.swap(other.std_map);
    ensure_equality();
  }

  void merge(CombinedMap<Key, T>& other) {
    s21_map.merge(other.s21_map);
    std_map.merge(other.std_map);
    ensure_equality();
  }
};

TEST(TestsMap, CombinedMapTest) {
  CombinedMap<int, std::string> map;
  CombinedMap<int, std::string> map_to_merge;

  map.insert_s21_map(1, "one");
  map.insert_s21_map(2, "two");
  map.insert_s21_map(3, "three");
  map.insert_std_map(1, "one");
  map.insert_std_map(2, "two");
  map.insert_std_map(3, "three");

  map_to_merge.insert_s21_map(4, "four");
  map_to_merge.insert_s21_map(5, "five");
  map_to_merge.insert_std_map(4, "four");
  map_to_merge.insert_std_map(5, "five");

  map.size();
  map.empty();
  map.clear();
  map.swap(map_to_merge);
  map.merge(map_to_merge);
}

TEST(TestsMap, Empty) {
  s21::map<int, std::string> m;
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

TEST(TestsMap, InsertPair) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});
  m.insert({3, "three"});

  EXPECT_EQ(m.size(), 3);
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
  EXPECT_EQ(m[3], "three");

  m.insert({2, "two_again"});
  EXPECT_EQ(m.size(), 3);
  EXPECT_EQ(m[2], "two");
}

TEST(TestsMap, InsertKeyValue) {
  s21::map<int, std::string> m;

  auto result1 = m.insert(1, "one");

  EXPECT_TRUE(result1.second);
  EXPECT_EQ(result1.first->first, 1);
  EXPECT_EQ(result1.first->second, "one");
  EXPECT_EQ(m.size(), 1);

  auto result2 = m.insert(1, "one_duplicate");

  EXPECT_FALSE(result2.second);
  EXPECT_EQ(result2.first->first, 1);
  EXPECT_EQ(result2.first->second, "one");
  EXPECT_EQ(m.size(), 1);
}

TEST(TestsMap, InsertOrAssign) {
  s21::map<int, std::string> m;

  auto result1 = m.insert_or_assign(1, "one");

  EXPECT_TRUE(result1.second);
  EXPECT_EQ(result1.first->first, 1);
  EXPECT_EQ(result1.first->second, "one");
  EXPECT_EQ(m.size(), 1);

  auto result2 = m.insert_or_assign(1, "one_updated");

  EXPECT_FALSE(result2.second);
  EXPECT_EQ(result2.first->first, 1);
  EXPECT_EQ(result2.first->second, "one_updated");
  EXPECT_EQ(m.size(), 1);
}

TEST(TestsMap, Erase) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});
  m.insert({3, "three"});

  EXPECT_EQ(m.size(), 3);
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
  EXPECT_EQ(m[3], "three");

  auto iter = m.begin();
  iter++;
  m.erase(iter);

  EXPECT_EQ(m.size(), 2);
  EXPECT_FALSE(m.contains(2));
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[3], "three");
}

TEST(TestsMap, IteratorEmptyContainer) {
  s21::map<int, std::string> m;

  auto iter = m.begin();

  EXPECT_THROW(iter++, std::out_of_range);
  EXPECT_THROW(iter--, std::out_of_range);
}

TEST(TestsMap, IteratorDecrementOutOfBounds) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});
  m.insert({3, "three"});

  auto iter = m.begin();

  EXPECT_THROW(iter--, std::out_of_range);
}

TEST(TestsMap, IteratorIncrement) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});
  m.insert({3, "three"});

  auto iter = m.begin();

  EXPECT_EQ((*iter).first, 1);
  EXPECT_EQ((*iter).second, "one");

  iter++;
  EXPECT_EQ((*iter).first, 2);
  EXPECT_EQ((*iter).second, "two");

  iter++;
  EXPECT_EQ((*iter).first, 3);
  EXPECT_EQ((*iter).second, "three");
}

TEST(TestsMap, IteratorDecrement) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});
  m.insert({3, "three"});

  auto iter = m.end();

  iter--;
  EXPECT_EQ((*iter).first, 3);
  EXPECT_EQ((*iter).second, "three");

  iter--;
  EXPECT_EQ((*iter).first, 2);
  EXPECT_EQ((*iter).second, "two");

  iter--;
  EXPECT_EQ((*iter).first, 1);
  EXPECT_EQ((*iter).second, "one");
}

TEST(TestsMap, Iterators) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});
  m.insert({3, "three"});

  auto it = m.begin();
  EXPECT_EQ(it->first, 1);
  EXPECT_EQ(it->second, "one");
  ++it;
  EXPECT_EQ(it->first, 2);
  EXPECT_EQ(it->second, "two");
  ++it;
  EXPECT_EQ(it->first, 3);
  EXPECT_EQ(it->second, "three");
}

TEST(TestsMap, IteratorElementAccess) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});
  m.insert({3, "three"});

  auto iter = m.begin();

  EXPECT_EQ((*iter).first, 1);
  EXPECT_EQ((*iter).second, "one");

  iter++;
  EXPECT_EQ((*iter).first, 2);
  EXPECT_EQ((*iter).second, "two");

  iter++;
  EXPECT_EQ((*iter).first, 3);
  EXPECT_EQ((*iter).second, "three");
}

TEST(TestsMap, IteratorEnd) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});
  m.insert({3, "three"});

  auto iter = m.end();

  iter--;
  EXPECT_EQ((*iter).first, 3);
  EXPECT_EQ((*iter).second, "three");
}

TEST(TestsMap, At) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});
  m.insert({3, "three"});

  EXPECT_EQ(m.at(1), "one");
  EXPECT_EQ(m.at(2), "two");
  EXPECT_EQ(m.at(3), "three");

  EXPECT_THROW(m.at(100), std::out_of_range);
}

TEST(TestsMap, OperatorBracketAccess) {
  s21::map<int, std::string> m;
  m.insert({1, "one"});
  m.insert({2, "two"});

  EXPECT_EQ(m[3], "");
  EXPECT_EQ(m.size(), 3);

  m[1] = "one_modified";
  m[2] = "two_modified";

  EXPECT_EQ(m[1], "one_modified");
  EXPECT_EQ(m[2], "two_modified");
  EXPECT_EQ(m.size(), 3);

  m[4] = "four_new";

  EXPECT_EQ(m[4], "four_new");
  EXPECT_EQ(m.size(), 4);
}

TEST(TestsMap, CopyConstructor) {
  s21::map<int, std::string> m1;
  m1.insert({1, "one"});
  m1.insert({2, "two"});
  m1.insert({3, "three"});

  s21::map<int, std::string> m2(m1);

  EXPECT_EQ(m1.size(), m2.size());
  EXPECT_EQ(m2[1], "one");
  EXPECT_EQ(m2[2], "two");
  EXPECT_EQ(m2[3], "three");
}

TEST(TestsMap, MoveConstructor) {
  s21::map<int, std::string> m1;
  m1.insert({1, "one"});
  m1.insert({2, "two"});
  m1.insert({3, "three"});

  size_t size = m1.size();

  s21::map<int, std::string> m2(std::move(m1));

  EXPECT_EQ(m2.size(), size);
  EXPECT_EQ(m2[1], "one");
  EXPECT_EQ(m2[2], "two");
  EXPECT_EQ(m2[3], "three");

  EXPECT_EQ(m1.size(), 0);
  EXPECT_TRUE(m1.empty());
}

TEST(TestsMap, Swap) {
  s21::map<int, std::string> m1;
  m1.insert({1, "one"});
  m1.insert({2, "two"});

  s21::map<int, std::string> m2;
  m2.insert({3, "three"});
  m2.insert({4, "four"});

  m1.swap(m2);

  EXPECT_EQ(m1.size(), 2);
  EXPECT_EQ(m2.size(), 2);

  EXPECT_EQ(m1[3], "three");
  EXPECT_EQ(m1[4], "four");
  EXPECT_EQ(m2[1], "one");
  EXPECT_EQ(m2[2], "two");
}

TEST(TestsMap, InitializerListConstructor) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};

  EXPECT_EQ(m.size(), 3);

  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
  EXPECT_EQ(m[3], "three");
}

TEST(TestsMap, MoveAssignmentOperator) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}, {3, "three"}};

  s21::map<int, std::string> m2;
  m2 = std::move(m1);

  EXPECT_EQ(m2.size(), 3);
  EXPECT_EQ(m2[1], "one");
  EXPECT_EQ(m2[2], "two");
  EXPECT_EQ(m2[3], "three");

  EXPECT_EQ(m1.size(), 0);
  EXPECT_TRUE(m1.empty());
}

TEST(TestsMap, InsertManyTest) {
  s21::map<int, std::string> m;
  auto results = m.insert_many(
      std::make_pair(1, "One"), std::make_pair(2, "Two"),
      std::make_pair(3, "Three"), std::make_pair(2, "Duplicate"),
      std::make_pair(1, "DuplicateOne"), std::make_pair(4, "Four"));

  EXPECT_EQ(results.size(), 6);

  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_TRUE(results[2].second);
  EXPECT_FALSE(results[3].second);
  EXPECT_FALSE(results[4].second);
  EXPECT_TRUE(results[5].second);

  std::vector<std::pair<int, std::string>> expected = {
      {1, "One"}, {2, "Two"}, {3, "Three"}, {4, "Four"}};

  int idx = 0;
  for (const auto& pair : m) {
    EXPECT_EQ(pair.first, expected[idx].first);
    EXPECT_EQ(pair.second, expected[idx].second);
    idx++;
  }
}