#ifndef S21_MAP_TPP
#define S21_MAP_TPP

#include "s21_map.h"

namespace s21 {

// Member functions
template <typename Key, typename T>
map<Key, T>::map(std::initializer_list<value_type> const& items) {
  this->root_ = nullptr;
  this->size_ = 0;
  for (const auto& item : items) {
    insert(item);
  }
}

template <typename Key, typename T>
map<Key, T>& map<Key, T>::operator=(map&& m) noexcept {
  if (this != &m) {
    root_ = m.root_;
    size_ = m.size_;

    m.root_ = nullptr;
    m.size_ = 0;
  }
  return *this;
}

template <typename Key, typename T>
map<Key, T>::~map() {
  AVLTree<Key, T>::clear();
}

// Element access
template <typename Key, typename T>
typename map<Key, T>::mapped_type& map<Key, T>::at(const Key& key) {
  auto node = this->find_node(this->root_, key);
  if (!node) {
    throw std::out_of_range("Key not found");
  }
  return node->value_;
}

template <typename Key, typename T>
typename map<Key, T>::mapped_type& map<Key, T>::operator[](const Key& key) {
  auto node = this->find_node(this->root_, key);
  if (node) {
    return node->value_;
  }
  mapped_type default_value = mapped_type();
  insert(std::make_pair(key, default_value));
  node = this->find_node(this->root_, key);

  return node->value_;
}

// Iterator
template <typename Key, typename T>
typename map<Key, T>::value_type map<Key, T>::MapIterator::operator*() const {
  return current_value_;
}

template <typename Key, typename T>
typename map<Key, T>::value_type* map<Key, T>::MapIterator::operator->() const {
  return &current_value_;
}

template <typename Key, typename T>
typename map<Key, T>::MapIterator& map<Key, T>::MapIterator::operator++() {
  base_iterator::operator++();
  update_value();
  return *this;
}

template <typename Key, typename T>
typename map<Key, T>::MapIterator map<Key, T>::MapIterator::operator++(int) {
  MapIterator temp = *this;
  ++(*this);
  return temp;
}

template <typename Key, typename T>
typename map<Key, T>::MapIterator& map<Key, T>::MapIterator::operator--() {
  base_iterator::operator--();
  update_value();
  return *this;
}

template <typename Key, typename T>
typename map<Key, T>::MapIterator map<Key, T>::MapIterator::operator--(int) {
  MapIterator temp = *this;
  --(*this);
  return temp;
}

template <typename Key, typename T>
void map<Key, T>::MapIterator::update_value() {
  const auto* node = this->get_node();
  if (node) {
    new (&current_value_) value_type{node->key_, node->value_};
  }
}

template <typename Key, typename T>
void map<Key, T>::MapIterator::refresh() {
  const auto* node = this->get_node();
  if (node) {
    new (&current_value_) value_type{node->key_, node->value_};
  }
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::begin() {
  return iterator(this->find_min_node(this->root_), this);
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::end() {
  return iterator(nullptr, this);
}

// Modifiers
template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const value_type& value) {
  auto result = AVLTree<Key, T>::insert(value.first, value.second);
  iterator map_iter(result.first.get_node(), this);
  return {map_iter, result.second};
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const Key& key, const T& obj) {
  auto result = AVLTree<Key, T>::insert(key, obj);
  MapIterator map_iter(result.first.get_node(), this);
  return {map_iter, result.second};
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert_or_assign(
    const Key& key, const T& obj) {
  auto result = AVLTree<Key, T>::insert(key, obj);

  MapIterator map_iter(result.first.get_node(), this);

  if (result.second) {
    return {map_iter, true};
  } else {
    result.first.get_node()->value_ = obj;
    map_iter.refresh();
    return {map_iter, false};
  }
}

template <typename Key, typename T>
template <typename... Args>
vector<std::pair<typename map<Key, T>::iterator, bool>>
map<Key, T>::insert_many(Args&&... args) {
  vector<std::pair<map<Key, T>::iterator, bool>> v;
  for (auto& arg : {args...}) {
    v.push_back(insert(arg));
  }

  return v;
}

}  // namespace s21

#endif