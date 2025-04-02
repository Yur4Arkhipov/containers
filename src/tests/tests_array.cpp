#include <gtest/gtest.h>

#include <array>
#include <initializer_list>
#include <string>
#include <utility>

#include "../s21_containersplus.h"

#define PREFIX \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

template <typename T, size_t N>
class CombinedArray {
 public:
  s21::array<T, N> s21_array;
  std::array<T, N> std_array;

  void ensure_equality() {
    EXPECT_EQ(s21_array.size(), std_array.size());
    for (size_t i = 0; i < N; i++) {
      EXPECT_EQ(s21_array[i], std_array[i]);
    }
  }

  CombinedArray() { ensure_equality(); }

  CombinedArray(std::initializer_list<T> items) : s21_array(items) {
    for (size_t i = 0; i < N; i++) {
      std_array[i] = items.begin()[i];
    }
    ensure_equality();
  }

  CombinedArray(CombinedArray<T, N>& other)
      : s21_array(other.s21_array), std_array(other.std_array) {
    ensure_equality();
    other.ensure_equality();
  }

  CombinedArray(CombinedArray<T, N>&& other)
      : s21_array(std::move(other.s21_array)),
        std_array(std::move(other.std_array)) {
    ensure_equality();
    other.ensure_equality();
  }

  CombinedArray<T, N>& operator=(CombinedArray<T, N>&& other) {
    s21_array = std::move(other.s21_array);
    std_array = std::move(other.std_array);
    ensure_equality();
    other.ensure_equality();
    return *this;
  }

  void swap(CombinedArray<T, N>& other) {
    s21_array.swap(other.s21_array);
    std_array.swap(other.std_array);
    ensure_equality();
    other.ensure_equality();
  }

  void fill(const T& value) {
    s21_array.fill(value);
    std_array.fill(value);
    ensure_equality();
  }
};

TEST(TestsArray, DefConstruct) { CombinedArray<std::string, 5> a; }

TEST(TestsArray, InitListConstruct) {
  std::string prefix(PREFIX);
  CombinedArray<std::string, 5> a = {prefix + "a", prefix + "b", prefix + "c",
                                     prefix + "d", prefix + "e"};
}

TEST(TestsArray, CopyConstruct) {
  std::string prefix(PREFIX);
  CombinedArray<std::string, 5> a = {prefix + "a", prefix + "b", prefix + "c",
                                     prefix + "d", prefix + "e"};
  CombinedArray<std::string, 5> b = a;
}

TEST(TestsArray, MoveConstruct) {
  std::string prefix(PREFIX);
  CombinedArray<std::string, 5> a = {prefix + "a", prefix + "b", prefix + "c",
                                     prefix + "d", prefix + "e"};
  CombinedArray<std::string, 5> b = std::move(a);
}

TEST(TestsArray, MoveAssign) {
  std::string prefix(PREFIX);
  CombinedArray<std::string, 5> a = {prefix + "a", prefix + "b", prefix + "c",
                                     prefix + "d", prefix + "e"};
  CombinedArray<std::string, 5> b = {prefix + "f", prefix + "g", prefix + "h",
                                     prefix + "i", prefix + "j"};
  b = std::move(a);
}

TEST(TestsArray, At) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  EXPECT_EQ(a.at(0), 1);
  EXPECT_EQ(a.at(4), 5);
  EXPECT_THROW(a.at(5), std::out_of_range);
}

TEST(TestsArray, FrontBack) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  EXPECT_EQ(a.front(), 1);
  EXPECT_EQ(a.back(), 5);
}

TEST(TestsArray, Data) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  EXPECT_EQ(a.data(), &a[0]);
}

TEST(TestsArray, BeginEnd) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  EXPECT_EQ(a.begin(), &a[0]);
  EXPECT_EQ(a.end(), &a[5]);
}

TEST(TestsArray, Empty) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  EXPECT_FALSE(a.empty());
  s21::array<int, 0> b;
  EXPECT_TRUE(b.empty());
}

TEST(TestsArray, Swap) {
  std::string prefix(PREFIX);
  CombinedArray<std::string, 5> a = {prefix + "a", prefix + "b", prefix + "c",
                                     prefix + "d", prefix + "e"};
  CombinedArray<std::string, 5> b = {prefix + "f", prefix + "g", prefix + "h",
                                     prefix + "i", prefix + "j"};
  a.swap(b);
}

TEST(TestsArray, Fill) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  a.fill(10);
}
