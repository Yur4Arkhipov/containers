#ifndef S21_ARRAY_H
#define S21_ARRAY_H

#include <cstddef>
#include <initializer_list>

namespace s21 {

template <typename T, std::size_t N>
class array {
 public:
  // Array Member type
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = std::size_t;

  // Array Member functions
  array();
  array(std::initializer_list<value_type> const& items);
  array(const array& a);
  array(array&& a);
  ~array();
  array& operator=(array&& a);

  // Array Element access
  reference at(size_type pos);
  reference operator[](size_type pos);
  const_reference front();
  const_reference back();
  iterator data();

  // Array Iterators
  iterator begin();
  iterator end();

  // Array Capacity
  bool empty();
  size_type size();
  size_type max_size();

  // Array Modifiers
  void swap(array& other);
  void fill(const_reference value);

 private:
  value_type data_[N];
};

}  // namespace s21

#include "s21_array.tpp"

#endif
