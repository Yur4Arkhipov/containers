#ifndef S21_QUEUE_TPP
#define S21_QUEUE_TPP

#include "s21_queue.h"

namespace s21 {

// Queue Member functions
template <typename T>
queue<T>::queue() {}

template <typename T>
queue<T>::queue(std::initializer_list<value_type> const& items)
    : _list(items) {}

template <typename T>
queue<T>::queue(const queue& s) : _list(s._list) {}

template <typename T>
queue<T>::queue(queue&& s) : _list(std::move(s._list)) {}

template <typename T>
queue<T>::~queue() {}

template <typename T>
queue<T>& queue<T>::operator=(queue&& s) {
  _list = std::move(s._list);
  return *this;
}

// Queue Element access
template <typename T>
typename queue<T>::const_reference queue<T>::front() {
  return _list.front();
}

template <typename T>
typename queue<T>::const_reference queue<T>::back() {
  return _list.back();
}

// Queue Capacity
template <typename T>
bool queue<T>::empty() {
  return _list.empty();
}

template <typename T>
typename queue<T>::size_type queue<T>::size() {
  return _list.size();
}

// Queue Modifiers
template <typename T>
void queue<T>::push(const_reference value) {
  _list.push_back(value);
}

template <typename T>
void queue<T>::pop() {
  _list.pop_front();
}

template <typename T>
void queue<T>::swap(queue& other) {
  _list.swap(other._list);
}

template <typename T>
template <typename... Args>
void queue<T>::insert_many_back(Args&&... args) {
  for (auto& arg : {args...}) {
    push(arg);
  }
}

}  // namespace s21

#endif
