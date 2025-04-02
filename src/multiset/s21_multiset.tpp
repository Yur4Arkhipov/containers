#ifndef S21_MULTISET_TPP
#define S21_MULTISET_TPP

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <utility>

#include "s21_multiset.h"

namespace s21 {

template <typename Key>
multiset<Key>::multiset() : _size(0) {}

template <typename Key>
multiset<Key>::multiset(std::initializer_list<value_type> const& items)
    : _size(0) {
  for (auto& item : items) {
    insert(item);
  }
}

template <typename Key>
multiset<Key>::multiset(const multiset& ms) : _set(ms._set), _size(ms._size) {}

template <typename Key>
multiset<Key>::multiset(multiset&& ms)
    : _set(std::move(ms._set)), _size(ms._size) {
  ms._size = 0;
}

template <typename Key>
multiset<Key>::~multiset() {}

template <typename Key>
multiset<Key>& multiset<Key>::operator=(multiset&& ms) {
  if (this != &ms) {
    _set = std::move(ms._set);
    _size = std::move(ms._size);
  }
  return *this;
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::insert(
    const value_type& value) {
  _size++;
  Iterator iter = find(value);
  if (iter != end()) {
    iter.set_iter->items.push_back(value);
    iter.index = iter.set_iter->items.size() - 1;
  } else {
    Wrapper<Key> wrapper(value);
    auto it = _set.insert(wrapper);
    Iterator new_it(it.first, 0);
    iter = new_it;
  }

  return iter;
}

template <typename Key>
void multiset<Key>::erase(iterator pos) {
  multiset<Key>::_size--;
  if (pos.set_iter->items.size() > 1) {
    pos.set_iter->items.erase(pos.set_iter->items.begin() + pos.index);
  } else {
    _set.erase(pos.set_iter);
  }
}

// Поиск
template <typename Key>
typename multiset<Key>::iterator multiset<Key>::find(const value_type& value) {
  Wrapper<Key> wrapper(value);
  auto iter = _set.find(wrapper);
  return Iterator(iter, 0);
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::size() {
  return _size;
}

template <typename Key>
bool multiset<Key>::empty() {
  return _size == 0;
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::max_size() {
  return std::numeric_limits<size_t>::max();
}

template <typename Key>
bool multiset<Key>::Iterator::operator==(const Iterator& other) const {
  return set_iter == other.set_iter && index == other.index;
}

template <typename Key>
bool multiset<Key>::Iterator::operator!=(const Iterator& other) const {
  return set_iter != other.set_iter || index != other.index;
}

template <typename Key>
typename multiset<Key>::Iterator& multiset<Key>::Iterator::operator++() {
  if (set_iter->items.size() - 1 == index) {
    set_iter++;
    index = 0;
  } else {
    index++;
  }

  return *this;
}

template <typename Key>
typename multiset<Key>::Iterator multiset<Key>::Iterator::operator++(int) {
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template <typename Key>
typename multiset<Key>::Iterator& multiset<Key>::Iterator::operator--() {
  if (index == 0) {
    set_iter--;
    index = set_iter->items.size() - 1;
  } else {
    index--;
  }

  return *this;
}

template <typename Key>
typename multiset<Key>::Iterator multiset<Key>::Iterator::operator--(int) {
  Iterator temp = *this;
  --(*this);
  return temp;
}

template <typename Key>
typename multiset<Key>::value_type multiset<Key>::Iterator::operator*() {
  return set_iter->items[index];
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::end() {
  return Iterator(_set.end(), 0);
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::begin() {
  return Iterator(_set.begin(), 0);
}

template <typename T>
Wrapper<T>::Wrapper(const T& value) {
  items.push_back(value);
}

template <typename T>
bool Wrapper<T>::operator==(const Wrapper<T>& other) const {
  return items[0] == other.items[0];
}

template <typename T>
bool Wrapper<T>::operator<(const Wrapper<T>& other) const {
  return items[0] < other.items[0];
}

template <typename T>
bool Wrapper<T>::operator>(const Wrapper<T>& other) const {
  return items[0] > other.items[0];
}

template <typename T>
bool Wrapper<T>::operator<=(const Wrapper<T>& other) const {
  return items[0] <= other.items[0];
}

template <typename T>
bool Wrapper<T>::operator>=(const Wrapper<T>& other) const {
  return items[0] >= other.items[0];
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::lower_bound(const Key& key) {
  return find(key);
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::upper_bound(const Key& key) {
  auto iter = find(key);
  if (iter == end()) {
    return end();
  }
  iter.set_iter++;

  return iter;
}

template <typename Key>
std::pair<typename multiset<Key>::iterator, typename multiset<Key>::iterator>
multiset<Key>::equal_range(const Key& key) {
  auto up = upper_bound(key);
  auto low = lower_bound(key);

  return {low, up};
}

template <typename Key>
bool multiset<Key>::contains(const Key& key) {
  return find(key) != end();
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::count(const Key& key) {
  auto iter = find(key);
  if (iter == end()) {
    return 0;
  }

  return iter.set_iter->items.size();
}

template <typename Key>
void multiset<Key>::swap(multiset& other) {
  std::swap(this->_set, other._set);
  std::swap(this->_size, other._size);
}

template <typename Key>
void multiset<Key>::merge(multiset& other) {
  if (this == &other) {
    return;
  }
  for (const auto& item : other) {
    this->insert(item);
  }
  other.clear();
  other._size = 0;
}

template <typename Key>
void multiset<Key>::clear() {
  _set.clear();
  _size = 0;
}

template <typename Key>
template <typename... Args>
vector<std::pair<typename multiset<Key>::iterator, bool>>
multiset<Key>::insert_many(Args&&... args) {
  vector<std::pair<multiset<Key>::iterator, bool>> v;
  for (auto& arg : {args...}) {
    v.push_back(std::make_pair(insert(arg), true));
  }

  return v;
}

}  // namespace s21

#endif