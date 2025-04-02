#ifndef S21_AVL_TPP
#define S21_AVL_TPP

#include "AVLTree.h"

namespace s21 {

/////////////////////////////////////
///////////    NODE    //////////////
/////////////////////////////////////

template <typename Key, typename T>
AVLTree<Key, T>::Node::Node(typename AVLTree<Key, T>::key_type key,
                            typename AVLTree<Key, T>::value_type value)
    : key_(key), value_(value) {
  count = 1;
  height = 1;

  left_ = nullptr;
  right_ = nullptr;
  parent_ = nullptr;
}

template <typename Key, typename T>
void AVLTree<Key, T>::Node::update_values() {
  count = (left_ != nullptr ? left_->count : 0) +
          (right_ != nullptr ? right_->count : 0) + 1;

  height = std::max(left_ != nullptr ? left_->height : 0,
                    right_ != nullptr ? right_->height : 0) +
           1;
}

template <typename Key, typename T>
int AVLTree<Key, T>::Node::balance_factor() {
  return (left_ != nullptr ? left_->height : 0) -
         (right_ != nullptr ? right_->height : 0);
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::Node::left_rotate() {
  Node* R = right_;
  right_ = R->left_;
  if (R->left_ != nullptr) {
    R->left_->parent_ = this;
  }
  R->left_ = this;

  R->parent_ = this->parent_;
  if (this->parent_ != nullptr) {
    if (this->parent_->left_ == this) {
      this->parent_->left_ = R;
    } else {
      this->parent_->right_ = R;
    }
  }
  this->parent_ = R;

  this->update_values();
  R->update_values();

  return R;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::Node::right_rotate() {
  Node* L = left_;
  left_ = L->right_;
  if (L->right_ != nullptr) {
    L->right_->parent_ = this;
  }
  L->right_ = this;

  // Обновляем родителя узла L
  L->parent_ = this->parent_;
  if (this->parent_ != nullptr) {
    if (this->parent_->left_ == this) {
      this->parent_->left_ = L;
    } else {
      this->parent_->right_ = L;
    }
  }

  this->parent_ = L;

  this->update_values();
  L->update_values();

  return L;
}

/////////////////////////////////////
///////////    AVLTree    ///////////
/////////////////////////////////////

// Constructors
template <typename Key, typename T>
AVLTree<Key, T>::AVLTree() : root_(nullptr), size_(0){};

template <typename Key, typename T>
AVLTree<Key, T>::AVLTree(std::initializer_list<value_type> const& items)
    : root_(nullptr) {
  for (const auto& item : items) {
    insert(item);
  }
}

// template <typename Key, typename T>
// AVLTree<Key, T>::AVLTree(const Key& key, const T& value) : root_{new
// Node{value_type{key, value}}} {}

// template <typename Key, typename T>
// AVLTree<Key, T>::AVLTree(const_reference elem) noexcept {
//   root_ = new Node(elem, elem);
// }

template <typename Key, typename T>
AVLTree<Key, T>::AVLTree(const AVLTree& other) noexcept
    : root_(nullptr), size_(0) {
  if (other.root_ != nullptr) {
    root_ = copy_node(other.root_);
    size_ = other.size_;
  }
}

template <typename Key, typename T>
AVLTree<Key, T>::AVLTree(AVLTree&& other) noexcept
    : root_(other.root_), size_(other.size_) {
  other.root_ = nullptr;
  other.size_ = 0;
}

template <typename Key, typename T>
AVLTree<Key, T>::~AVLTree() {
  clear();
}

// template <typename Key, typename T>
// AVLTree<Key, T>& AVLTree<Key, T>::operator=(const AVLTree& other) noexcept {
//   root_ = nullptr;
//   size_ = 0;

//   if (other.root_ != nullptr) {
//     root_ = copy_node(other.root_);
//     size_ = other.size_;
//   }
// }

template <typename Key, typename T>
AVLTree<Key, T>& AVLTree<Key, T>::operator=(AVLTree&& other) noexcept {
  if (this != &other) {
    root_ = other.root_;
    size_ = other.size_;

    other.root_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

/////////////////////////////////////////
////////    CAPACITY    /////////////////
/////////////////////////////////////////

template <typename Key, typename T>
bool AVLTree<Key, T>::empty() const {
  return size_ == 0 && root_ == nullptr;
}

template <typename Key, typename T>
size_t AVLTree<Key, T>::size() const {
  return size_;
}

template <typename Key, typename T>
size_t AVLTree<Key, T>::max_size() {
  return std::numeric_limits<size_t>::max();
}

//////////////////////////////////////////
////////    MODIFIERS    /////////////////
//////////////////////////////////////////

template <typename Key, typename T>
void AVLTree<Key, T>::clear() {
  this->clear_tree(this->root_);
  this->root_ = nullptr;
  size_ = 0;
}

template <typename Key, typename T>
std::pair<typename AVLTree<Key, T>::iterator, bool> AVLTree<Key, T>::insert(
    const Key& key, const T& value) {
  Node* inserted_node = nullptr;
  bool inserted = false;

  root_ = insert_node(root_, key, value, inserted_node, inserted);

  if (inserted) {
    return {iterator(inserted_node), true};
  } else {
    Node* existing_node = find_node(root_, key);
    return {iterator(existing_node), false};
  }
}

template <typename Key, typename T>
void AVLTree<Key, T>::erase(Iterator pos) {
  if (pos == Iterator(nullptr)) {
    return;
  }

  Node* node_to_remove = pos.get_node();
  if (!node_to_remove) {
    return;
  }

  root_ = erase_node(root_, node_to_remove->key_);
}

template <typename Key, typename T>
void AVLTree<Key, T>::swap(AVLTree& other) noexcept {
  std::swap(this->root_, other.root_);
  std::swap(this->size_, other.size_);
}

template <typename Key, typename T>
void AVLTree<Key, T>::merge(AVLTree& other) {
  if (this == &other) {
    return;
  }
  merge_nodes(other.root_);
  other.clear();
}

/////////////////////////////
///////    ITERATOR    //////
/////////////////////////////

template <typename Key, typename T>
typename AVLTree<Key, T>::iterator AVLTree<Key, T>::begin() {
  return Iterator(find_min_node(root_), this);
}

template <typename Key, typename T>
typename AVLTree<Key, T>::const_iterator AVLTree<Key, T>::cbegin() {
  return ConstIterator(find_min_node(root_), this);
}

template <typename Key, typename T>
typename AVLTree<Key, T>::iterator AVLTree<Key, T>::end() {
  return Iterator(nullptr, this);
}

template <typename Key, typename T>
typename AVLTree<Key, T>::const_iterator AVLTree<Key, T>::cend() {
  return ConstIterator(nullptr, this);
}

// Constructors and Destructor
template <typename Key, typename T>
AVLTree<Key, T>::ConstIterator::ConstIterator() noexcept
    : AVLTree_(nullptr), node_(nullptr) {}

template <typename Key, typename T>
AVLTree<Key, T>::ConstIterator::ConstIterator(Node* node, AVLTree* tree)
    : node_(node), AVLTree_(tree) {}

template <typename Key, typename T>
AVLTree<Key, T>::ConstIterator::ConstIterator(Node* node)
    : node_(node), AVLTree_(nullptr) {}

// Operators
template <typename Key, typename T>
bool AVLTree<Key, T>::ConstIterator::operator==(
    const ConstIterator& other) const {
  return this->node_ == other.node_;
}

template <typename Key, typename T>
bool AVLTree<Key, T>::ConstIterator::operator!=(
    const ConstIterator& other) const {
  return this->node_ != other.node_;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::value_type AVLTree<Key, T>::ConstIterator::operator*()
    const {
  if (node_ == nullptr) {
    throw std::out_of_range("Iterator is out of bounds[operator *]");
  }
  return node_->key_;
}

template <typename Key, typename T>
const typename AVLTree<Key, T>::value_type*
AVLTree<Key, T>::ConstIterator::operator->() const {
  return &(node_->key_);
}

template <typename Key, typename T>
typename AVLTree<Key, T>::ConstIterator&
AVLTree<Key, T>::ConstIterator::operator++() {
  if (node_ == nullptr) {
    throw std::out_of_range("Iterator is out of bounds[operator ++]");
  }

  node_ = AVLTree_->find_next_node(node_);

  return *this;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::ConstIterator
AVLTree<Key, T>::ConstIterator::operator++(int) {
  ConstIterator temp = *this;
  ++(*this);
  return temp;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::ConstIterator&
AVLTree<Key, T>::ConstIterator::operator--() {
  if (node_ == nullptr) {
    node_ = AVLTree_->find_last_node();
    if (node_ == nullptr) {
      throw std::out_of_range("Cannot decrement iterator in an empty tree.");
    }
  } else {
    node_ = AVLTree_->find_prev_node(node_);
    if (node_ == nullptr) {
      throw std::out_of_range("Iterator moved before the first element.");
    }
  }
  return *this;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::ConstIterator
AVLTree<Key, T>::ConstIterator::operator--(int) {
  ConstIterator temp = *this;
  --(*this);
  return temp;
}

// Iterator
template <typename Key, typename T>
typename AVLTree<Key, T>::value_type* AVLTree<Key, T>::Iterator::operator->() {
  return &(this->get_element());
}

template <typename Key, typename T>
typename AVLTree<Key, T>::reference
AVLTree<Key, T>::ConstIterator::get_element() const {
  if (!node_) {
    throw std::out_of_range("Iterator is invalid or points to end().");
  }
  return node_->key_;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::ConstIterator::get_node()
    const {
  return node_;
}

/////////////////////////////////////////
////////    LOOKUP FUNCTIONS    ///////////
/////////////////////////////////////////

template <typename Key, typename T>
bool AVLTree<Key, T>::contains(const Key& key) {
  Node* current = root_;

  while (current != nullptr) {
    if (key < current->key_) {
      current = current->left_;
    } else if (key > current->key_) {
      current = current->right_;
    } else {
      return true;
    }
  }

  return false;
}

template <typename Key, typename T>
void AVLTree<Key, T>::display() {
  printf("\n");
  if (root_ != nullptr)
    display(root_);
  else
    printf("Empty");
  printf("\n");
}

/////////////////////////////////////////
////////    HELP FUNCTIONS    ///////////
/////////////////////////////////////////

template <typename Key, typename T>
void AVLTree<Key, T>::clear_tree(Node* root) {
  if (root == nullptr) return;
  clear_tree(root->left_);
  clear_tree(root->right_);
  delete root;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::find_node(Node* node,
                                                           const Key& key) {
  if (node == nullptr || node->key_ == key) {
    return node;
  }
  if (key < node->key_) {
    return find_node(node->left_, key);
  } else {
    return find_node(node->right_, key);
  }
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::copy_node(Node* other_node) {
  if (other_node == nullptr) {
    return nullptr;
  }
  Node* new_node = new Node(other_node->key_, other_node->value_);

  new_node->left_ = copy_node(other_node->left_);
  if (new_node->left_ != nullptr) {
    new_node->left_->parent_ = new_node;
  }
  new_node->right_ = copy_node(other_node->right_);
  if (new_node->right_ != nullptr) {
    new_node->right_->parent_ = new_node;
  }

  return new_node;
}

template <typename Key, typename T>
void AVLTree<Key, T>::merge_nodes(Node* node) {
  if (node == nullptr) return;
  merge_nodes(node->left_);
  // insert(node->element_.first, node->element_.second);
  insert(node->key_, node->value_);
  // insert(value_type value)
  merge_nodes(node->right_);
}

template <typename Key, typename T>
void AVLTree<Key, T>::display(
    Node* current, int depth,
    int state) {  // state: 1 -> left, 2 -> right , 0 -> root
  if (current->left_) display(current->left_, depth + 1, 1);

  for (int i = 0; i < depth; i++) printf("     ");

  if (state == 1)  // left
    printf("┌───");
  // printf("/---");
  else if (state == 2)  // right
    printf("└───");
  // printf("\\---");

  if (current->parent_ != nullptr) {
    std::cout << "[" << current->value_ << "] - (k:" << current->key_ << ")"
              << std::endl;
  } else {
    std::cout << "[" << current->value_ << "] - (k:" << current->key_ << ")"
              << std::endl;
  }
  // if (current->parent_ != nullptr) {
  //     std::cout << "[" << current->value_ << "] - (p:" <<
  //     current->parent_->value_ << ")" << std::endl;
  // } else {
  //     std::cout << "[" << current->value_ << "] - (p:null)"  << std::endl;
  // }
  // if (current->parent_ != nullptr) {
  //     std::cout << "[" << current->value_ << "]" << std::endl;
  // } else {
  //     std::cout << "[" << current->value_ << "]" << std::endl;
  // }

  if (current->right_) display(current->right_, depth + 1, 2);
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::find_next_node(Node* node) {
  if (!node) return nullptr;
  if (node == find_max_node(root_)) {
    return nullptr;
  }
  if (node->right_) {
    return find_min_node(node->right_);
  }
  Node* parent = node->parent_;
  while (parent && node == parent->right_) {
    node = parent;
    parent = parent->parent_;
  }

  return parent;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::find_min_node(Node* node) {
  while (node && node->left_) {
    node = node->left_;
  }
  return node;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::find_max_node(Node* node) {
  while (node && node->right_ != nullptr) {
    node = node->right_;
  }
  return node;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::find_last_node() const {
  Node* current = root_;
  if (current == nullptr) {
    return nullptr;
  }
  while (current->right_ != nullptr) {
    current = current->right_;
  }
  return current;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::find_prev_node(Node* node) {
  if (!node) return nullptr;
  if (node->left_) {
    return find_max_node(node->left_);
  }
  Node* parent = node->parent_;
  while (parent && node == parent->left_) {
    node = parent;
    parent = parent->parent_;
  }
  return parent;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::erase_node(Node* node,
                                                            const Key& key) {
  if (!node) {
    return nullptr;
  }

  if (key < node->key_) {
    node->left_ = erase_node(node->left_, key);
    if (node->left_) {
      node->left_->parent_ = node;
    }
  } else if (key > node->key_) {
    node->right_ = erase_node(node->right_, key);
    if (node->right_) {
      node->right_->parent_ = node;
    }
  } else {
    if (!node->left_ && !node->right_) {
      // Узел — лист
      delete node;
      size_--;
      return nullptr;
    } else if (!node->left_ || !node->right_) {
      // Узел имеет одного потомка
      Node* child = node->left_ ? node->left_ : node->right_;
      child->parent_ = node->parent_;
      delete node;
      size_--;
      return child;
    } else {
      // Узел - два потомка
      node = replace_with_successor(node);
    }
  }

  // Балансировка
  node->update_values();
  int balance = node->balance_factor();

  // Левый перекос
  if (balance > 1 && node->left_->balance_factor() >= 0) {
    return node->right_rotate();
  }
  if (balance > 1 && node->left_->balance_factor() < 0) {
    node->left_ = node->left_->left_rotate();
    return node->right_rotate();
  }

  // Правый перекос
  if (balance < -1 && node->right_->balance_factor() <= 0) {
    return node->left_rotate();
  }
  if (balance < -1 && node->right_->balance_factor() > 0) {
    node->right_ = node->right_->right_rotate();
    return node->left_rotate();
  }

  return node;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::replace_with_successor(
    Node* node) {
  Node* successor = find_min_node(node->right_);
  Node* successor_parent = successor->parent_;
  Node* successor_right_child = successor->right_;

  node->key_ = successor->key_;
  node->value_ = successor->value_;

  if (successor_parent != node) {
    successor_parent->left_ = successor_right_child;
    if (successor_right_child) {
      successor_right_child->parent_ = successor_parent;
    }
  } else {
    node->right_ = successor_right_child;
    if (successor_right_child) {
      successor_right_child->parent_ = node;
    }
  }

  delete successor;
  size_--;
  return node;
}

template <typename Key, typename T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::insert_node(
    Node* node, const Key& key, const T& value, Node*& inserted_node,
    bool& inserted) {
  if (node == nullptr) {
    size_++;
    inserted_node = new Node(key, value);
    inserted = true;
    return inserted_node;
  }

  if (key < node->key_) {
    node->left_ = insert_node(node->left_, key, value, inserted_node, inserted);
    node->left_->parent_ = node;
  } else if (key > node->key_) {
    node->right_ =
        insert_node(node->right_, key, value, inserted_node, inserted);
    node->right_->parent_ = node;
  } else {
    inserted = false;
    return node;
  }

  node->update_values();
  int balance = node->balance_factor();

  // Левый перекос
  if (balance > 1 && key < node->left_->key_) {
    return node->right_rotate();
  }

  // Левый-правый перекос
  if (balance > 1 && key > node->left_->key_) {
    node->left_ = node->left_->left_rotate();
    return node->right_rotate();
  }

  // Правый перекос
  if (balance < -1 && key > node->right_->key_) {
    return node->left_rotate();
  }

  // Правый-левый перекос
  if (balance < -1 && key < node->right_->key_) {
    node->right_ = node->right_->right_rotate();
    return node->left_rotate();
  }

  return node;
}

}  // namespace s21

#endif