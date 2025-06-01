#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename Value> class BinaryHeap {
public:
  using value_type = std::pair<size_t, Value>;
  using key_type = size_t;
  using mapped_type = Value;

  BinaryHeap() = default;
  ~BinaryHeap() = default;
  BinaryHeap(const BinaryHeap &) = default;
  BinaryHeap(BinaryHeap &&) = default;
  BinaryHeap &operator=(const BinaryHeap &) = default;
  BinaryHeap &operator=(BinaryHeap &&) = default;

  void Insert(const value_type &new_pair);
  void Insert(value_type &&new_pair);
  void DecreaseKey(const Value &value, key_type new_key);
  value_type ExtractMin();
  const value_type &PeekMin() const;
  bool IsEmpty() const noexcept;
  size_t Size() const noexcept;

private:
  std::vector<value_type> data_;
  std::unordered_map<Value, size_t> vk_map_;

  void SieveUp(size_t index);
  void SieveDown(size_t index);
  void SwapData(size_t i1, size_t i2);
};

template <typename Value>
void BinaryHeap<Value>::SwapData(size_t i1, size_t i2) {
  if (i1 == i2)
    return;
  assert(i1 < data_.size() && i2 < data_.size());
  std::swap(data_[i1], data_[i2]);
  vk_map_[data_[i1].second] = i1;
  vk_map_[data_[i2].second] = i2;
}

template <typename Value> bool BinaryHeap<Value>::IsEmpty() const noexcept {
  return data_.empty();
}

template <typename Value> size_t BinaryHeap<Value>::Size() const noexcept {
  return data_.size();
}

template <typename Value>
void BinaryHeap<Value>::Insert(const value_type &new_pair) {
  if (vk_map_.find(new_pair.second) != vk_map_.end()) {
    throw std::invalid_argument("Duplicate value insertion");
  }
  data_.push_back(new_pair);
  size_t insert_index = data_.size() - 1;
  vk_map_[new_pair.second] = insert_index;
  SieveUp(insert_index);
}

template <typename Value>
void BinaryHeap<Value>::Insert(value_type &&new_pair) {
  if (vk_map_.find(new_pair.second) != vk_map_.end()) {
    throw std::invalid_argument("Duplicate value insertion");
  }
  data_.push_back(std::move(new_pair));
  size_t insert_index = data_.size() - 1;
  vk_map_[data_.back().second] = insert_index;
  SieveUp(insert_index);
}

template <typename Value>
void BinaryHeap<Value>::DecreaseKey(const Value &value, key_type new_key) {
  auto it = vk_map_.find(value);
  if (it == vk_map_.end()) {
    throw std::invalid_argument("Value not found");
  }

  size_t index = it->second;
  if (data_[index].first <= new_key) {
    return;
  }

  data_[index].first = new_key;
  SieveUp(index);
}

template <typename Value> auto BinaryHeap<Value>::ExtractMin() -> value_type {
  if (IsEmpty()) {
    throw std::out_of_range("Cannot extract from an empty heap");
  }

  value_type elem = data_.front();
  vk_map_.erase(elem.second);

  if (data_.size() == 1) {
    data_.pop_back();
    return elem;
  }

  data_.front() = std::move(data_.back());
  data_.pop_back();
  vk_map_[data_.front().second] = 0;
  SieveDown(0);
  return elem;
}

template <typename Value>
auto BinaryHeap<Value>::PeekMin() const -> const value_type & {
  if (IsEmpty())
    throw std::out_of_range("Heap is empty");
  return data_.front();
}

template <typename Value> void BinaryHeap<Value>::SieveUp(size_t index) {
  size_t parent_index = (index - 1) / 2;
  while (index > 0 && data_[index].first < data_[parent_index].first) {
    SwapData(index, parent_index);
    index = std::exchange(parent_index, (parent_index - 1) / 2);
  }
}

template <typename Value> void BinaryHeap<Value>::SieveDown(size_t index) {
  while (2 * index + 1 < data_.size()) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t curr_index = left;
    if (right < data_.size() && data_[right].first < data_[left].first) {
      curr_index = right;
    }
    if (data_[index].first <= data_[curr_index].first)
      break;
    SwapData(index, curr_index);
    index = curr_index;
  }
}
