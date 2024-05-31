#pragma once

#include <thread>
#include <shared_mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace SimpleTextingService {
namespace impl {

template <typename T>
class MessageQueue {
 public:
  MessageQueue() = default;
  virtual ~MessageQueue() = default;

  /// @brief Push a message to the back of the queue. Notifies all waiting
  /// threads and locks the queue while pushing.
  /// @param msg shared pointer to the message
  void PushBack(std::shared_ptr<const T> msg) {
    std::unique_lock<std::shared_mutex> lock(mutex_);

    queue_.push(message);
    cv_.notify_all();

    count++;
  }

  /// @brief Pop a message from the front of the queue. Blocks the message queue
  /// while poping.
  /// @return shared pointer to the message. Will be nullptr if the queue is
  /// empty.
  std::shared_ptr<const T> PopFront() {
    std::unique_lock<std::shared_mutex> lock(mutex_);

    if (count == 0) {
      return nullptr;
    }

    auto msg = queue_.front();
    queue_.pop();

    count--;

    return msg;
  }

  /// @brief Access the front of the queue without popping it. Aquires a
  /// shared_lock
  /// @return Pointer to the first message in the queue. Will be nullptr if the
  /// queue is empty.
  std::shared_ptr<const T> Front() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);

    if (count == 0) {
      return nullptr;
    }

    return queue_.front();
  }

  /// @brief Access the back of the queue without popping it. Aquires a
  /// shared_lock
  /// @return Pointer to the last message in the queue. Will be nullptr if the
  /// queue is empty.
  std::shared_ptr<const T> Back() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);

    if (count == 0) {
      return nullptr;
    }

    return queue_.back();
  }

  bool Empty() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return queue_.empty();
  }

  size_t Size() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return count;
  }

  void Clear() {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    while (!queue_.empty()) {
      queue_.pop();
    }
    count = 0;
  }

  /// @brief Wait for the queue to be non-empty. Blocks the calling thread.
  void Wait() {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    cv_.wait(lock, [this] { return count > 0; });
  }

 private:
  std::queue<std::shared_ptr<const T>> queue_;
  mutable std::shared_mutex mutex_;
  std::condition_variable_any cv_;

  size_t size_;
};

}  // namespace impl
}  // namespace SimpleTextingService
