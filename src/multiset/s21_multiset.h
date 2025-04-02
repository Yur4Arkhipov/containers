#ifndef S21_MULTISET_H
#define S21_MULTISET_H

#include <cstddef>
#include <vector>

#include "../AVL/AVLTree.h"
#include "../set/s21_set.h"
#include "../vector/s21_vector.h"

namespace s21 {

template <typename T>
struct Wrapper {
  std::vector<T> items;

  Wrapper(const T& value);

  bool operator==(const Wrapper<T>& other) const;
  bool operator<(const Wrapper<T>& other) const;
  bool operator>(const Wrapper<T>& other) const;
  bool operator<=(const Wrapper<T>& other) const;
  bool operator>=(const Wrapper<T>& other) const;
};

template <typename Key>
class multiset {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  // Конструкторы и деструктор
  multiset();
  multiset(std::initializer_list<value_type> const& items);
  multiset(const multiset& ms);
  multiset(multiset&& ms);
  ~multiset();
  multiset& operator=(multiset&& ms);

  class Iterator {
   public:
    typename s21::set<Wrapper<Key>>::Iterator set_iter;
    size_type index;

    Iterator(typename s21::set<Wrapper<Key>>::Iterator set_iter,
             size_type index)
        : set_iter(set_iter), index(index) {}

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
    Iterator& operator++();
    Iterator operator++(int);
    Iterator& operator--();
    Iterator operator--(int);
    // Iterator operator--(int);
    value_type operator*();
  };

  using iterator = Iterator;

  iterator begin();
  iterator end();

  // Модификаторы
  iterator insert(const value_type& value);
  void erase(iterator pos);
  void swap(multiset& other);
  void merge(multiset& other);
  void clear();

  // Поиск
  iterator find(const value_type& value);
  std::pair<iterator, iterator> equal_range(const Key& key);
  iterator lower_bound(const Key& key);
  iterator upper_bound(const Key& key);
  bool contains(const Key& key);
  size_type count(const Key& key);

  // Capacity
  bool empty();
  size_type size();
  size_type max_size();

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args);

 private:
  s21::set<Wrapper<Key>> _set;
  size_type _size;
};

}  // namespace s21

#include "s21_multiset.tpp"

#endif
