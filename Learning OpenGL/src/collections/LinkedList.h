#pragma once
#include "IList.h"
#include <list>

namespace Shado {
	template<typename T>
	class LinkedList : public IList<T> {

	public:
		LinkedList();
		LinkedList(const std::initializer_list<T>& other);
		LinkedList(const LinkedList& other);
		~LinkedList();
	
		void add(const T& element) override;
		void add(const T& element, uint32_t index) override;
		void addAll(const IList<T>& other) override;
	
		T remove(uint32_t index) override;
		void removeAll(std::function<bool(T&)> condition) override;
	
		void clear() override;	
		T& get(uint32_t index) override;
	
		int size() override;
		bool isEmpty() override;
		bool equals(const IList<T>& other) override;
	
		T& operator[](uint32_t index) override;
		IList<T>& operator<<(const T& element) override;
		bool operator==(const IList<T>& other) override;
		operator bool() override;

		std::iterator begin() override;
		std::iterator end() override;

	private:
		std::list<T> list;
	};

	template <typename T>
	LinkedList<T>::LinkedList() {}

	template <typename T>
	LinkedList<T>::LinkedList(const std::initializer_list<T>& other) {
		std::scoped_lock lock(this->mutex);
		list = std::list(other);
	}

	template <typename T>
	LinkedList<T>::LinkedList(const LinkedList& other) {
		std::scoped_lock lock(this->mutex);
		for (int i = 0; i < other.size(); i++) {
			add(other[i]);
		}
	
	}

	template <typename T>
	LinkedList<T>::~LinkedList() {
		std::scoped_lock lock(this->mutex);

		list.~std::list<T>;
	}

	template <typename T>
	void LinkedList<T>::add(const T& element) {
		std::scoped_lock lock(this->mutex);
		list.push_back(element);
	}

	template <typename T>
	void LinkedList<T>::add(const T& element, uint32_t index) {

		std::scoped_lock lock(this->mutex);

		if (index > size())
			throw std::exception("Index out of bound exception");

		auto& it = list.begin();
		list.insert(it, index - 1, element);
	
	}

	template <typename T>
	void LinkedList<T>::addAll(const IList<T>& other) {

		std::scoped_lock lock(this->mutex);

		for (int i = 0; i < other.size(); i++)
			add(other.get(i));
		
	}

	template <typename T>
	T LinkedList<T>::remove(uint32_t index) {

		std::scoped_lock lock(this->mutex);

		if (index >= size())
			throw std::exception("Out of bound exception");

		T temp = list[index];
		list.erase(list.begin() + (index - 1));

		return temp;
	}

	template <typename T>
	void LinkedList<T>::removeAll(std::function<bool(T&)> condition) {

		std::scoped_lock lock(this->mutex);

		for (int i = size() - 1; i >= 0; i--)
			if (condition(get(i)))
				remove(i);
	}

	template <typename T>
	void LinkedList<T>::clear() {

		std::scoped_lock lock(this->mutex);
		list.clear();
	}

	template <typename T>
	T& LinkedList<T>::get(uint32_t index) {

		std::scoped_lock lock(this->mutex);
		return list[index];
	}

	template <typename T>
	int LinkedList<T>::size() {

		std::scoped_lock lock(this->mutex);
		return list.size();
	}

	template <typename T>
	bool LinkedList<T>::isEmpty() {
		std::scoped_lock lock(this->mutex);
		return size() == 0;
	}

	template <typename T>
	bool LinkedList<T>::equals(const IList<T>& other) {
	
		std::scoped_lock lock(this->mutex);

		if (size() != other.size()) {
			return false;
		}
		
		for (int i = 0; i < size(); i++) {
			if (get(i) != other.get(i))
				return false;
		}
		
		return true;
	}

	template <typename T>
	T& LinkedList<T>::operator[](uint32_t index) {
		std::scoped_lock lock(this->mutex);
		return get(index);
	}

	template <typename T>
	IList<T>& LinkedList<T>::operator<<(const T& element) {
		std::scoped_lock lock(this->mutex);
		add(element);

		return *this;
	}

	template <typename T>
	bool LinkedList<T>::operator==(const IList<T>& other) {
		std::scoped_lock lock(this->mutex);
		return equals(other);
	}

	template <typename T>
	LinkedList<T>::operator bool() {
		std::scoped_lock lock(this->mutex);
		return !isEmpty();
	}

	
}
