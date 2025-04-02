#ifndef S21_SET_H
#define S21_SET_H

#include "../AVL/AVLTree.h"
#include "../vector/s21_vector.h"

namespace s21 {

template <typename Key>
class set : public AVLTree<Key, Key> {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename AVLTree<Key, Key>::iterator;
  using const_iterator = typename AVLTree<Key, Key>::const_iterator;
  using size_type = size_t;

 private:
  using AVLTree<Key, Key>::root_;
  using AVLTree<Key, Key>::size_;

 public:
  set() : AVLTree<Key, Key>() {}
  set(std::initializer_list<value_type> const& items);
  set(const set& s) : AVLTree<Key, Key>(s) {}
  set(set&& s) : AVLTree<Key, Key>(std::move(s)){};
  ~set();
  set& operator=(set&& s) noexcept;

  // Capacity
  // bool empty() const; - from AVL
  // size_type size() const; - from AVL
  // size_type max_size(); - from AVL

  // Modifiers
  // void clear(); - from AVL
  std::pair<iterator, bool> insert(const value_type& value);
  // void erase(iterator pos); - from AVL
  // void swap(set& other); - from AVL
  // void merge(set& other); - from AVL

  // Lookup
  iterator find(const Key& key);
  // bool contains(const Key& key); - from AVL

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args);
};

}  // namespace s21

#include "s21_set.tpp"

#endif
