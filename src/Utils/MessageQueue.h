#pragma once

#include <memory>
#include <deque>
#include <stdexcept>


template<class T>
class MessageQueue { // stackoverflow.com/questions/5465332/c-queue-with-efficient-get-put-of-multiple-elements
private:
	std::deque<T> queue;
public:
	MessageQueue<T>() {}

	template<class T> void push(T* buffer, size_t bytes) {
		queue.insert(queue.end(), buffer, buffer + bytes);
	};

	template<class T> void pop(T* buffer, size_t bytes) {
		if (bytes > size()) throw std::out_of_range("Queue size smaller than given byte amount.");
		std::copy_n(queue.begin(), bytes, buffer);
		queue.erase(queue.begin(), queue.begin() + bytes);
	};

	template<class T> void copy(T* buffer, size_t bytes) {
		if (bytes > size()) throw std::out_of_range("Queue size smaller than given byte amount.");
		std::copy_n(queue.begin(), bytes, buffer);
	};

	void erase(size_t bytes) {
		if (bytes > size()) throw std::out_of_range("Queue size smaller than given byte amount.");
		queue.erase(queue.begin(), queue.begin() + bytes);
	};

	size_t size() {
		return queue.size();
	}
};
