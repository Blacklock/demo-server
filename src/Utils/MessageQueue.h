#pragma once

#include <memory>
#include <deque>


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
		std::copy_n(queue.begin(), bytes, buffer);
		queue.erase(queue.begin(), queue.begin() + bytes);
	};

	size_t size() {
		return queue.size();
	}
};
