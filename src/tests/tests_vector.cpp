#include <gtest/gtest.h>

#include <initializer_list>
#include <stdexcept>
#include <string>

#include "../s21_containers.h"

#define PREFIX \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

template <typename T>
class CombinedVector {
 private:
  s21::vector<T> s21_vector;
  std::vector<T> std_vector;

 public:
  void ensure_equality() {
    EXPECT_EQ(s21_vector.size(), std_vector.size());
    EXPECT_EQ(s21_vector.capacity(), std_vector.capacity());

    for (int i = 0; i < s21_vector.size(); i++) {
      EXPECT_EQ(s21_vector[i], std_vector[i]);
    }
  }

  CombinedVector() { ensure_equality(); }

  CombinedVector(size_t n) : s21_vector(n), std_vector(n) { ensure_equality(); }

  CombinedVector(std::initializer_list<T> const& items)
      : s21_vector(items), std_vector(items) {
    ensure_equality();
  }

  void push_back(const T& const_ref) {
    s21_vector.push_back(const_ref);
    std_vector.push_back(const_ref);
    ensure_equality();
  }

  void insert(size_t pos, const T& const_ref) {
    s21_vector.insert(s21_vector.begin() + pos, const_ref);
    std_vector.insert(std_vector.begin() + pos, const_ref);
    ensure_equality();
  }

  void erase(size_t pos) {
    s21_vector.erase(s21_vector.begin() + pos);
    std_vector.erase(std_vector.begin() + pos);
    ensure_equality();
  }

  void pop_back() {
    s21_vector.pop_back();
    std_vector.pop_back();
    ensure_equality();
  }

  void reserve(size_t size) {
    s21_vector.reserve(size);
    std_vector.reserve(size);
    ensure_equality();
  }

  void shrink_to_fit() {
    s21_vector.shrink_to_fit();
    std_vector.shrink_to_fit();
    ensure_equality();
  }

  void front() {
    const T& s21_front = s21_vector.front();
    const T& std_front = std_vector.front();
    EXPECT_EQ(s21_front, std_front);
  }

  void back() {
    const T& s21_back = s21_vector.back();
    const T& std_back = std_vector.back();
    EXPECT_EQ(s21_back, std_back);
  }

  template <typename... Args>
  void insert_many_at(size_t pos, Args&&... args) {
    s21_vector.insert_many(s21_vector.begin() + pos, args...);
    for (const auto& arg : {args...}) {
      std_vector.insert(std_vector.begin() + pos++, arg);
    }
    ensure_equality();
  }

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    s21_vector.insert_many_back(args...);
    for (const auto& arg : {args...}) {
      std_vector.push_back(arg);
    }
    ensure_equality();
  }
};

TEST(TestsVector, MultipleOperations) {
  CombinedVector<std::string> v;
  std::string prefix(PREFIX);
  v.push_back(prefix + "5");
  v.push_back(prefix + "6");
  v.push_back(prefix + "7");
  v.push_back(prefix + "8");

  v.insert(0, prefix + "4");
  v.insert(5, prefix + "9");
  v.front();
  v.back();

  v.erase(1);
  v.erase(2);
  v.erase(2);
  v.pop_back();
  v.pop_back();

  for (int i = 0; i < 20; i++) {
    v.push_back(prefix + std::to_string(i));
  }

  v.reserve(50);

  for (int i = 0; i < 150; i++) {
    v.push_back(prefix + std::to_string(i));
  }

  v.shrink_to_fit();
}

TEST(TestsVector, Empty) {
  s21::vector<int> v;
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(TestsVector, ShrinkToFitEmpty) {
  std::string prefix(PREFIX);
  CombinedVector<std::string> v = {prefix + "my", prefix + "data"};
  v.pop_back();
  v.pop_back();
  v.shrink_to_fit();
}

TEST(TestsVector, InitSize) { CombinedVector<std::string> v(10); }

TEST(TestsVector, InitList) {
  std::string prefix(PREFIX);
  CombinedVector<std::string> v = {prefix + "Hello", prefix + ",",
                                   prefix + "world", prefix + "!"};
}

TEST(TestsVector, Copy) {
  std::string prefix(PREFIX);
  s21::vector<std::string> v1;
  for (int i = 0; i < 20; i++) {
    v1.push_back(prefix + std::to_string(i + 5));
  }
  s21::vector<std::string> v2(v1);
  EXPECT_EQ(v1.size(), v2.size());
  for (int i = 0; i < 20; i++) {
    EXPECT_EQ(v1[i], v2[i]);
  }
}

TEST(TestsVector, Move) {
  std::string prefix(PREFIX);
  s21::vector<std::string> v1;
  for (int i = 0; i < 20; i++) {
    v1.push_back(prefix + std::to_string(i + 5));
  }
  size_t size = v1.size();
  size_t capacity = v1.capacity();
  s21::vector<std::string> v2(std::move(v1));
  EXPECT_EQ(v1.size(), 0);
  EXPECT_EQ(v1.capacity(), 0);
  EXPECT_EQ(v2.size(), size);
  EXPECT_EQ(v2.capacity(), capacity);
  for (int i = 0; i < 20; i++) {
    EXPECT_EQ(v2[i], prefix + std::to_string(i + 5));
  }
}

TEST(TestsVector, MoveAssignment) {
  std::string prefix(PREFIX);
  s21::vector<std::string> v1;
  for (int i = 0; i < 20; i++) {
    v1.push_back(prefix + std::to_string(i + 5));
  }
  size_t size = v1.size();
  size_t capacity = v1.capacity();
  s21::vector<std::string> v2 = {prefix + "old", prefix + "data"};
  v2 = std::move(v1);
  EXPECT_EQ(v1.size(), 0);
  EXPECT_EQ(v1.capacity(), 0);
  EXPECT_EQ(v2.size(), size);
  EXPECT_EQ(v2.capacity(), capacity);
  for (int i = 0; i < 20; i++) {
    EXPECT_EQ(v2[i], prefix + std::to_string(i + 5));
  }
}

TEST(TestsVector, At) {
  s21::vector<int> a(5);
  EXPECT_THROW(a.at(10), std::out_of_range);
}

TEST(TestsVector, Data) {
  s21::vector<int> a(5);
  EXPECT_EQ(a.data(), &a[0]);
}

TEST(TestsVector, InsertMany) {
  std::string prefix(PREFIX);
  CombinedVector<std::string> v;
  v.insert_many_at(0, prefix + "Hello", prefix + "world");
  v.insert_many_at(1, prefix + "1", prefix + "2", prefix + "3");
  v.insert_many_back(prefix + "4", prefix + "5", prefix + "6");
  v.insert_many_at(0, prefix + "1", prefix + "2", prefix + "3");
}
