#ifndef S21_STACK_TPP
#define S21_STACK_TPP

#include "s21_stack.h"

namespace s21 {

// Stack Member type

template <typename T>
stack<T>::stack() {}

template <typename T>
stack<T>::stack(std::initializer_list<value_type> const& items)
    : _vector(items) {}

template <typename T>
stack<T>::stack(const stack& s) : _vector(s._vector) {}

template <typename T>
stack<T>::stack(stack&& s) : _vector(std::move(s._vector)) {}

template <typename T>
stack<T>::~stack() {}

template <typename T>
stack<T>& stack<T>::operator=(stack&& s) {
  _vector = std::move(s._vector);
  return *this;
}

// Stack Element access
template <typename T>
typename stack<T>::const_reference stack<T>::top() {
  return _vector.back();
}

// Stack Capacity

template <typename T>
bool stack<T>::empty() {
  return _vector.empty();
}

template <typename T>
typename stack<T>::size_type stack<T>::size() {
  return _vector.size();
}

// Stack Modifiers
template <typename T>
void stack<T>::push(const_reference value) {
  _vector.push_back(value);
}

template <typename T>
void stack<T>::pop() {
  _vector.pop_back();
}

template <typename T>
void stack<T>::swap(stack& other) {
  _vector.swap(other._vector);
}

template <typename T>
template <typename... Args>
void stack<T>::insert_many_back(Args&&... args) {
  for (auto& arg : {args...}) {
    push(arg);
  }
}

}  // namespace s21

#endif
