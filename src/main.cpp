#include <iostream>

#include "PriorityQueue.hpp"

int main() {
  PriorityQueue<std::string> queue;
  queue.Enqueue("~~~", 1);
  queue.Enqueue("!", 3);
  queue.Enqueue("world", 2);
  queue.Enqueue("hello", 1);
  while (!queue.IsEmpty()) {
    const auto &[priority, data] = queue.Peek();
    std::cout << "Priority : " << priority << ", Data : " << data << std::endl;
    queue.Dequeue();
  }
}
