#pragma once

#include "BinaryHeap.hpp"

template <typename Value> class PriorityQueue : public BinaryHeap<Value> {
public:
  using value_type = std::pair<size_t, Value>;

  PriorityQueue() = default;
  ~PriorityQueue() = default;

  void Enqueue(const Value &value, size_t priority = 0) {
    BinaryHeap<Value>::Insert({priority, value});
  }

  void Dequeue() { BinaryHeap<Value>::ExtractMin(); }

  value_type Peek() const { return BinaryHeap<Value>::PeekMin(); }

  void DecreasePriority(const Value &value, size_t new_priority = 0) {
    BinaryHeap<Value>::DecreaseKey(new_priority, value);
  }
};
