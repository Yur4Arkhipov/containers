#ifndef S21_LIST_H
#define S21_LIST_H

#include <cstddef>
#include <initializer_list>

#ifdef __APPLE__
#define DEL2 1  // If on macOS, do not divide by 2
#else
#define DEL2 2
#endif

namespace s21 {

template <typename T>
class list {
  // Внутриклассовое переопределение типов
  using value_type = T;  // Определяет тип данных элементов списка
  using size_type = std::size_t;  // Определяет размер списка
  using reference = T &;  // Определяет ссылку на значение элемента списка
  // Определяет ссылку на const значение элемента списка
  using const_reference = const T &;
  // ->

 public:
  friend class ListIterator;

 private:
  struct Node {  // Структура списка
    Node *ptr_next;  // Указатель на следующий элемент списка
    Node *ptr_prev;  // Указатель на предыдущий элемент списка
    value_type data;  // Значение элемента списка

    Node(value_type data_ = value_type(), Node *ptr_next_ = nullptr,
         Node *ptr_prev_ = nullptr) {
      this->data = data_;
      this->ptr_next = ptr_next_;
      this->ptr_prev = ptr_prev_;
    }
  };

  Node *head;
  Node *tail;
  size_type count_list;

  // Вспомогательные методы для сортировки
  void quick_sort(Node *low, Node *high);
  Node *partition(Node *low, Node *high);
  // ->

 public:
  class ListIterator {
    friend class list;

   private:
    // Указатель для хранения текущего элемента списка на который указывает
    // итератор
    Node *ptr;
    list *current;  // Указатель на список

   public:
    ListIterator(list *_current) : ptr(_current->head), current(_current){};
    ListIterator(list *_current, Node *_ptr) : ptr(_ptr), current(_current){};

    reference operator*() const;
    ListIterator operator++();
    ListIterator operator++(int);
    ListIterator operator+(int n);
    ListIterator operator--();
    ListIterator operator--(int);
    bool operator==(const ListIterator &other) const;
    bool operator!=(const ListIterator &other) const;
    ~ListIterator() { ptr = nullptr; }
  };

  class ListConstIterator : public ListIterator {
   public:
    const_reference operator*() const { return ListIterator::operator*(); }
    ListConstIterator(const ListIterator &_current) : ListIterator(_current){};
  };

  using iterator = class ListIterator;  // Итератор списка
  // Определяет тип const для перебора элементов списка
  using const_iterator = class ListConstIterator;

  list() noexcept : head(nullptr), tail(nullptr), count_list(0) {}
  list(size_type n);
  ~list();

  list(const list &l);  // Конструктор копирования
  list(list &&l);       // Конструктор перемещения

  list &operator=(const list &l);  // Перегрузка оператора копирования
  list &operator=(list &&l);  // Перегрузка оператора перемещения

  // Инициализация списком
  list(std::initializer_list<value_type> const &items);

  void push_back(const_reference data);  // Добавить элемент в начало списка
  void push_front(const_reference data);  // Добавить элемент в конец списка

  const_reference front() const;  // Доступ к первому элементу списка
  const_reference back() const;  // Доступ к последнему элементу списка

  iterator begin();  // Возвращает итератор на начало
  iterator end();    // Возвращает итератор на конец

  const_iterator begin() const;  // Возвращает итератор const на начало
  const_iterator end() const;  // Возвращает итератор const на конец
  // Возвращает 1 если конетйнер пустой, в противном случае возвращает 0
  bool empty() const { return count_list == 0; }
  // Возвращает количество элементов в списке
  size_type size() { return this->count_list; }
  size_type max_size();  //	returns the maximum possible number of elements

  void clear();  // Очистка списка
  // Вставляет элемент в указанную позицию и возвращает итератор
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);  // Удаляет элемент из указанной позиции
  void pop_back();  // Удаляет элемента из конца списка
  void pop_front();  // Удаляет элемент из начала списка
  void swap(list &other);  // Обменивает значения двух списков
  void merge(list &other);  //	Объединяет два списка и сортирует их
  // Переносит элементы другого списка в указанную позицию
  void splice(const_iterator pos, list &other);
  void reverse();  //  Меняет порядок элементов
  // Удаляет повторяющиеся элементы списка идущие друг за другом
  void unique();
  // 	Сортировка списка
  void sort() { quick_sort(this->head, this->tail); }

  // Bonus of method's
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args);

  template <typename... Args>
  void insert_many_back(Args &&...args);

  template <typename... Args>
  void insert_many_front(Args &&...args);
  // ->
};
}  // namespace s21

#include "s21_list.tpp"

#endif