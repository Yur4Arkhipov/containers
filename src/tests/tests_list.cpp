#include <gtest/gtest.h>

#include <list>
#include <string>
#include <utility>

#include "../s21_containers.h"

#define PREFIX \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

template <typename T>
class CombinedList {
 public:
  s21::list<T> s21_list;
  std::list<T> std_list;

  void ensure_equality() {
    EXPECT_EQ(s21_list.size(), std_list.size());

    auto s21_iter = s21_list.begin();
    auto std_iter = std_list.begin();

    for (int i = 0; i < s21_list.size(); i++) {
      EXPECT_NE(s21_iter, s21_list.end());
      EXPECT_NE(std_iter, std_list.end());
      EXPECT_EQ(*s21_iter, *std_iter);
      ++s21_iter;
      ++std_iter;
    }

    EXPECT_EQ(s21_iter, s21_list.end());
    EXPECT_EQ(std_iter, std_list.end());
  }

  CombinedList() {}

  CombinedList(size_t n) : s21_list(n), std_list(n) { ensure_equality(); }

  CombinedList(std::initializer_list<T> const& items)
      : s21_list(items), std_list(items) {
    ensure_equality();
  }

  void push_back(const T& value) {
    s21_list.push_back(value);
    std_list.push_back(value);
    ensure_equality();
  }

  void push_front(const T& value) {
    s21_list.push_front(value);
    std_list.push_front(value);
    ensure_equality();
  }

  void insert_at(size_t pos, const T& value) {
    auto s21_iter = s21_list.begin();
    auto std_iter = std_list.begin();
    for (int i = 0; i < pos; i++) {
      s21_iter++;
      std_iter++;
    }
    s21_list.insert(s21_iter, value);
    std_list.insert(std_iter, value);
    ensure_equality();
  }

  void erase_at(size_t pos) {
    auto s21_iter = s21_list.begin();
    auto std_iter = std_list.begin();
    for (int i = 0; i < pos; i++) {
      s21_iter++;
      std_iter++;
    }
    s21_list.erase(s21_iter);
    std_list.erase(std_iter);
    ensure_equality();
  }

  void pop_back() {
    s21_list.pop_back();
    std_list.pop_back();
    ensure_equality();
  }

  void pop_front() {
    s21_list.pop_front();
    std_list.pop_front();
    ensure_equality();
  }

  void swap(CombinedList& other) {
    s21_list.swap(other.s21_list);
    std_list.swap(other.std_list);
    ensure_equality();
    other.ensure_equality();
  }

  void merge(CombinedList& other) {
    s21_list.merge(other.s21_list);
    std_list.merge(other.std_list);
    ensure_equality();
    other.ensure_equality();
  }

  void splice_at(size_t pos, CombinedList& other) {
    auto s21_iter = s21_list.begin();
    auto std_iter = std_list.begin();
    for (int i = 0; i < pos; i++) {
      s21_iter++;
      std_iter++;
    }
    s21_list.splice(s21_iter, other.s21_list);
    std_list.splice(std_iter, other.std_list);
    ensure_equality();
    other.ensure_equality();
  }

  void reverse() {
    s21_list.reverse();
    std_list.reverse();
    ensure_equality();
  }

  void unique() {
    s21_list.unique();
    std_list.unique();
    ensure_equality();
  }

  void sort() {
    s21_list.sort();
    std_list.sort();
    ensure_equality();
  }

  void clear() {
    s21_list.clear();
    std_list.clear();
    ensure_equality();
  }

  template <typename... Args>
  void insert_many_at(size_t pos, Args&&... args) {
    auto s21_iter = s21_list.begin();
    auto std_iter = std_list.begin();
    for (int i = 0; i < pos; i++) {
      s21_iter++;
      std_iter++;
    }
    s21_list.insert_many(s21_iter, args...);
    for (const auto& arg : {args...}) {
      std_list.insert(std_iter, arg);
      std_iter++;
    }
    ensure_equality();
  }

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    s21_list.insert_many_back(args...);
    for (const auto& arg : {args...}) {
      std_list.push_back(arg);
    }
  }

  template <typename... Args>
  void insert_many_front(Args&&... args) {
    s21_list.insert_many_front(args...);
    auto std_iter = std_list.begin();
    for (const auto& arg : {args...}) {
      std_list.insert(std_iter, arg);
    }
  }
};

