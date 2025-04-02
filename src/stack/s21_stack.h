#ifndef S21_STACK_H
#define S21_STACK_H

#include <cstddef>
#include <initializer_list>

#include "../vector/s21_vector.h"

namespace s21 {

template <typename T>
class stack {
 public:
  // Stack Member type
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;

  // Stack Member functions
  stack();
  stack(std::initializer_list<value_type> const& items);
  stack(const stack& s);
  stack(stack&& s);
  ~stack();
  stack& operator=(stack&& s);

  // Stack Element access
  const_reference top();

  // Stack Capacity
  bool empty();
  size_type size();

  // Stack Modifiers
  void push(const_reference value);
  void pop();
  void swap(stack& other);

  template <typename... Args>
  void insert_many_back(Args&&... args);

 private:
  vector<value_type> _vector;
};

}  // namespace s21

#include "s21_stack.tpp"

#endif
