#ifndef S21_ARRAY_tpp
#define S21_ARRAY_tpp

#include <stdexcept>

#include "s21_array.h"

namespace s21 {

template <typename T, std::size_t N>
array<T, N>::array() {}

template <typename T, std::size_t N>
array<T, N>::array(std::initializer_list<value_type> const& items) {
  if (items.size() != N) {
    throw std::out_of_range("items.size() != N");
  }

  iterator ptr = begin();
  for (auto item : items) {
    *ptr = item;
    ptr++;
  }
}

template <typename T, std::size_t N>
array<T, N>::array(const array& a) {
  for (size_type i = 0; i < N; i++) {
    data_[i] = a.data_[i];
  }
}

template <typename T, std::size_t N>
array<T, N>::array(array&& a) {
  for (size_type i = 0; i < N; i++) {
    data_[i] = std::move(a.data_[i]);
  }
}

template <typename T, std::size_t N>
array<T, N>::~array() {}

template <typename T, std::size_t N>
array<T, N>& array<T, N>::operator=(array&& a) {
  for (size_type i = 0; i < N; i++) {
    data_[i] = std::move(a.data_[i]);
  }
  return *this;
}

// Array Element access
template <typename T, std::size_t N>
typename array<T, N>::reference array<T, N>::at(size_type pos) {
  if (pos >= N) {
    throw std::out_of_range("pos >= N");
  }
  return data_[pos];
}

template <typename T, std::size_t N>
typename array<T, N>::reference array<T, N>::operator[](size_type pos) {
  return data_[pos];
}

template <typename T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::front() {
  return data_[0];
}

template <typename T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::back() {
  return data_[N - 1];
}

template <typename T, std::size_t N>
typename array<T, N>::iterator array<T, N>::data() {
  return data_;
}

// Array Iterators
template <typename T, std::size_t N>
typename array<T, N>::iterator array<T, N>::begin() {
  return data_;
}

template <typename T, std::size_t N>
typename array<T, N>::iterator array<T, N>::end() {
  return data_ + N;
}

// Array Capacity
template <typename T, std::size_t N>
bool array<T, N>::empty() {
  return N == 0;
}

template <typename T, std::size_t N>
typename array<T, N>::size_type array<T, N>::size() {
  return N;
}

template <typename T, std::size_t N>
typename array<T, N>::size_type array<T, N>::max_size() {
  std::allocator<value_type> a;
  return a.max_size();
}

// Array Modifiers
template <typename T, std::size_t N>
void array<T, N>::swap(array& other) {
  for (size_type i = 0; i < N; i++) {
    std::swap(data_[i], other.data_[i]);
  }
}

template <typename T, std::size_t N>
void array<T, N>::fill(const_reference value) {
  for (size_type i = 0; i < N; i++) {
    data_[i] = value;
  }
}

}  // namespace s21

#endif
