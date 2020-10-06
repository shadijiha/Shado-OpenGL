#pragma once
#include "IThreadsafe.h"

namespace Shado {
	
	template <typename T>
	class IQueue : public IThreadSafe {
		virtual ~IQueue() = default;

		/**
		 * Inserts the specified element into this queue if it is possible to do so immediately without violating capacity restrictions,
		 * returning true upon success and false if no space is currently available.
		 */
		virtual bool addBack(const T& element) = 0;

		/**
		 * Retrieves, but does not remove, the head of this queue.
		 */
		virtual T& element() = 0;

		/**
		 * Retrieves and removes the head of this queue, or returns null if this queue is empty.
		 */
		virtual T pop() = 0;

		/**
		 * Retrieves and removes the head of this queue
		 */
		virtual T remove() = 0;

		virtual int size() = 0;
		virtual bool isEmpty() = 0;
		virtual void clear() = 0;

		virtual IQueue& operator<<(const T& element) = 0;
		virtual bool operator==(const IQueue<T>& other) = 0;

		virtual operator bool() = 0;

		virtual std::iterator begin() = 0;
		virtual std::iterator end() = 0;
	};
	
}
