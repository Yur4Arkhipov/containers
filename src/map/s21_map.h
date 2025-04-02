#ifndef S21_MAP_H
#define S21_MAP_H

#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <vector>

#include "../AVL/AVLTree.h"
#include "../vector/s21_vector.h"

namespace s21 {

template <typename Key, typename T>
class map : public AVLTree<Key, T> {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;

  using size_type = size_t;

 private:
  using AVLTree<Key, T>::root_;
  using AVLTree<Key, T>::size_;

 public:
  // Member functions
  map() : AVLTree<Key, T>() {}
  map(std::initializer_list<value_type> const& items);
  map(const map& m) : AVLTree<Key, T>(m) {}
  map(map&& m) : AVLTree<Key, T>(std::move(m)){};
  ~map();
  map<Key, T>& operator=(map&& m) noexcept;

 private:
  class MapIterator : public AVLTree<Key, T>::Iterator {
   private:
    mutable value_type current_value_;

   public:
    using base_iterator = typename AVLTree<Key, T>::Iterator;

    MapIterator() noexcept : base_iterator() {}
    MapIterator(typename AVLTree<Key, T>::Node* node, AVLTree<Key, T>* tree)
        : base_iterator(node, tree) {
      update_value();
    }
    MapIterator(typename AVLTree<Key, T>::Node* node) : base_iterator(node) {
      update_value();
    }

    void refresh();
    void update_value();
    MapIterator& operator++();
    MapIterator operator++(int);
    MapIterator& operator--();
    MapIterator operator--(int);
    value_type operator*() const;
    value_type* operator->() const;
  };

  using iterator = MapIterator;
  using const_iterator = MapIterator;

 public:
  iterator begin();
  iterator end();

  // Element access
  T& at(const Key& key);
  T& operator[](const Key& key);

  // Capacity - from AVL full
  // bool empty();
  // size_type size();
  // size_type max_size();

  // Modifiers
  // void clear(); - from AVL
  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insert(const Key& key, const T& obj);
  std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj);
  // void erase(iterator pos); - from AVL
  // void swap(map& other); - from AVL
  // void merge(map& other); - from AVL

  // Loockup
  // bool contains(const Key& key); - from AVL

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args);
};

}  // namespace s21

#include "s21_map.tpp"

#endif
