#ifndef S21_LIST_TPP
#define S21_LIST_TPP

#include <limits>
#include <stdexcept>

#include "s21_list.h"

namespace s21 {

//  Итератор класса s21_list
template <typename T>
typename list<T>::reference list<T>::ListIterator::operator*() const {
  if (this->ptr == nullptr) {  // Проверяем, если итератор указывает на конец
                               // списка (nullptr)
    throw std::out_of_range("Dereferencing end iterator");
  }
  return this->ptr->data;  // возвращаем данные текущего узла
}

template <typename T>
typename list<T>::ListIterator list<T>::ListIterator::operator++() {
  if (this->ptr == nullptr) {  // если указатель уже указывает на конец nullptr,
                               // ничего не делаем
    return *this;
  } else if (this->ptr ==
             this->current->tail) {  // если указатель указывает на последний
                                     // элемент, перемещаем его на конец nullptr
    this->ptr = nullptr;
  } else {  // иначе просто перемещаем итератор на следующий узел
    this->ptr = this->ptr->ptr_next;
  }
  return *this;
}

template <typename T>
typename list<T>::ListIterator list<T>::ListIterator::operator++(int) {
  ListIterator temp = *this;  // сохраняем текущее состояние итератора
  this->ptr = this->ptr ? this->ptr->ptr_next : nullptr;
  return temp;  // возвращаем сохраненный итератор
}

template <typename T>
typename list<T>::ListIterator list<T>::ListIterator::operator--() {
  if (this->ptr == nullptr) {  // если итератор указывает на конец nullptr, то
                               // перемещаем его на tail
    this->ptr = this->current->tail;
  } else if (this->ptr ==
             this->current->head) {  // если итератор указывает на начало,
                                     // оставляем его на этом месте
  } else {  // в остальных случаях просто перемещаем его на предыдущий узел
    this->ptr = this->ptr->ptr_prev;
  }
  return *this;
}

template <typename T>
typename list<T>::ListIterator list<T>::ListIterator::operator+(int n) {
  if (n == 0) return *this;
  iterator temp = *this;
  while (n-- > 0) ++temp;
  return temp;
}

template <typename T>
typename list<T>::ListIterator list<T>::ListIterator::operator--(int) {
  ListIterator temp = *this;
  if (this->ptr == nullptr) {
    this->ptr = this->current->tail;
  } else {
    this->ptr = this->ptr->ptr_prev;
  }
  return temp;
}

template <typename T>
bool list<T>::ListIterator::operator==(const ListIterator &other) const {
  return this->ptr == other.ptr;
}

template <typename T>
bool list<T>::ListIterator::operator!=(const ListIterator &other) const {
  return this->ptr != other.ptr;
}
// ->

// Класс s21_list

// Конструктор создания списка из n элементов
template <typename T>
list<T>::list(size_type n) {
  head = nullptr;
  tail = nullptr;
  count_list = 0;
  for (size_type i = 0; i < n; ++i) {
    this->push_back(T());  // инициализация значением по умолчанию для типа T
                           // this->push_back(0);
  }
}

// Конструктор инициализации list списком
template <typename T>
list<T>::list(std::initializer_list<value_type> const &items)
    : head(nullptr), tail(nullptr), count_list(0) {
  for (const auto &item : items) {
    push_back(item);
  }
}
// ->

// Конструктор копирования
template <typename T>
list<T>::list(const list &l) : head(nullptr), tail(nullptr), count_list(0) {
  Node *ptr = l.head;
  while (ptr != nullptr) {
    this->push_back(ptr->data);
    ptr = ptr->ptr_next;
  }
}
// ->

// Конструктор перемещения
template <typename T>
list<T>::list(list &&l) : head(l.head), tail(l.tail), count_list(l.count_list) {
  l.head = l.tail = nullptr;
  l.count_list = 0;
}
// ->

// Оператор копирования
template <typename T>
list<T> &list<T>::operator=(const list &l) {
  if (this != &l) {
    list copy(l);
    *this = std::move(copy);
  }
  return *this;
}
// ->

// Оператор перемещения
template <typename T>
list<T> &list<T>::operator=(list &&l) {
  if (this != &l) {
    this->clear();  // освобождаем текущие данные
    this->head = l.head;
    this->tail = l.tail;
    this->count_list = l.count_list;
    l.head = nullptr;  // обнуляем указатели у перемещенного объекта
    l.tail = nullptr;
    l.count_list = 0;
  }
  return *this;
}
//->

// Деструктор
template <typename T>
list<T>::~list() {
  if (this->head != nullptr) this->clear();
}

template <typename T>
void list<T>::clear() {
  if (head == nullptr) return;  // список уже пуст, ничего делать не нужно
  Node *current = this->head;
  Node *next = nullptr;
  while (current != nullptr) {
    next = current->ptr_next;
    delete current;
    current = next;
  }
  head = nullptr;
  tail = nullptr;
  count_list = 0;
}

template <typename T>
typename list<T>::iterator list<T>::begin() {
  iterator cur_iter = iterator(this);
  cur_iter.ptr = cur_iter.current->head;
  return cur_iter;
}

template <typename T>
typename list<T>::iterator list<T>::end() {
  iterator cur_iter = iterator(this);
  cur_iter.ptr = nullptr;
  return cur_iter;
}

template <typename T>
typename list<T>::const_iterator list<T>::begin() const {
  return const_iterator(const_cast<list<T> *>(this)->begin());
}

template <typename T>
typename list<T>::const_iterator list<T>::end() const {
  return const_iterator(const_cast<list<T> *>(this)->end());
}

// Вставка множества элементов в начало списка
template <typename T>
template <typename... Args>
void list<T>::insert_many_front(Args &&...args) {
  iterator ptr = this->begin();
  for (const auto &arg : {args...}) {
    this->insert(ptr, arg);
  }
}

template <typename T>
template <typename... Args>
void list<T>::insert_many_back(Args &&...args) {
  for (const auto &arg : {args...}) {
    this->push_back(arg);
  }
}

template <typename T>
template <typename... Args>
typename list<T>::iterator list<T>::insert_many(const_iterator pos,
                                                Args &&...args) {
  // переменная для хранения последней позиции
  iterator last_inserted_pos = pos;
  list<T> tmp{args...};
  for (const auto &arg : tmp) {
    // вставляем элемент и обновляем позицию
    last_inserted_pos = this->insert(pos, arg);
    ++pos;  // обновляем итератор позиции для следующей вставки
  }
  // возвращаем итератор последнего вставленного элемента
  return last_inserted_pos;
}

// Вставляет элемент в указанную позицию списка
template <typename T>
typename list<T>::iterator list<T>::insert(iterator pos,
                                           const_reference value) {
  Node *new_element = new Node(value);  // создаем новый элемент списка
  if (head == nullptr) {  // если список пуст, добавляем элемент как первый и
                          // единственный
    head = tail = new_element;
  } else if (pos.ptr == head) {  // если вставляем в начало
    new_element->ptr_next = head;
    head->ptr_prev = new_element;
    head = new_element;
  } else if (pos.ptr == nullptr ||
             pos.ptr == tail->ptr_next) {  // если вставляем в конец
    new_element->ptr_prev = tail;
    tail->ptr_next = new_element;
    tail = new_element;
  } else {  // вставляем в середину
    Node *temp = pos.ptr->ptr_prev;
    temp->ptr_next = new_element;
    new_element->ptr_prev = temp;
    new_element->ptr_next = pos.ptr;
    pos.ptr->ptr_prev = new_element;
  }

  count_list++;  // увеличиваем счетчик элементов

  return iterator(this, new_element);  // возвращаем итератор на новый элемент
}
// ->

// Удаляет элемент из указанной позиции списка
template <typename T>
void list<T>::erase(iterator pos) {
  if (pos.ptr == pos.current->head) {
    pos.current->pop_front();
  } else if (pos.ptr == pos.current->tail) {
    pos.current->pop_back();
  } else {
    pos.ptr->ptr_prev->ptr_next = pos.ptr->ptr_next;
    pos.ptr->ptr_next->ptr_prev = pos.ptr->ptr_prev;
    delete pos.ptr;
    pos.ptr = nullptr;
    pos.current->count_list--;
  }
}
// ->

// Метод класса list для изменения порядка элементов списка
template <typename T>
void list<T>::reverse() {
  Node *ptr = this->head;
  std::swap(this->head, this->tail);
  while (ptr != nullptr) {
    std::swap(ptr->ptr_next, ptr->ptr_prev);
    ptr = ptr->ptr_prev;
  }
}
// ->

// Метод класса list для обмена значениями двух списков
template <typename T>
void list<T>::swap(list &other) {
  std::swap(this->head, other.head);
  std::swap(this->tail, other.tail);
  std::swap(this->count_list, other.count_list);
}
// ->

// Метод класса list для добавления элемента в конец списка
template <typename T>
void list<T>::push_back(const_reference data) {
  if (tail == nullptr)
    tail = head = new Node(data);
  else {
    tail->ptr_next = new Node(data);
    Node *temp_ptr = tail;
    tail = tail->ptr_next;
    tail->ptr_prev = temp_ptr;
  }
  ++count_list;
}
// ->

// Метод класса list для добавления элемента в начало списка
template <typename T>
void list<T>::push_front(const_reference data) {
  if (head == nullptr)
    head = tail = new Node(data);
  else {
    head->ptr_prev = new Node(data);
    Node *temp_ptr = head;
    head = head->ptr_prev;
    head->ptr_next = temp_ptr;
  }
  ++count_list;
}
// ->

template <typename T>
void list<T>::splice(const_iterator pos, list &other) {
  if (other.empty()) {  // проверяем пуст ли список other
    return;
  }

  Node *parent =
      pos.ptr
          ? pos.ptr->ptr_prev
          : nullptr;  // получаем указатель на элемент списка, который находится
                      // перед позицией pos, если такой элемент существует
  Node *current = pos.ptr;  // текущая позиция для вставки

  if (this->empty()) {  // если вставляем в пустой список
    this->head = other.head;
    this->tail = other.tail;
  } else {
    other.tail->ptr_next =
        current;  // привязываем хвост списка other к текущему элементу pos
    if (current) {
      current->ptr_prev = other.tail;
    }

    if (parent) {  // если есть предыдущий элемент перед pos, привязываем его к
                   // голове списка other
      parent->ptr_next = other.head;
      other.head->ptr_prev = parent;
    } else {
      this->head = other.head;  // если вставляем в начало списка, обновляем
                                // указатель на голову списка pos.current
    }
  }
  other.head = nullptr;  // обнуляем список other
  other.tail = nullptr;

  this->count_list += other.count_list;  // обновляем количество элементов
  other.count_list = 0;
}

// Метод класса list для удаления последнего элемента списка
template <typename T>
void list<T>::pop_back() {
  if (this->tail == nullptr) {
    throw std::runtime_error("free(): invalid pointer");
  } else {
    Node *temp_ptr = tail;
    tail = tail->ptr_prev;
    if (tail != nullptr)
      tail->ptr_next = nullptr;
    else
      head = nullptr;
    delete temp_ptr;
    --count_list;
  }
}
// ->

// Метод класса list для удаления первого элемента списка
template <typename T>
void list<T>::pop_front() {
  if (this->head == nullptr) {
    throw std::runtime_error("free(): invalid pointer");
  } else {
    Node *temp_ptr = head;
    head = head->ptr_next;
    if (head != nullptr)
      head->ptr_prev = nullptr;
    else
      tail = nullptr;
    delete temp_ptr;
    --count_list;
  }
}

template <typename T>
void list<T>::unique() {
  iterator current = this->begin() + 1;
  for (; current != this->end(); ++current) {
    if (current.ptr->data == current.ptr->ptr_prev->data) {
      iterator temp = current + 1;
      this->erase(current);
      current = temp;
      --current;
    }
  }
}
//->

template <typename T>
size_t list<T>::max_size() {
  return std::numeric_limits<size_type>::max() / sizeof(Node) / DEL2;
}

// Вспомогательная функция для сортировки списка
template <typename T>
typename list<T>::Node *list<T>::partition(Node *low, Node *high) {
  T pivot = high->data;
  Node *i = low->ptr_prev;

  for (Node *j = low; j != high; j = j->ptr_next) {
    if (j->data < pivot) {
      i = (i == nullptr) ? low : i->ptr_next;
      std::swap(i->data, j->data);
    }
  }
  i = (i == nullptr) ? low : i->ptr_next;
  std::swap(i->data, high->data);
  return i;
}
// ->

// Вспомогательный метод для сортировки списка
template <typename T>
void list<T>::quick_sort(Node *low, Node *high) {
  if (high != nullptr && low != high && low != high->ptr_next) {
    Node *p = partition(low, high);
    quick_sort(low, p->ptr_prev);
    quick_sort(p->ptr_next, high);
  }
}

template <typename T>
void list<T>::merge(list &other) {
  if (this == &other) {
    return;  // проверка на слияние с самим собой
  }
  const_iterator ptr = this->end();
  --ptr;
  this->splice(ptr, other);
  this->sort();
}

template <typename T>
typename list<T>::const_reference list<T>::front() const {
  if (this->empty()) {
    throw std::out_of_range("List is empty");
  }
  return head->data;
}

template <typename T>
typename list<T>::const_reference list<T>::back() const {
  if (tail == nullptr) {
    throw std::out_of_range("List is empty");
  }
  return tail->data;
}

}  // namespace s21

#endif
