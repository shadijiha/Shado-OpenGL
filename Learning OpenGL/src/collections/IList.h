#pragma once

#include "IThreadsafe.h"
#include <functional>
#include <exception>
#include <iterator>

namespace Shado {
	template<typename T>
	class IList	: public IThreadSafe {
	public:
		virtual ~IList() {};	
	
		virtual void add(const T& element) = 0;
		virtual void add(const T& element, uint32_t index) = 0;
		virtual void addAll(const IList<T>& other) = 0;
		virtual T remove(uint32_t index) = 0;
		virtual void removeAll(std::function<bool(T&)> condition) = 0;
		virtual void clear() = 0;
	
		virtual T& get(uint32_t index) = 0;

		virtual int size() = 0;
		virtual bool isEmpty() = 0;
		virtual bool equals(const IList<T>& other) = 0;

		virtual T& operator[] (uint32_t index) = 0;
		virtual IList& operator<<(const T& element) = 0;
		virtual bool operator==(const IList<T>& other) = 0;

		virtual operator bool() = 0;

		virtual std::iterator begin() = 0;
		virtual std::iterator end() = 0;
	
	};
}
