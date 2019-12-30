//
//
//
#ifndef WET_ARRAY_H
#define WET_ARRAY_H
#include <iostream>
template <class T>
class Array {
	T* data;
	int size_a;
public:
	explicit Array(int size_a);
	Array(const Array& a);
	Array& operator=(const Array& a);
	~Array();
	int size() const;
	T& operator[](int index);
	const T& operator[](int index) const;
	class BadIndex {
	public:
		int index;
		explicit BadIndex(int index) : index(index) {}
	};
};


template<class T>
Array<T>::Array(int size_a) : data(new T[size_a]), size_a(size_a) {
}

template<class T>
Array<T>::Array(const Array &a) : data(new T[a.size_a]), size_a(a.size_a) {
	for (int i = 0; i < size_a; ++i) {
		data[i] = a.data[i];
	}
}

template<class T>
Array<T>& Array<T>::operator=(const Array& a) {
	if (this == &a) {
		return *this;
	}
	delete[] data;
	data = new T[a.size_a];
	size_a = a.size_a;
	for (int i = 0; i < a.size_a; ++i) {
		data[i] = a.data[i];
	}
	return *this;
}

template<class T>
Array<T>::~Array() {
	delete[] data;
}

template<class T>
int Array<T>::size() const {
	return size_a;
}

template<class T>
T &Array<T>::operator[](int index) {
	if (index < 0 || index >= size_a)
		throw BadIndex(index);
	return data[index];
}

template<class T>
const T &Array<T>::operator[](int index) const {
	if (index < 0 || index >= size_a)
		throw BadIndex(index);
	return &data[index];
}


#endif //WET_ARRAY_H