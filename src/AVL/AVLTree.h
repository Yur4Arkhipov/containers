#ifndef AVLTree_H
#define AVLTree_H

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

namespace s21 {

template <typename Key, typename T>
class AVLTree {
 public:
  using key_type = Key;
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

 protected:
  struct Node {
    key_type key_;
    value_type value_;
    Node* left_;
    Node* right_;
    Node* parent_;

    int count;
    int height;

    Node(key_type key, value_type value);
    void update_values();
    int balance_factor();

    Node* left_rotate();
    Node* right_rotate();
  };

  Node* root_;
  size_type size_ = 0;

 public:
  class ConstIterator {
   private:
    AVLTree* AVLTree_;
    Node* node_;

   public:
    // Constructors
    ConstIterator() noexcept;
    ConstIterator(Node* node, AVLTree* tree);
    ConstIterator(Node* node);

    // Operators
    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;
    value_type operator*() const;
    const value_type* operator->() const;
    ConstIterator& operator++();
    ConstIterator operator++(int);
    ConstIterator& operator--();
    ConstIterator operator--(int);

    // help functions
    reference get_element() const;
    Node* get_node() const;
  };

  class Iterator : public ConstIterator {
   public:
    Iterator() noexcept : ConstIterator() {}
    Iterator(Node* node, AVLTree* tree) noexcept : ConstIterator(node, tree) {}
    Iterator(Node* node) : ConstIterator(node) {}
    ~Iterator() = default;
    value_type* operator->();
  };

 public:
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  iterator begin();
  iterator end();
  const_iterator cbegin();
  const_iterator cend();

  // AVLTree Constructors
  AVLTree();
  AVLTree(std::initializer_list<value_type> const& items);
  // AVLTree(const Key& key, const T& value);
  // AVLTree(const_reference elem) noexcept;
  AVLTree(const AVLTree& other) noexcept;
  AVLTree(AVLTree&& other) noexcept;
  // AVLTree& operator=(const AVLTree& other) noexcept;
  AVLTree& operator=(AVLTree&& other) noexcept;
  ~AVLTree();

 protected:
  Node* insert_node(Node* node, const Key& key, const T& value,
                    Node*& inserted_node, bool& inserted);
  Node* find_max_node(Node* node);
  Node* find_min_node(Node* node);
  Node* find_next_node(Node* node);
  Node* find_prev_node(Node* node);
  Node* find_last_node() const;
  void merge_nodes(Node* node);
  Node* find_node(Node* node, const Key& query_key);
  void clear_tree(Node* root);
  Node* copy_node(Node* other_node);
  void display(Node* cur, int depth = 0, int state = 0);
  Node* erase_node(Node* node, const Key& key);
  Node* replace_with_successor(Node* node);

 public:
  // Capacity
  bool empty() const;
  size_type size() const;
  size_type max_size();

  // Modifiers
  void clear();
  std::pair<iterator, bool> insert(const Key& key, const T& obj);
  std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj);
  void erase(Iterator pos);
  void swap(AVLTree& other) noexcept;
  void merge(AVLTree& other);

  // Lookup
  void display();
  bool contains(const Key& key);
};

}  // namespace s21

#include "AVLTree.tpp"

#endif
