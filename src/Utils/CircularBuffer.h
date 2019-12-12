#pragma once

#include <stdexcept>


template<class T>
class CircularBuffer {
private:
	T* buffer;
	size_t buffer_size;
	int head = 0;
	int tail = 0;
	int size = 0;
	bool full;
public:
	CircularBuffer<T>(size_t size) :
		buffer_size(size),
		buffer(new T[size]) {} // if we ever use this class, change this
	
	size_t get_buffer_size() {
		return buffer_size;
	}

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

	template<class T> void get_buffer(T buffer_out[]) {
		// Returns the whole buffer, from tail to head

		for (int i = 0; i < size; i++) {
			buffer_out[i] = buffer[(i + tail) % buffer_size];
		}
	}

	template<class T> bool find(T value) {
		// Returns the whole buffer, from tail to head

		for (size_t i = 0; i < size; i++) {
			if (buffer[(i + tail) % buffer_size] == value) {
				return true;
			}
		}
		return false;
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
