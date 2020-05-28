#pragma once

#include <stdexcept>
#include <chrono>


template<class T>
class CircularBuffer {
public:
	T* buffer; // TODO: Back to private!
private:
	size_t buffer_size;
	int head = 0;
	int tail = 0;
	size_t size = 0;
	bool full = false;
public:
	CircularBuffer<T>(size_t size) :
		buffer_size(size),
		buffer(new T[size]) {} // if we ever use this class, change this

	template<class T> void put(T value) {
		// Appends value to end of buffer

		buffer[head] = value;

		if (full) {
			tail = (tail + 1) % buffer_size;
		}

		if (!full) size++;
		head = (head + 1) % buffer_size;
		full = head == tail;
	};

	template<class T> bool find(T value) {
		for (size_t i = 0; i < size; i++) {
			if (buffer[(i + tail) % buffer_size] == value) {
				return true;
			}
		}
		return false;
	}

	template<class T> bool replace(T value, T new_value) {
		for (size_t i = 0; i < size; i++) {
			if (buffer[(i + tail) % buffer_size] == value) {
				buffer[(i + tail) % buffer_size] = new_value;
				return true;
			}
		}
		return false;
	}

	template<class T> int count(T value) {
		int c = 0;
		for (size_t i = 0; i < size; i++) {
			if (buffer[(i + tail) % buffer_size] == value) {
				c++;
			}
		}
		return c;
	}

	T getLast() {
		return buffer[tail];
	}

	size_t getSize() {
		return size;
	}

	size_t getBufferSize() {
		return buffer_size;
	}

	bool isFull() {
		return full;
	}

	void reset() {
		head = 0;
		tail = 0;
		size = 0;
		full = false;
	}

	/*template<class T> T get_item(int index) {
		// Returns item at an index (starting from tail)
		if (index < buffer_size) {
			return buffer[(index + tail) % buffer_size];
		} else {
			throw std::out_of_range("Item index larger than buffer size.");
		}
	}*/
};
