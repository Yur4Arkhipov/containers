#ifndef S21_VECTOR_H
#define S21_VECTOR_H

#include <cstddef>
#include <initializer_list>

namespace s21 {

template <typename T>
class vector {
 public:
  // Vector Member type
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = std::size_t;

  // Vector Member functions
  vector();
  vector(size_type n);
  vector(std::initializer_list<value_type> const& items);
  vector(const vector& other);
  vector(vector&& other) noexcept;
  ~vector();
  vector& operator=(vector&& other);

  // Vector Element access
  reference at(size_type pos);
  reference operator[](size_type pos);
  const_reference front() const noexcept;
  const_reference back() const noexcept;
  T* data() noexcept;

  // Vector Iterators
  iterator begin() noexcept;
  iterator end() noexcept;

  // Vector Capacity
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void reserve(size_type size);
  size_type capacity() const noexcept;
  void shrink_to_fit();

  // Vector Modifiers
  void clear();
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back() noexcept;
  void swap(vector& other) noexcept;

  template <typename... Args>
  iterator insert_many(iterator pos, Args&&... args);
  template <typename... Args>
  void insert_many_back(Args&&... args);

 private:
  value_type* data_;
  size_type size_;
  size_type capacity_;

  void ensure_capacity_1();
};

}  // namespace s21

#include "s21_vector.tpp"

#endif
