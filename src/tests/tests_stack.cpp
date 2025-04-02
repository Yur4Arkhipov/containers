#include <gtest/gtest.h>

#include <stack>
#include <string>

#include "../s21_containers.h"

#define PREFIX \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

template <typename T>
class CombinedStack {
 public:
  s21::stack<T> s21_stack;
  std::stack<T> std_stack;

  void ensure_equality() {
    EXPECT_EQ(s21_stack.size(), std_stack.size());
    if (!s21_stack.empty() && !std_stack.empty()) {
      EXPECT_EQ(s21_stack.top(), std_stack.top());
    }
  }

  CombinedStack() {}

  CombinedStack(std::initializer_list<T> const& items)
      : s21_stack(items), std_stack(items) {
    ensure_equality();
  }

  void push(const T& value) {
    s21_stack.push(value);
    std_stack.push(value);
    ensure_equality();
  }

  void pop() {
    EXPECT_FALSE(s21_stack.empty());
    EXPECT_FALSE(std_stack.empty());
    s21_stack.pop();
    std_stack.pop();
    ensure_equality();
  }

  void swap(CombinedStack& other) {
    s21_stack.swap(other.s21_stack);
    std_stack.swap(other.std_stack);
    ensure_equality();
    other.ensure_equality();
  }

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    s21_stack.insert_many_back(args...);
    for (const auto& item : {args...}) {
      std_stack.push(item);
    }
    ensure_equality();
  }
};

TEST(TestsStack, PushAndPop) {
  std::string prefix(PREFIX);
  CombinedStack<std::string> s;

  s.push(prefix + "1");
  s.push(prefix + "2");
  s.push(prefix + "3");

  s.pop();
  s.pop();
  s.pop();

  s.insert_many_back(prefix + "1", prefix + "2", prefix + "3");

  s.pop();
  s.pop();
  s.pop();

  s.insert_many_back(prefix + "1", prefix + "2", prefix + "3");
  s.insert_many_back(prefix + "1", prefix + "2", prefix + "3");

  s.pop();
  s.pop();
  s.pop();
  s.pop();
  s.pop();
  s.pop();
}

TEST(TestsStack, InitializerList) {
  std::string prefix(PREFIX);
  CombinedStack<std::string> s = {prefix + "Hello", prefix + "world!",
                                  prefix + "stack!"};
  s.pop();
  s.pop();
  s.pop();
}

TEST(TestsStack, CopyConstruct) {
  std::string prefix(PREFIX);
  CombinedStack<std::string> s1 = {prefix + "First", prefix + "Second"};
  CombinedStack<std::string> s2 = s1;
  s2.pop();
  s2.pop();
}

TEST(TestsStack, MoveConstruct) {
  std::string prefix(PREFIX);
  CombinedStack<std::string> s1 = {prefix + "First", prefix + "Second"};
  CombinedStack<std::string> s2 = std::move(s1);

  EXPECT_TRUE(s1.s21_stack.empty());
  s2.pop();
  s2.pop();
}

TEST(TestsStack, Swap) {
  std::string prefix(PREFIX);
  CombinedStack<std::string> s1 = {prefix + "Stack1", prefix + "Test1"};
  CombinedStack<std::string> s2 = {prefix + "Stack2", prefix + "Test2"};

  s1.swap(s2);

  s1.pop();
  s1.pop();
  s2.pop();
  s2.pop();
}

TEST(TestsStack, Move) {
  std::string prefix(PREFIX);
  CombinedStack<std::string> s1 = {prefix + "Stack1", prefix + "Test1"};
  CombinedStack<std::string> s2 = {prefix + "Stack2", prefix + "Test2"};
  s2 = std::move(s1);
  EXPECT_EQ(s1.s21_stack.size(), 0);
  s2.pop();
  s2.pop();
}
