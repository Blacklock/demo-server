#pragma once

#include <stdexcept>


template<class T>
class CircularBuffer {
private:
	T* buffer;
	size_t buffer_size;
	int head = 0;
	int tail = 0;
	bool full;
public:
	CircularBuffer<T>(size_t size) :
		buffer_size(size),
		buffer(new T[size]) {}
	
	size_t get_buffer_size() {
		return buffer_size;
	}

	template<class T> void put(T value) {
		// Appends value to end of buffer

		buffer[head] = value;

		if (full) {
			tail = (tail + 1) % buffer_size;
		}

		head = (head + 1) % buffer_size;
		full = head == tail;
	};

	/*template<class T> void get_buffer(T buffer_out[]) {
		// Returns the whole buffer, from tail to head

		for (int i = 0; i < buffer_size; i++) {
			buffer_out[i] = buffer[(i + tail) % buffer_size];
		}
	}*/

	/*template<class T> T get_item(int index) {
		// Returns item at an index (starting from tail)
		if (index < buffer_size) {
			return buffer[(index + tail) % buffer_size];
		} else {
			throw std::out_of_range("Item index larger than buffer size.");
		}
	}*/
};
