#ifndef S21_QUEUE_H
#define S21_QUEUE_H

#include <cstddef>
#include <initializer_list>

#include "../list/s21_list.h"

namespace s21 {

template <typename T>
class queue {
 public:
  // Stack Member type
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;

  // Queue Member functions
  queue();
  queue(std::initializer_list<value_type> const& items);
  queue(const queue& q);
  queue(queue&& q);
  ~queue();
  queue& operator=(queue&& q);

  // Queue Element access
  const_reference front();
  const_reference back();

  // Queue Capacity
  bool empty();
  size_type size();

  // Queue Modifiers
  void push(const_reference value);
  void pop();
  void swap(queue& other);

  template <typename... Args>
  void insert_many_back(Args&&... args);

 private:
  list<value_type> _list;
};

}  // namespace s21

#include "s21_queue.tpp"

#endif
