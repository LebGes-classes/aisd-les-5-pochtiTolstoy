#pragma once

#include <algorithm>
#include <stdexcept>

template <typename Value> class BinaryHeap {
public:
  using value_type = typename std::pair<size_t, Value>;
  using key_type = size_t;
  using mapped_type = Value;

  BinaryHeap() = default;
  ~BinaryHeap() = default;

  bool is_empty() const noexcept;
  size_t size() const noexcept;

protected:
  void Insert(const value_type &new_pair);
  void DecreaseKey(const value_type &new_pair);
  value_type ExtractMin();
  const value_type &PeekMin() const;

private:
  std::vector<value_type> data_;
  void SieveUp(int index);
  void SieveDown(int index);
};

template <typename Value> bool BinaryHeap<Value>::is_empty() const noexcept {
  return data_.size() == 0;
}

template <typename Value> size_t BinaryHeap<Value>::size() const noexcept {
  return data_.size();
}

template <typename Value>
void BinaryHeap<Value>::Insert(const value_type &new_pair) {
  data_.push_back(new_pair);
  SieveUp(data_.size() - 1);
}

template <typename Value>
void BinaryHeap<Value>::DecreaseKey(const value_type &new_pair) {
  auto it = std::find_if(data_.begin(), data_.end(), [&](const auto &p) {
    return p.second == new_pair.second;
  });

  if (it == data_.end() || it->first <= new_pair.first)
    return;

  it->first = new_pair.first;
  SieveUp(static_cast<size_t>(std::distance(data_.begin(), it)));
}

template <typename Value> auto BinaryHeap<Value>::ExtractMin() -> value_type {
  if (is_empty()) {
    throw std::out_of_range("Cannot extract from an empty heap");
  }
  auto elem = std::exchange(data_.front(), data_.back());
  data_.pop_back();
  if (not is_empty()) {
    SieveDown(0);
  }
  return elem;
}

template <typename Value>
auto BinaryHeap<Value>::PeekMin() const -> const value_type & {
  return data_.front();
}

template <typename Value> void BinaryHeap<Value>::SieveUp(int index) {
  int parent_index = (index - 1) / 2;
  while (index > 0 && data_[index].first < data_[parent_index].first) {
    std::swap(data_[index], data_[parent_index]);
    index = std::exchange(parent_index, (index - 1) / 2);
  }
}

template <typename Value> void BinaryHeap<Value>::SieveDown(int index) {
  while (2 * index + 1 < data_.size()) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int curr_index = left;
    if (right < data_.size() && data_[right].first < data_[left].first) {
      curr_index = right;
    }
    if (data_[index].first <= data_[curr_index].first) {
      break;
    } else {
      std::swap(data_[index], data_[curr_index]);
      index = curr_index;
    }
  }
}
