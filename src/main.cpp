#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template <typename Value> class BinaryHeap {
public:
  BinaryHeap() = default;
  ~BinaryHeap() = default;
  bool is_empty() { return data_.size() == 0; }
  size_t size() { return data_.size(); }

protected:
  void insert(const std::pair<size_t, Value> &new_pair) {
    data_.push_back(new_pair);
    sieve_up(data_.size() - 1);
  }

  void decrease_key(size_t new_key, const Value &value) {
    auto it = std::find_if(
        data_.begin(), data_.end(),
        [&value](const auto &vec_pair) { return vec_pair.second == value; });
    if (it == data_.end())
      return;
    if (it->first < new_key)
      return;
    it->first = new_key;
    size_t sieve_idx = std::distance(data_.begin(), it);
    sieve_up(sieve_idx);
  }

  std::pair<size_t, Value> extract_min() {
    auto elem = std::exchange(data_.front(), data_.back());
    data_.pop_back();
    sieve_down(0);
    return elem;
  }

  const std::pair<size_t, Value> &peek_min() const { return data_.front(); }

private:
  std::vector<std::pair<size_t, Value>> data_;

  void sieve_up(int index) {
    int parent_index = (index - 1) / 2;
    while (index > 0 && data_[index].first < data_[parent_index].first) {
      std::swap(data_[index], data_[parent_index]);
      index = parent_index;
    }
  }

  void sieve_down(int index) {
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
};

template <typename Value> class PriorityQueue : public BinaryHeap<Value> {
public:
  PriorityQueue() = default;
  ~PriorityQueue() = default;

  void Enqueue(const Value &value, size_t key = 0) {
    BinaryHeap<Value>::insert({key, value});
  }

  void Dequeue() { BinaryHeap<Value>::extract_min(); }

  std::pair<size_t, Value> Peek() const {
    return BinaryHeap<Value>::peek_min();
  }

  void DecreasePriority(size_t new_priority, const Value &value) {
    BinaryHeap<Value>::DecreaseKey(new_priority, value);
  }
};

int main() {
  PriorityQueue<std::string> queue;
  queue.Enqueue("!", 3);
  queue.Enqueue("world", 2);
  queue.Enqueue("hello", 1);
  while (not queue.is_empty()) {
    const auto &[priority, data] = queue.Peek();
    std::cout << "Priority : " << priority << ", Data : " << data << std::endl;
    queue.Dequeue();
  }
}
