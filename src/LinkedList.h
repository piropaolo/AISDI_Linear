#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  struct Node
  {
    Type *obj;
    Node *prev;
    Node *next;
    Node(): obj(nullptr), prev(nullptr), next(nullptr){}
    Node(Type object): Node() {obj = new Type (object);}
    ~Node() {delete obj;}
  };

private:
  Node *head;
  Node *tail;
  size_type length;

public:

  LinkedList(): head(nullptr), tail(nullptr), length(0)
  {
    head = new Node;
    tail = new Node;
    head->next = tail;
    tail->prev = head;
  }

  LinkedList(std::initializer_list<Type> l):LinkedList()
  {
    for (auto it = l.begin(); it != l.end(); ++it)
      append(*it);
    //(void)l; // disables "unused argument" warning, can be removed when method is implemented.
    //throw std::runtime_error("TODO");
  }

  LinkedList(const LinkedList& other):LinkedList()
  {
    for (auto it = other.begin(); it != other.end(); it++)
      append(*it);
    //(void)other;
    //throw std::runtime_error("TODO");
  }

  LinkedList(LinkedList&& other):LinkedList()
  {
    while (!other.isEmpty())
    {
      append(*(other.begin()));
      other.erase(other.begin());
    }
    //(void)other;
    //throw std::runtime_error("TODO");
  }

  ~LinkedList()
  {
    while (!isEmpty())
      erase(begin());
    delete tail;
    delete head;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    while (!isEmpty())
      erase(begin());
    for (auto it = other.begin(); it != other.end(); it++)
      append(*it);

    return *this;
    //(void)other;
    //throw std::runtime_error("TODO");
  }

  LinkedList& operator=(LinkedList&& other)
  {
    while (!isEmpty())
      erase(begin());
    while (!other.isEmpty()) {
      append(*(other.begin()));
      other.erase(other.begin());
    }

    return *this;
    //(void)other;
    //throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
    return !length;
    //throw std::runtime_error("TODO");
  }

  size_type getSize() const
  {
    return length;
    //throw std::runtime_error("TODO");
  }

  void append(const Type& item)
  {
    Node *temp = new Node(item);

    temp->prev = tail->prev;
    temp->next = tail;

    tail->prev->next = temp;
    tail->prev= temp;

    ++length;
    //(void)item;
    //throw std::runtime_error("TODO");
  }

  void prepend(const Type& item)
  {
    Node *temp = new Node(item);

    temp->prev = head;
    temp->next = head->next;

    head->next->prev = temp;
    head->next= temp;

    ++length;
    //(void)item;
    //throw std::runtime_error("TODO");
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if (insertPosition == begin() )
      prepend(item);
    else if (insertPosition == end())
      append(item);
    else {
      Node* temp = new Node(item);

      temp->next = insertPosition.pointee;
      temp->prev = insertPosition.pointee->prev;

      insertPosition.pointee->prev->next = temp;
      insertPosition.pointee->prev = temp;
      ++length;
    }
    //(void)insertPosition;
    //(void)item;
    //throw std::runtime_error("TODO");
  }

  Type popFirst()
  {
    if (isEmpty())
      throw std::logic_error("Object cannot be popped.");

    Type obj = *begin();
    erase(begin());
    return obj;
    //throw std::runtime_error("TODO");
  }

  Type popLast()
  {
    if (isEmpty())
      throw std::logic_error("Object cannot be popped.");

    Type obj = *(--end());
    erase((--end()));
    return obj;
    //throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& position)
  {
    if (position == end())
      throw std::out_of_range("Object cannot be erased.");
    position.pointee->next->prev = position.pointee->prev;
    position.pointee->prev->next = position.pointee->next;
    delete position.pointee;
    --length;
    //(void)position;
    //throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    auto it = firstIncluded;
    while (it != lastExcluded) {
      it++;
      erase(it - 1);
    }
    //(void)firstIncluded;
    //(void)lastExcluded;
    //throw std::runtime_error("TODO");
  }

  iterator begin()
  {
    return iterator(head->next);
    //throw std::runtime_error("TODO");
  }

  iterator end()
  {
    return iterator(tail);
    //throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
    return const_iterator(head->next);
    //throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
    return const_iterator(tail);
    //throw std::runtime_error("TODO");
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

private:
  Node *pointee;
  friend void LinkedList <Type>::insert(const const_iterator&, const Type&);
  friend void LinkedList <Type>::erase(const const_iterator&);

public:

  explicit ConstIterator(Node *pnt = nullptr) : pointee(pnt)
  {}

  reference operator*() const
  {
    if(pointee->obj == nullptr)
        throw std::out_of_range("Out of range.");
    return *(pointee->obj);
    //throw std::runtime_error("TODO");
  }

  ConstIterator& operator++()
  {
    if (pointee->next == nullptr)
        throw std::out_of_range("Out of range.");
    pointee = pointee->next;
    return *this;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int)
  {
    auto result = *this;
    operator++();
    return result;
    //throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
    if (pointee->prev->prev == nullptr)
        throw std::out_of_range("Out of range.");
    pointee = pointee->prev;
    return *this;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
    auto result = *this;
    operator--();
    return result;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator+(difference_type d) const
  {
    auto result = *this;
    for (difference_type i = 0; i < d; ++i)
        ++result;
    return result;
    //(void)d;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator-(difference_type d) const
  {
    auto result = *this;
    for (difference_type i = 0; i < d; ++i)
        --result;
    return result;
    //(void)d;
    //throw std::runtime_error("TODO");
  }

  bool operator==(const ConstIterator& other) const
  {
    return this->pointee == other.pointee;
    //(void)other;
    //throw std::runtime_error("TODO");
  }

  bool operator!=(const ConstIterator& other) const
  {
    return this->pointee != other.pointee;
    //(void)other;
    //throw std::runtime_error("TODO");
  }
};

template <typename Type> //done
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(Node *pnt = nullptr): const_iterator(pnt)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
