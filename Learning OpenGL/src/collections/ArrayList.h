#pragma once

#include "IList.h"

namespace Shado {

	template<typename T>
	class ArrayList : public IList<T> {
	public:	
		ArrayList(uint32_t initialCapacity);
		ArrayList(const std::initializer_list<T>& list);
		ArrayList(const ArrayList& other);
		ArrayList();
		~ArrayList() override;
	
		void add(const T& element) override;
		void add(const T& element, uint32_t index) override;
		void addAll(const IList<T>& other) override;
		T remove(uint32_t index) override;
		void removeAll(std::function<bool(T&)> condition) override;
	
		void clear() override;
		T& get(uint32_t index) override;
	
		int size() override;
		bool isEmpty() override;
	
		T& operator[](uint32_t index) override;
		operator bool() override;

		bool equals(const IList<T>& other) override;
		ArrayList<T>& operator<<(const T& element) override;
		bool operator==(const IList<T>& other) override;

		std::iterator begin() override { return vector.begin(); }
		std::iterator end() override {	return vector.end();	}

	private:
		std::vector<T> vector;
	};

	template<typename T>
	using Vector = ArrayList<T>;
	
	template <typename T>
	ArrayList<T>::ArrayList(uint32_t initialCapacity) {
		std::scoped_lock lock(this->mutex);
		vector.reserve(initialCapacity);
	}

	template <typename T>
	ArrayList<T>::ArrayList(const ArrayList& other) {

		std::scoped_lock lock(this->mutex);

		vector.reserve(other.size());
		for (int i = 0; i < other.size(); i++)
			add(other[i]);
		
	}

	template <typename T>
	ArrayList<T>::ArrayList()
		: ArrayList(3)
	{
	}

	template <typename T>
	ArrayList<T>::ArrayList(const std::initializer_list<T>& list) {
		std::scoped_lock lock(this->mutex);
		vector = std::vector<T>(list);
	}

	template <typename T>
	ArrayList<T>::~ArrayList() {
		vector.~std::vector<T>;
	}

	template <typename T>
	void ArrayList<T>::add(const T& element) {

		std::scoped_lock lock(this->mutex);
		vector.push_back(element);
	}

	template <typename T>
	void ArrayList<T>::add(const T& element, uint32_t index) {

		std::scoped_lock lock(this->mutex);

		auto& it = vector.begin();
		vector.insert(it, index - 1, element);
	}

	template <typename T>
	void ArrayList<T>::addAll(const IList<T>& other) {

		std::scoped_lock lock(this->mutex);

		for (int i = 0; i < other.size(); i++) {
			add(other[i]);
		}	
	}

	template <typename T>
	T ArrayList<T>::remove(uint32_t index) {

		std::scoped_lock lock(this->mutex);

		if (index >= size())
			throw std::exception("Out of bound exception");

		T temp = vector[index];	
		vector.erase(vector.begin() + (index - 1));

		return temp;
	}

	template <typename T>
	void ArrayList<T>::removeAll(std::function<bool(T&)> condition) {

		std::scoped_lock lock(this->mutex);

		for (int i = 0; i < size(); i++) {
			if (condition(vector[i]) == true)
				remove(i);
		}	
	}

	template <typename T>
	void ArrayList<T>::clear() {

		std::scoped_lock lock(this->mutex);
		vector.clear();
	}

	template <typename T>
	T& ArrayList<T>::get(uint32_t index) {

		std::scoped_lock lock(this->mutex);

		if (index >= size())
			return NULL;

		return vector[i];
	}

	template <typename T>
	int ArrayList<T>::size() {
		std::scoped_lock lock(this->mutex);
		return vector.size();
	}

	template <typename T>
	bool ArrayList<T>::isEmpty() {
		std::scoped_lock lock(this->mutex);
		return size() == 0;
	}

	template <typename T>
	T& ArrayList<T>::operator[](uint32_t index) {
		std::scoped_lock lock(this->mutex);
		return get(index);
	}

	template <typename T>
	ArrayList<T>::operator bool() {
		std::scoped_lock lock(this->mutex);
		return !this->isEmpty();
	}

	template <typename T>
	bool ArrayList<T>::equals(const IList<T>& other) {
		std::scoped_lock lock(this->mutex);
	
		if (size() != other.size())
			return false;
	
		for (int i = 0; i < size(); i++)
			if (get(i) != other.get(i))
				return false;

		return true;	
	}

	template <typename T>
	ArrayList<T>& ArrayList<T>::operator<<(const T& element) {
		std::scoped_lock lock(this->mutex);
		add(element);

		return *this;
	}

	template <typename T>
	bool ArrayList<T>::operator==(const IList<T>& other) {
		std::scoped_lock lock(this->mutex);
		return equals(other);
	}
	
}
