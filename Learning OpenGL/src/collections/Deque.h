#pragma once
#include "IQueue.h"
#include <deque>

namespace Shado {

	template<typename T>
	class Deque : public IQueue<T> {
	public:
		Deque(uint32_t size);
		Deque();

		bool addBack(const T& element) override;
		bool addFront(const T& element);
		T& element() override;
		T pop() override;
		T popBack();
		T remove() override;		
		
		int size() override;
		bool isEmpty() override;
		void clear() override;
		
		IQueue<T>& operator<<(const T& element) override;
		bool operator==(const IQueue<T>& other) override;
		operator bool() override;

		virtual std::iterator begin() override { return deque.begin(); }
		virtual std::iterator end() override { return deque.end(); }

	private:
		std::deque<T> deque;
	};

	template <typename T>
	Deque<T>::Deque(uint32_t size) {
		std::scoped_lock lock(this->mutex);
		deque.resize(size);
	}

	template <typename T>
	Deque<T>::Deque() {}

	template <typename T>
	bool Deque<T>::addBack(const T& element) {
		std::scoped_lock lock(this->mutex);
		
		if (deque.max_size() >= deque.size())
			return false;
		
		deque.push_back(element);
		return true;
	}

	template <typename T>
	bool Deque<T>::addFront(const T& element) {
		std::scoped_lock lock(this->mutex);
		
		if (deque.max_size() >= deque.size())
			return false;

		deque.push_front(element);
		return true;
	}

	template <typename T>
	T& Deque<T>::element() {
		std::scoped_lock lock(this->mutex);
		return deque[0];
	}

	template <typename T>
	T Deque<T>::pop() {
		std::scoped_lock lock(this->mutex);
		return deque.pop_front();
	}

	template <typename T>
	T Deque<T>::popBack() {
		std::scoped_lock lock(this->mutex);
		return deque.pop_back();
	}

	template <typename T>
	T Deque<T>::remove() {
		std::scoped_lock lock(this->mutex);
		return deque.pop_front();
	}

	template <typename T>
	int Deque<T>::size() {
		std::scoped_lock lock(this->mutex);
		return deque.size();
	}

	template <typename T>
	bool Deque<T>::isEmpty() {
		std::scoped_lock lock(this->mutex);
		return deque.size() == 0;
	}

	template <typename T>
	void Deque<T>::clear() {
		std::scoped_lock lock(this->mutex);
		deque.clear();
	}

	template <typename T>
	IQueue<T>& Deque<T>::operator<<(const T& element) {
		std::scoped_lock lock(this->mutex);
		this->addBack(element);

		return *this;
	}

	template <typename T>
	bool Deque<T>::operator==(const IQueue<T>& other) {
		std::scoped_lock lock(this->mutex);

		if (this->size() != other.size())
			return false;
		
		for (int i = 0; i < other.size(); i++) {
			if (deque[i] != other[i])
				return false;
		}

		return true;	
	}

	template <typename T>
	Deque<T>::operator bool() {
		std::scoped_lock lock(this->mutex);
		return !isEmpty();
	}
}
