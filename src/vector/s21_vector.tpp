#ifndef S21_VECTOR_TPP
#define S21_VECTOR_TPP

#include <memory>
#include <stdexcept>

#include "s21_vector.h"

namespace s21 {

// Vector Member functions

template <typename T>
vector<T>::vector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
vector<T>::vector(size_type n) : vector() {
  if (n > max_size()) {
    throw std::out_of_range("new_cap is >= max_size()");
  }
  reserve(n);
  new (data_) value_type[n];
  size_ = n;
}

template <typename T>
vector<T>::vector(std::initializer_list<value_type> const& items) : vector() {
  reserve(items.size());
  value_type* ptr = data_;
  for (auto it = items.begin(); it != items.end(); it++) {
    new (ptr) value_type(*it);
    ptr++;
  }
  size_ = items.size();
}

template <typename T>
vector<T>::vector(const vector& other) : vector() {
  reserve(other.size());
  for (size_type i = 0; i < other.size(); i++) {
    new (data_ + i) value_type(other.data_[i]);
  }
  size_ = other.size();
}

template <typename T>
vector<T>::vector(vector&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template <typename T>
vector<T>::~vector() {
  for (size_type i = 0; i < size_; i++) {
    data_[i].~value_type();
  }
  std::allocator<value_type> a;
  a.deallocate(data_, capacity_);
}

template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& other) {
  if (this != &other) {
    clear();
    shrink_to_fit();
    swap(other);
  }
  return *this;
}

// Vector Element access

template <typename T>
typename vector<T>::reference vector<T>::at(size_type pos) {
  if (pos >= size_) {
    throw std::out_of_range("Index out of range");
  }
  return data_[pos];
}

template <typename T>
typename vector<T>::reference vector<T>::operator[](size_type pos) {
  if (pos >= size_) {
    throw std::out_of_range("Index out of range");
  }
  return data_[pos];
}

template <typename T>
typename vector<T>::const_reference vector<T>::front() const noexcept {
  return data_[0];
}

template <typename T>
typename vector<T>::const_reference vector<T>::back() const noexcept {
  return data_[size_ - 1];
}

template <typename T>
T* vector<T>::data() noexcept {
  return data_;
}

// Vector Iterators

template <typename T>
typename vector<T>::iterator vector<T>::begin() noexcept {
  return data_;
}

template <typename T>
typename vector<T>::iterator vector<T>::end() noexcept {
  return data_ + size_;
}

// Vector Capacity

template <typename T>
bool vector<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
typename vector<T>::size_type vector<T>::size() const noexcept {
  return size_;
}

template <typename T>
typename vector<T>::size_type vector<T>::max_size() const noexcept {
  std::allocator<value_type> a;
  return a.max_size();
}

template <typename T>
void vector<T>::reserve(size_type new_cap) {
  if (new_cap > max_size()) {
    throw std::out_of_range("new_cap is >= max_size()");
  }
  if (new_cap > capacity_) {
    std::allocator<value_type> a;
    value_type* new_data = a.allocate(new_cap);
    for (size_type i = 0; i < size_; i++) {
      new (new_data + i) value_type(std::move(data_[i]));
      data_[i].~value_type();
    }
    if (capacity_ > 0) {
      a.deallocate(data_, capacity_);
    }
    data_ = new_data;
    capacity_ = new_cap;
  }
}

template <typename T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
  return capacity_;
}

template <typename T>
void vector<T>::shrink_to_fit() {
  if (size_ < capacity_) {
    if (size_ > 0) {
      std::allocator<value_type> a;
      value_type* new_data = a.allocate(size_);
      for (size_type i = 0; i < size_; i++) {
        new (new_data + i) value_type(std::move(data_[i]));
        data_[i].~value_type();
      }
      a.deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = size_;
    } else {
      std::allocator<value_type> a;
      a.deallocate(data_, capacity_);
      data_ = nullptr;
      capacity_ = size_;
    }
  }
}

// Vector Modifiers

template <typename T>
void vector<T>::clear() {
  for (size_type i = 0; i < size_; i++) {
    data_[i].~value_type();
  }
  size_ = 0;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos,
                                               const_reference value) {
  size_type move_number = end() - pos;
  size_type idx = pos - begin();
  ensure_capacity_1();
  if (idx == size_) {
    push_back(value);
  } else {
    // first move is placement new, because end() is uninitialized
    new (end()) value_type(std::move(*(end() - 1)));
    for (size_type i = 1; i < move_number; i++) {
      *(end() - i) = std::move(*(end() - i - 1));
    }
    data_[idx] = value;
    size_++;
  }
  return begin() + idx;
}

template <typename T>
void vector<T>::erase(iterator pos) {
  for (iterator it = pos; it != end() - 1; it++) {
    *it = std::move(*(it + 1));
  }
  (end() - 1)->~value_type();
  --size_;
}

template <typename T>
void vector<T>::push_back(const_reference value) {
  ensure_capacity_1();
  new (end()) value_type(value);
  size_++;
}

template <typename T>
void vector<T>::pop_back() noexcept {
  data_[size_ - 1].~value_type();
  size_--;
}

template <typename T>
void vector<T>::swap(vector& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T>
template <typename... Args>
typename vector<T>::iterator vector<T>::insert_many(iterator pos,
                                                    Args&&... args) {
  for (auto& arg : {args...}) {
    pos = insert(pos, arg);
    pos++;
  }
  return pos;
}

template <typename T>
template <typename... Args>
void vector<T>::insert_many_back(Args&&... args) {
  for (auto& arg : {args...}) {
    push_back(arg);
  }
}

template <typename T>
void vector<T>::ensure_capacity_1() {
  if (size_ == capacity_) {
    reserve(capacity_ == 0 ? 1 : 2 * capacity_);
  }
}

}  // namespace s21

#endif
