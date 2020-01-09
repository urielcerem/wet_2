#ifndef WET_1_HASH_TABLE_H
#define WET_1_HASH_TABLE_H
#include "Array.h"
#include <math.h>

typedef enum {
	SUCCESS_HT = 0,
	FAILURE_HT = -1,
	ALLOCATION_ERROR_HT = -2
} StatusTypeHashTable;

typedef enum {
	INCREASE = 1,
	DECREASE = 2,
	ORIGINAL_SIZE = 100
}TableSizeFunc;



template <class T>
class HTNode {
public:
	T* data;
	int key;
	HTNode <T> * next;
	HTNode <T> * prev;
	explicit HTNode(T* data,int key, HTNode <T> * next = NULL, 
		HTNode <T> * prev = NULL) : data(data), key(key), next(next), prev(prev) {}
	~HTNode() = default;

};



template <class T>
class HASH_TABLE {
	int size_of_table;
	int num_of_items;
	int ratio;
	HTNode<T> ** table;
public:
	explicit HASH_TABLE() : size_of_table(ORIGINAL_SIZE), num_of_items(0), ratio(10) {
		table = new HTNode<T>*[size_of_table];
		for (int i = 0; i < size_of_table; i++) {
			table[i] = NULL;
		}
	}
	~HASH_TABLE();
	StatusTypeHashTable Insert(int key,T* item);
	T* Find(int key);
	StatusTypeHashTable Remove(int key);
	void PrintTable();//for debugging purpuses
private:
	int Shuffling_func(int key);
	StatusTypeHashTable ChangeTableSize(TableSizeFunc func);
};

template <class T>
HASH_TABLE<T>::~HASH_TABLE() {
	HTNode<T> *curr_node, *temp_node;
	for (int j = 0; j < size_of_table; j++) {
		curr_node = table[j];
		while (curr_node) {
			temp_node = (*curr_node).next;
			delete curr_node;
			curr_node = temp_node;
		}
	}
	delete[] table;
}


template <class T>
int HASH_TABLE<T>::Shuffling_func(int key) {
	double a = (sqrt(5) - 1) / 2;
	double modified_key = a * key;
	double frac_of_m_key = modified_key - ((long)modified_key);
	double out_key = frac_of_m_key * size_of_table;
	return out_key;
}


template <class T>
StatusTypeHashTable HASH_TABLE<T>::Insert(int key, T* item) {
	int modified_key = Shuffling_func(key);
	if (table[modified_key] == nullptr) {
		num_of_items++;
		table[modified_key] = new HTNode<T>(item, key);
		if (!table[modified_key])
			return ALLOCATION_ERROR_HT;
	}
	else {
		HTNode<T> *curr_node = table[modified_key];
		if ((*curr_node).data == item) {
			return FAILURE_HT;
		}
		while ((*curr_node).next) {
			curr_node = (*curr_node).next;
			if ((*curr_node).data == item) {
				return FAILURE_HT;
			}
		}
		num_of_items++;
		(*curr_node).next = new HTNode<T>(item, key);
		(*curr_node).next->prev = curr_node;
	}
	if (num_of_items >= ratio * size_of_table * 2) {
		StatusTypeHashTable status_out = ChangeTableSize(INCREASE);
		if (status_out != SUCCESS_HT)
			return status_out;
	}
	return SUCCESS_HT;
}


template <class T>
StatusTypeHashTable HASH_TABLE<T>::ChangeTableSize(TableSizeFunc func) {
	int old_table_size = size_of_table;
	if (func == INCREASE)
		size_of_table = size_of_table * 2;
	else
		size_of_table = size_of_table / 2;
	HTNode<T> ** old_table = table;
	table = new HTNode<T>*[size_of_table];
	if (!table)
		return ALLOCATION_ERROR_HT;
	num_of_items = 0;
	for (int i = 0; i < size_of_table; i++) {
		table[i] = NULL;
	}
	HTNode<T> *curr_node, *temp_node;
	T* item;
	int key;
	StatusTypeHashTable status_out;
	for (int j = 0; j < old_table_size; j++) {
		curr_node = old_table[j];
		while (curr_node) {
			temp_node = (*curr_node).next;
			item = (*curr_node).data;
			key = (*curr_node).key;
			delete curr_node;
			curr_node = temp_node;
			status_out = Insert(key, item);
			if (status_out != SUCCESS_HT)
				return status_out;
		}
	}
	delete[] old_table;
	return SUCCESS_HT;
}

//returns NULL if any error accured//
template <class T>
T* HASH_TABLE<T>::Find(int key) {
	int modified_key = Shuffling_func(key);
	std::cout << key << "  " << modified_key << std::endl;
	if (table[modified_key] == nullptr) {
		return NULL;
	}
	else {
		HTNode<T> *curr_node = table[modified_key];
		if ((*curr_node).key == key) {
			return (*curr_node).data;
		}
		while ((*curr_node).next) {
			curr_node = (*curr_node).next;
			if ((*curr_node).key == key) {
				return (*curr_node).data;
			}
		}
		return NULL;
	}
}


template <class T>
StatusTypeHashTable HASH_TABLE<T>::Remove(int key) {
	int modified_key = Shuffling_func(key);
	if (table[modified_key] == nullptr) {
		return FAILURE_HT;
	}
	else {
		HTNode<T> *curr_node = table[modified_key];
		if ((*curr_node).key == key) {
			if ((*curr_node).next)
				(*curr_node).next->prev = NULL;
			table[modified_key] = (*curr_node).next;
			T* out_data = (*curr_node).data;
			delete curr_node;
			num_of_items--;
			if (num_of_items <= ratio * size_of_table / 2 && size_of_table > ORIGINAL_SIZE) {
				StatusTypeHashTable status_out = ChangeTableSize(DECREASE);
				if (status_out != SUCCESS_HT)
					return status_out;
			}
			return SUCCESS_HT;
		}
		else {
			while ((*curr_node).next) {
				curr_node = (*curr_node).next;
				if ((*curr_node).key == key) {
					(*curr_node).prev->next = (*curr_node).next;
					if ((*curr_node).next)
						(*curr_node).next->prev = (*curr_node).prev;
					T* out_data = (*curr_node).data;
					delete curr_node;
					num_of_items--;
					if (num_of_items <= ratio * size_of_table / 2 && size_of_table > ORIGINAL_SIZE) {
						StatusTypeHashTable status_out = ChangeTableSize(DECREASE);
						if (status_out != SUCCESS_HT)
							return status_out;
					}
					return SUCCESS_HT;
				}
			}
		}
	}
	return FAILURE_HT;
}



template <class T>
void HASH_TABLE<T>::PrintTable() {
	HTNode<T> *curr_node, *temp_node;
	T* item;
	int key;
	for (int j = 0; j < size_of_table; j++) {
		curr_node = table[j];
		while (curr_node) {
			temp_node = (*curr_node).next;
			item = (*curr_node).data;
			std::cout << *item << "    ";
			curr_node = temp_node;
		}
		std::cout << std::endl;
	}
	std::cout << "num of items = "<< num_of_items << std::endl;
	std::cout << "size of table = " << size_of_table << std::endl;
}

#endif