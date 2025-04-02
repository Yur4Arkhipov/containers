#ifndef S21_SET_TPP
#define S21_SET_TPP

#include <gtest/gtest-typed-test.h>

#include "s21_set.h"

namespace s21 {

// CONSTRUCTORS
template <typename Key>
set<Key>::set(std::initializer_list<value_type> const& items) {
  this->root_ = nullptr;
  this->size_ = 0;
  for (const auto& item : items) {
    insert(item);
  }
}

template <typename Key>
set<Key>& set<Key>::operator=(set&& s) noexcept {
  if (this != &s) {
    root_ = s.root_;
    size_ = s.size_;

    s.root_ = nullptr;
    s.size_ = 0;
  }
  return *this;
}

template <typename Key>
set<Key>::~set() {
  AVLTree<Key, Key>::clear();
}

// Modifiers
template <typename Key>
std::pair<typename set<Key>::iterator, bool> set<Key>::insert(
    const typename set<Key>::value_type& value) {
  return AVLTree<Key, Key>::insert(value, value);
}

// Lookup
template <typename Key>
typename set<Key>::iterator set<Key>::find(const Key& key) {
  return iterator(this->find_node(root_, key), this);
}

template <typename Key>
template <typename... Args>
vector<std::pair<typename set<Key>::iterator, bool>> set<Key>::insert_many(
    Args&&... args) {
  vector<std::pair<set<Key>::iterator, bool>> v;
  for (auto& arg : {args...}) {
    // v.push_back(std::make_pair(insert(arg), true));
    v.push_back(insert(arg));
  }

  return v;
}

}  // namespace s21

#endif