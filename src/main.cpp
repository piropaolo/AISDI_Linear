#include <cstddef>
#include <cstdlib>
#include <string>

#include <iostream>
#include <chrono>
#include <ctime>

#include "Vector.h"
#include "LinkedList.h"

namespace
{

template <typename T>
using LinkedList = aisdi::LinkedList<T>;
template <typename T>
using Vector = aisdi::Vector<T>;

void performTest1(std::size_t n)
{
  LinkedList<std::string> collection;
  std::chrono::time_point<std::chrono::system_clock> start, end;

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
    collection.append("DONE");
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "LinkedList      Append time:        " << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
    collection.prepend("DONE");
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "LinkedList      Prepend time:       " << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
    collection.erase(collection.begin());
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "LinkedList      EraseBegin time:    " << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
    collection.erase(collection.end()-1);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "LinkedList      EraseEnd time:      " << elapsed_seconds.count() << "s\n";
}

void performTest2(std::size_t n)
{
  Vector<std::string> collection;
  std::chrono::time_point<std::chrono::system_clock> start, end;

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
    collection.append("DONE");
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "Vector          Append time:        " << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
    collection.prepend("DONE");
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "Vector          Prepend time:       " << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
    collection.erase(collection.begin());
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "Vector          EraseBegin time:    " << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < n; ++i)
    collection.erase(collection.end()-1);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "Vector          EraseEnd time:      " << elapsed_seconds.count() << "s\n";
}

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  //for (std::size_t i = 0; i < repeatCount; ++i)
  performTest1(repeatCount);
  performTest2(repeatCount);
  return 0;
}
