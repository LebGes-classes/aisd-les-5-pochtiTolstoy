#pragma once

#include "BinaryHeap.hpp"

template <typename Value> class PriorityQueue {
public:
  PriorityQueue() = default;
  ~PriorityQueue() = default;
  PriorityQueue(const PriorityQueue &) = default;
  PriorityQueue(PriorityQueue &&) = default;
  PriorityQueue &operator=(const PriorityQueue &) = default;
  PriorityQueue &operator=(PriorityQueue &&) = default;

  using value_type = std::pair<size_t, Value>;

  void Enqueue(const Value &value, size_t priority = 0) {
    heap_.Insert({priority, value});
  }

  void Enqueue(Value &&value, size_t priority = 0) {
    heap_.Insert({priority, std::move(value)});
  }

  void Dequeue() { heap_.ExtractMin(); }

  value_type Peek() const { return heap_.PeekMin(); }

  void DecreasePriority(const Value &value, size_t new_priority = 0) {
    heap_.DecreaseKey(value, new_priority);
  }

  bool IsEmpty() const { return heap_.IsEmpty(); }

  size_t Size() const { return heap_.Size(); }

private:
  BinaryHeap<Value> heap_;
};
