#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template <typename Value> class BinaryHeap {
public:
  using value_type = typename std::pair<size_t, Value>;

  BinaryHeap() = default;
  ~BinaryHeap() = default;

  bool is_empty() { return data_.size() == 0; }
  size_t size() { return data_.size(); }

protected:
  void Insert(const value_type &new_pair) {
    data_.push_back(new_pair);
    SieveUp(data_.size() - 1);
  }

  void DecreaseKey(const value_type &new_pair) {
    auto it = std::find_if(data_.begin(), data_.end(),
                           [&new_pair](const auto &vec_pair) {
                             return vec_pair.second == new_pair.second;
                           });

    if (it == data_.end())
      return;

    if (it->first < new_pair->first)
      return;

    it->first = new_pair->first;
    size_t sieve_idx = std::distance(data_.begin(), it);
    SieveUp(sieve_idx);
  }

  value_type ExtractMin() {
    auto elem = std::exchange(data_.front(), data_.back());
    data_.pop_back();
    SieveDown(0);
    return elem;
  }

  const value_type &PeekMin() const { return data_.front(); }

private:
  std::vector<value_type> data_;

  void SieveUp(int index) {
    int parent_index = (index - 1) / 2;
    while (index > 0 && data_[index].first < data_[parent_index].first) {
      std::swap(data_[index], data_[parent_index]);
      index = parent_index;
    }
  }

  void SieveDown(int index) {
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
  using value_type = std::pair<size_t, Value>;

  PriorityQueue() = default;
  ~PriorityQueue() = default;

  void Enqueue(const Value &value, size_t key = 0) {
    BinaryHeap<Value>::Insert({key, value});
  }

  void Dequeue() { BinaryHeap<Value>::ExtractMin(); }

  value_type Peek() const { return BinaryHeap<Value>::PeekMin(); }

  void DecreasePriority(const Value &value, size_t new_priority = 0) {
    BinaryHeap<Value>::DecreaseKey(new_priority, value);
  }
};

int main() {
  PriorityQueue<std::string> queue;
  queue.Enqueue("~~~", 1);
  queue.Enqueue("!", 3);
  queue.Enqueue("world", 2);
  queue.Enqueue("hello", 1);
  while (not queue.is_empty()) {
    const auto &[priority, data] = queue.Peek();
    std::cout << "Priority : " << priority << ", Data : " << data << std::endl;
    queue.Dequeue();
  }
}
