#include "../src/PriorityQueue.hpp"
#include <functional>
#include <gtest/gtest.h>
#include <string>
#include <vector>

class PriorityQueueTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
};

struct Point {
  int x, y;
  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }
};

namespace std {
template <> struct hash<Point> {
  size_t operator()(const Point &p) const {
    return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
  }
};
} // namespace std

TEST_F(PriorityQueueTest, BasicEnqueueDequeue) {
  PriorityQueue<std::string> queue;

  queue.Enqueue("first", 3);
  queue.Enqueue("second", 1);
  queue.Enqueue("third", 2);

  auto [priority1, value1] = queue.Peek();
  EXPECT_EQ(priority1, 1);
  EXPECT_EQ(value1, "second");

  queue.Dequeue();
  auto [priority2, value2] = queue.Peek();
  EXPECT_EQ(priority2, 2);
  EXPECT_EQ(value2, "third");

  queue.Dequeue();
  auto [priority3, value3] = queue.Peek();
  EXPECT_EQ(priority3, 3);
  EXPECT_EQ(value3, "first");
}

TEST_F(PriorityQueueTest, EmptyQueue) {
  PriorityQueue<int> queue;
  EXPECT_TRUE(queue.IsEmpty());
  EXPECT_EQ(queue.Size(), 0);
}

TEST_F(PriorityQueueTest, DecreasePriority) {
  PriorityQueue<std::string> queue;

  queue.Enqueue("high", 10);
  queue.Enqueue("medium", 5);
  queue.Enqueue("low", 1);

  queue.DecreasePriority("high", 0);
  auto [priority, value] = queue.Peek();
  EXPECT_EQ(priority, 0);
  EXPECT_EQ(value, "high");
}

TEST_F(PriorityQueueTest, SamePriority) {
  PriorityQueue<std::string> queue;

  queue.Enqueue("first", 1);
  queue.Enqueue("second", 1);
  queue.Enqueue("third", 1);

  std::set<std::string> values;
  while (!queue.IsEmpty()) {
    auto [p, v] = queue.Peek();
    EXPECT_EQ(p, 1);
    values.insert(v);
    queue.Dequeue();
  }
  EXPECT_EQ(values, (std::set<std::string>{"first", "second", "third"}));
}

TEST_F(PriorityQueueTest, SizeOperations) {
  PriorityQueue<int> queue;

  EXPECT_EQ(queue.Size(), 0);

  queue.Enqueue(1, 1);
  EXPECT_EQ(queue.Size(), 1);

  queue.Enqueue(2, 2);
  EXPECT_EQ(queue.Size(), 2);

  queue.Dequeue();
  EXPECT_EQ(queue.Size(), 1);

  queue.Dequeue();
  EXPECT_EQ(queue.Size(), 0);
}

TEST_F(PriorityQueueTest, ComplexPriorityChanges) {
  PriorityQueue<std::string> queue;

  queue.Enqueue("A", 5);
  queue.Enqueue("B", 3);
  queue.Enqueue("C", 1);

  queue.DecreasePriority("A", 0);
  auto [priority1, value1] = queue.Peek();
  EXPECT_EQ(priority1, 0);
  EXPECT_EQ(value1, "A");

  queue.Dequeue();

  queue.DecreasePriority("B", 0);
  auto [priority2, value2] = queue.Peek();
  EXPECT_EQ(priority2, 0);
  EXPECT_EQ(value2, "B");
}

TEST_F(PriorityQueueTest, DifferentValueTypes) {
  PriorityQueue<int> intQueue;
  intQueue.Enqueue(42, 1);
  auto [priority1, value1] = intQueue.Peek();
  EXPECT_EQ(value1, 42);

  PriorityQueue<double> doubleQueue;
  doubleQueue.Enqueue(3.14, 1);
  auto [priority2, value2] = doubleQueue.Peek();
  EXPECT_DOUBLE_EQ(value2, 3.14);

  PriorityQueue<Point> pointQueue;
  pointQueue.Enqueue({1, 2}, 1);
  auto [priority3, value3] = pointQueue.Peek();
  EXPECT_EQ(value3.x, 1);
  EXPECT_EQ(value3.y, 2);
}

TEST_F(PriorityQueueTest, DuplicateInsertion) {
  PriorityQueue<int> queue;
  queue.Enqueue(42, 1);
  EXPECT_THROW(queue.Enqueue(42, 2), std::invalid_argument);
  EXPECT_EQ(queue.Size(), 1);
}

