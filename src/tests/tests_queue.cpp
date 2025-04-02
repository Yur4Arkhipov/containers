#include <gtest/gtest.h>

#include <queue>
#include <string>

#include "../s21_containers.h"

#define PREFIX \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

template <typename T>
class CombinedQueue {
 public:
  s21::queue<T> s21_queue;
  std::queue<T> std_queue;

  void ensure_equality() {
    EXPECT_EQ(s21_queue.size(), std_queue.size());
    if (!s21_queue.empty() && !std_queue.empty()) {
      EXPECT_EQ(s21_queue.front(), std_queue.front());
      EXPECT_EQ(s21_queue.back(), std_queue.back());
    }
  }

  CombinedQueue() {}

  CombinedQueue(std::initializer_list<T> const& items)
      : s21_queue(items), std_queue(items) {
    ensure_equality();
  }

  void push(const T& value) {
    s21_queue.push(value);
    std_queue.push(value);
    ensure_equality();
  }

  void pop() {
    EXPECT_FALSE(s21_queue.empty());
    EXPECT_FALSE(std_queue.empty());
    s21_queue.pop();
    std_queue.pop();
    ensure_equality();
  }

  void swap(CombinedQueue& other) {
    s21_queue.swap(other.s21_queue);
    std_queue.swap(other.std_queue);
    ensure_equality();
    other.ensure_equality();
  }

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    s21_queue.insert_many_back(args...);
    for (const auto& item : {args...}) {
      std_queue.push(item);
    }
    ensure_equality();
  }
};

TEST(TestsQueue, PushAndPop) {
  std::string prefix(PREFIX);
  CombinedQueue<std::string> s;

  s.push(prefix + "1");
  s.push(prefix + "2");
  s.push(prefix + "3");

  s.pop();
  s.pop();
  s.pop();
}

TEST(TestsQueue, InitializerList) {
  std::string prefix(PREFIX);
  CombinedQueue<std::string> s = {prefix + "Hello", prefix + "world!",
                                  prefix + "queue!"};
  s.pop();
  s.pop();
  s.pop();
}

TEST(TestsQueue, CopyConstruct) {
  std::string prefix(PREFIX);
  CombinedQueue<std::string> s1 = {prefix + "First", prefix + "Second"};
  CombinedQueue<std::string> s2 = s1;
  s2.pop();
  s2.pop();
}

TEST(TestsQueue, MoveConstruct) {
  std::string prefix(PREFIX);
  CombinedQueue<std::string> s1 = {prefix + "First", prefix + "Second"};
  CombinedQueue<std::string> s2 = std::move(s1);

  EXPECT_TRUE(s1.s21_queue.empty());
  s2.pop();
  s2.pop();
}

TEST(TestsQueue, Swap) {
  std::string prefix(PREFIX);
  CombinedQueue<std::string> s1 = {prefix + "Queue1", prefix + "Test1"};
  CombinedQueue<std::string> s2 = {prefix + "Queue2", prefix + "Test2"};

  s1.swap(s2);

  s1.pop();
  s1.pop();
  s2.pop();
  s2.pop();
}

TEST(TestsQueue, Move) {
  std::string prefix(PREFIX);
  CombinedQueue<std::string> s1 = {prefix + "Queue1", prefix + "Test1"};
  CombinedQueue<std::string> s2 = {prefix + "Queue2", prefix + "Test2"};
  s2 = std::move(s1);
  EXPECT_EQ(s1.s21_queue.size(), 0);
  s2.pop();
  s2.pop();
}

TEST(TestsQueue, InsertMany) {
  std::string prefix(PREFIX);
  CombinedQueue<std::string> s;
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