TEST(TestsList, MultipleOperations) {
  std::string prefix(PREFIX);
  CombinedList<std::string> l;

  l.push_back(prefix + "5");
  l.push_back(prefix + "6");
  l.push_back(prefix + "7");
  l.push_back(prefix + "8");

  l.push_front(prefix + "4");
  l.push_front(prefix + "3");
  l.push_front(prefix + "2");
  l.push_front(prefix + "1");

  l.insert_at(2, prefix + "other");
  l.insert_at(5, prefix + "other");
  l.insert_at(4, prefix + "other");
  l.insert_at(1, prefix + "other");
  l.insert_at(0, prefix + "other");
  l.insert_at(13, prefix + "other");

  l.insert_many_at(3, prefix + "other2", prefix + "other3", prefix + "other4");
  l.insert_many_back(prefix + "other2", prefix + "other3", prefix + "other4");
  l.insert_many_front(prefix + "other2", prefix + "other3", prefix + "other4");

  l.erase_at(1);
  l.erase_at(2);
  l.erase_at(3);
  l.erase_at(4);
  l.erase_at(0);
  l.erase_at(8);

  l.pop_back();
  l.pop_front();
  l.pop_front();
  l.pop_back();

  for (int i = 0; i < 50; i++) {
    l.push_back(prefix + std::to_string(i / 4));
  }

  CombinedList l2 = {prefix + "1", prefix + "2"};
  l.swap(l2);
  l.swap(l2);

  for (int i = 0; i < 50; i++) {
    l2.push_front(prefix + std::to_string(i / 3));
  }
  CombinedList l3 = l2;
  l.splice_at(25, l2);

  l.reverse();
  l3.reverse();
  l3.sort();
  l3.unique();
  l.sort();
  l.merge(l3);
  l.clear();
}

TEST(TestsList, SpliceIntoEmpty) {
  std::string prefix(PREFIX);
  CombinedList<std::string> l = {prefix + "Hello", prefix + "world!"};
  CombinedList<std::string> empty;
  empty.splice_at(0, l);
}

TEST(TestsList, ConstructN) { CombinedList<std::string> l(15); }

TEST(TestsList, Move) {
  std::string prefix(PREFIX);
  s21::list<std::string> l = {prefix + "Hello", prefix + "world!"};
  s21::list<std::string> l2 = std::move(l);
  EXPECT_EQ(l.size(), 0);
  EXPECT_EQ(l2.size(), 2);
  EXPECT_EQ(l2.front(), prefix + "Hello");
  EXPECT_EQ(l2.back(), prefix + "world!");
}

TEST(TestsList, MoveConstruct) {
  std::string prefix(PREFIX);
  s21::list<std::string> l = {prefix + "Hello", prefix + "world!"};
  s21::list<std::string> l2 = {prefix + "Other", prefix + "data!"};
  l2 = std::move(l);
  EXPECT_EQ(l.size(), 0);
  EXPECT_EQ(l2.size(), 2);
  EXPECT_EQ(l2.front(), prefix + "Hello");
  EXPECT_EQ(l2.back(), prefix + "world!");
}

TEST(TestsList, CopyConstruct) {
  std::string prefix(PREFIX);
  s21::list<std::string> l = {prefix + "Hello", prefix + "world!"};
  s21::list<std::string> l2 = {prefix + "Other", prefix + "data!"};
  l2 = l;
  EXPECT_EQ(l.size(), 2);
  EXPECT_EQ(l2.size(), 2);
  EXPECT_EQ(l2.front(), prefix + "Hello");
  EXPECT_EQ(l2.back(), prefix + "world!");
}