TEST_F(PriorityQueueTest, DecreasePriorityNonExistent) {
  PriorityQueue<std::string> queue;
  queue.Enqueue("exists", 5);
  EXPECT_THROW(queue.DecreasePriority("missing", 0), std::invalid_argument);
}

TEST_F(PriorityQueueTest, ExtractMinEmpty) {
  PriorityQueue<int> queue;
  EXPECT_THROW(queue.Dequeue(), std::out_of_range);
}

TEST_F(PriorityQueueTest, PeekEmpty) {
  PriorityQueue<int> queue;
  EXPECT_THROW(queue.Peek(), std::out_of_range);
}

TEST_F(PriorityQueueTest, MoveSemantics) {
  PriorityQueue<std::string> queue;
  std::string movable = "movable";

  queue.Enqueue(std::move(movable), 1);
  EXPECT_TRUE(movable.empty());

  auto [_, value] = queue.Peek();
  EXPECT_EQ(value, "movable");
}

TEST_F(PriorityQueueTest, LargeScale) {
  PriorityQueue<int> queue;
  constexpr size_t N = 1000;

  for (size_t i = N; i > 0; i--) {
    queue.Enqueue(i, i);
  }

  EXPECT_EQ(queue.Size(), N);

  for (size_t i = 1; i <= N; i++) {
    auto [priority, value] = queue.Peek();
    EXPECT_EQ(priority, i);
    EXPECT_EQ(value, i);
    queue.Dequeue();
  }

  EXPECT_TRUE(queue.IsEmpty());
}

TEST_F(PriorityQueueTest, IncreasePriorityIgnored) {
  PriorityQueue<std::string> queue;
  queue.Enqueue("test", 5);

  queue.DecreasePriority("test", 10);

  auto [priority, _] = queue.Peek();
  EXPECT_EQ(priority, 5);
}

TEST_F(PriorityQueueTest, HeapInvariant) {
  PriorityQueue<int> queue;

  queue.Enqueue(1, 5);
  queue.Enqueue(2, 3);
  queue.Enqueue(3, 4);
  queue.Enqueue(4, 1);
  queue.Enqueue(5, 2);

  std::vector<size_t> expected_priorities = {1, 2, 3, 4, 5};
  for (size_t prio : expected_priorities) {
    auto [priority, _] = queue.Peek();
    EXPECT_EQ(priority, prio);
    queue.Dequeue();
  }
}

TEST_F(PriorityQueueTest, UpdateCausesReordering) {
  PriorityQueue<std::string> queue;

  queue.Enqueue("A", 5);
  queue.Enqueue("B", 3);
  queue.Enqueue("C", 1);

  queue.DecreasePriority("A", 0);
  EXPECT_EQ(queue.Peek().second, "A");

  queue.DecreasePriority("B", 0);
  queue.Dequeue();
  EXPECT_EQ(queue.Peek().second, "B");
}

TEST_F(PriorityQueueTest, MoveOperations) {
  PriorityQueue<int> queue1;
  queue1.Enqueue(1, 3);
  queue1.Enqueue(2, 1);

  PriorityQueue<int> queue2 = std::move(queue1);
  EXPECT_EQ(queue2.Size(), 2);
  EXPECT_TRUE(queue1.IsEmpty());

  PriorityQueue<int> queue3;
  queue3 = std::move(queue2);
  EXPECT_EQ(queue3.Size(), 2);
  EXPECT_TRUE(queue2.IsEmpty());

  auto [p1, v1] = queue3.Peek();
  EXPECT_EQ(p1, 1);
  EXPECT_EQ(v1, 2);
}

TEST_F(PriorityQueueTest, MoveSemanticsValue) {
  PriorityQueue<std::string> queue;

  queue.Enqueue(std::string("temporary"), 3);

  EXPECT_EQ(queue.Size(), 1);
  auto [priority, value] = queue.Peek();
  EXPECT_EQ(value, "temporary");

  std::string movable = "movable";
  queue.Enqueue(std::move(movable), 2);
  EXPECT_TRUE(movable.empty());

  queue.DecreasePriority("temporary", 1);
  auto [p2, v2] = queue.Peek();
  EXPECT_EQ(v2, "temporary");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
