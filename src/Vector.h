#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#define S_CAP 10

namespace aisdi
{

template <typename Type>
class Vector
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

private:
  size_type length;
  size_type capacity;
  Type *head;
  Type *tail;

void l_move(const const_iterator& position_to, const const_iterator& position_from)
  {
    iterator to = iterator(position_to.pointee, *this);
    const_iterator from = position_from;

    for(; from != cend(); ++to, ++from)
      *to = *from;

    tail = to.pointee;
  }

void r_move(const const_iterator& position)
  {
    if(isEmpty())
    {
      ++tail;
      return;
    }

    iterator it = end() - 1;

    ++tail;

    for(; it != position; --it)
      *(it+1) = *it;

    *(it+1) = *it;
  }

Type* resize()
  {
    if(length + 1 <= capacity)
      return NULL;

    capacity = 2 * length;

    return new Type[capacity];
  }

public:

  Vector():length(0), capacity(S_CAP)
  {
    tail = head = new Type[S_CAP];
  }

  Vector(std::initializer_list<Type> l):Vector()
  {
    typename std::initializer_list<Type>::iterator it;

    for(it = l.begin(); it != l.end(); ++it)
      append(*it);
    //(void)l; // disables "unused argument" warning, can be removed when method is implemented.
    //throw std::runtime_error("TODO");
  }

  Vector(const Vector& other):Vector()
  {
    for(const_iterator it = other.cbegin(); it != other.cend(); ++it)
      append(*it);
    //(void)other;
    //throw std::runtime_error("TODO");
  }

  Vector(Vector&& other)
  {
    length = other.length;
    capacity = other.capacity;

    head = other.head;
    tail = other.tail;

    other.length = 0;
    other.capacity = S_CAP;
    other.tail = other.head = new Type[S_CAP];
    //(void)other;
    //throw std::runtime_error("TODO");
  }

  ~Vector()
  {
    tail = nullptr;

    length = 0;
    capacity = 0;

    delete[] head;

    head = nullptr;
  }

  Vector& operator=(const Vector& other)
  {
    erase(cbegin(), cend());

    for(const_iterator it = other.cbegin(); it != other.cend(); ++it)
      append(*it);

    return *this;
    //(void)other;
    //throw std::runtime_error("TODO");
  }

  Vector& operator=(Vector&& other)
  {
    Type *del = head;

    length = other.length;
    capacity = other.capacity;

    head = other.head;
    tail = other.tail;

    other.length = 0;
    other.capacity = S_CAP;
    other.tail = other.head = new Type[S_CAP];

    delete[] del;

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
    ++length;
    Type *temp;

    //resize needed
    if((temp = resize()))
    {
      Type *del = head;

      Type  *i;
      const_iterator it = cbegin();

      for(i = temp; it != cend(); ++i, ++it)
        *i = *it;

      head = temp;
      tail = i;

      delete[] del;
    }
    *(tail++) = item;
    //(void)item;
    //throw std::runtime_error("TODO");
  }

  void prepend(const Type& item)
  {
    Type *temp;

    //resize needed
    if((temp = resize()))
    {
      Type *del = head;

      *temp = item;

      Type *i;
      const_iterator it = cbegin();

      for(i = temp + 1; it != cend(); ++i, ++it)
        *i = *it;

      head = temp;
      tail = i;

      delete[] del;
    }
    else
    {
      r_move(cbegin());

      *head = item;
    }

    ++length;
    //(void)item;
    //throw std::runtime_error("TODO");
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(insertPosition == cend())
    {
      append(item);
      return;
    }

    Type *temp;

    //resize needed
    if((temp = resize()))
    {
      Type *del = head;

      Type *i;
      const_iterator it = cbegin();

      //copy from beg to insPos-1
      for(i = temp; it != insertPosition; ++i, ++it)
        *i = *it;

      //insert element
      *i = item;

      //copy from insPos to end
      for(++i; it != cend(); ++i, ++it)
        *i = *it;

      head = temp;
      tail = i;

      delete[] del;
    }

    //right shift
    else
    {
      r_move(insertPosition);

      *( (iterator)insertPosition ) = item;
    }

    ++length;
    //(void)insertPosition;
    //(void)item;
    //throw std::runtime_error("TODO");
  }

  Type popFirst()
  {
    if(isEmpty())
      throw std::logic_error("Object cannot be popped.");

    Type obj = *head;

    l_move(cbegin(), cbegin()+1);

    --length;

    return obj;
    //throw std::runtime_error("TODO");
  }

  Type popLast()
  {
    if(isEmpty())
      throw std::logic_error("Object cannot be popped.");

    --length;

    return *(--tail);
    //throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& position)
  {
    if(isEmpty())
      throw std::out_of_range("Erasing in an empty vector.");

    if(position == cend())
      throw std::out_of_range("Erasing at end iterator");

    if(position == cend()-1)
    {
      popLast();
      return;
    }

    l_move(position, position+1);

    --length;
    //(void)possition;
    //throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(isEmpty())
      throw std::out_of_range("Erasing in an empty vector.");

    if(firstIncluded == lastExcluded)
      return;

    length -= lastExcluded.pointee - firstIncluded.pointee;

    l_move(firstIncluded, lastExcluded);
    //(void)firstIncluded;
    //(void)lastExcluded;
    //throw std::runtime_error("TODO");
  }

  iterator begin()
  {
    return iterator(head, *this);
    //throw std::runtime_error("TODO");
  }

  iterator end()
  {
    return iterator(tail, *this);
    //throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
    return const_iterator(head, *this);
    //throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
    return const_iterator(tail, *this);
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

template <typename Type> //done
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

private:
  Type *pointee;
  const Vector<Type>& vec;

  friend void aisdi::Vector<Type>::erase(const ConstIterator&, const ConstIterator&);
  friend void aisdi::Vector<Type>::l_move(const ConstIterator&, const ConstIterator&);

public:

  explicit ConstIterator(Type *pnt, const Vector<Type>& vtr): pointee(pnt), vec(vtr)
  {}

  reference operator*() const
  {
    if(pointee == vec.tail)
      throw std::out_of_range("Out of range.");

    return *pointee;
    //throw std::runtime_error("TODO");
  }

  ConstIterator& operator++()
  {
    if(pointee == vec.tail)
      throw std::out_of_range("Out of range.");

    ++pointee;

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
    if(pointee == vec.head)
      throw std::out_of_range("Out of range.");

    --pointee;

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
    if(pointee + d > vec.tail)
      throw std::out_of_range("Out of range.");

    return ConstIterator(pointee + d, vec);
    //(void)d;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator-(difference_type d) const
  {
    if(pointee - d < vec.head)
      throw std::out_of_range("Out of range.");

    return ConstIterator(pointee - d, vec);
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
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(Type *pnt, const Vector<Type>& vtr): ConstIterator(pnt, vtr)
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

#endif // AISDI_LINEAR_VECTOR_H
