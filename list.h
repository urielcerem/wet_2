#ifndef LIST_H
#define LIST_H 


#include <iostream>

using namespace std;

template<class T>
class ListItem {

	ListItem<T>* next;
	ListItem<T>* prev;

public:
	T data;
	explicit  ListItem(const T& value) : data(value), next(NULL), prev(NULL) {}
	~ListItem() = default;

	void SetValue(const T& value);
	const T& GetValue() const;
	void SetNext(ListItem<T>* next_item);
	ListItem<T>* GetNext() const;
	void SetPrev(ListItem<T>* prev_item);
	ListItem<T>* GetPrev() const;
	void Print() const;//for debugging purpuses/
};

template <class T>
class List {



public:
	ListItem<T>* head;
	ListItem<T>* tail;
	List() : head(NULL), tail(NULL) {}
	~List()= default;

	//insert value into a listitem type and then insert
	//the listitem into the begining of the list
	//returns pointer to the newly created listitem or null if there was an error
	ListItem<T>* PushFront(const T& value);
	ListItem<T>* PushBack(const T& value);

	/*not nedded for us
	bool PopFront(T* value);
	bool PopBack(T* value);
	*/

	int size() const;//for debugging purpuses//
	void Print() const;//for debugging purpuses//

	/* not nedded for us
	bool Insert(const T& value, const int& position);
	*/

	/*not nedded for us
	bool Delete(const int& position);
	*/

	bool DeleteItem(ListItem<T>* item_to_delete);//delet a listitem

	T PopItem(ListItem<T>* item_to_pop);//delet a listitem and return the data it was holding in it

	//insert data into a listitem and place it after item_before in the list
	ListItem<T>* Insert_after(ListItem<T>* item_before, const T& value);

	/*not nedded for us
	int  Find(const int& value) const;
	bool Get(const int& position, T* value);
	*/

	ListItem<T>* GetHead() const;//return the first listitem in the list
	ListItem<T>* GetTail() const;//return the last listitem in the list
};



template <class T>
void ListItem<T>::SetValue(const T& value) {
	data = value;
}

template <class T>
const T& ListItem<T>::GetValue() const {
	return data;
}

template <class T>
void ListItem<T>::SetNext(ListItem<T>* next_item) {
	next = next_item;
}

template <class T>
ListItem<T>* ListItem<T>::GetNext() const {
	return next;
}

template <class T>
void ListItem<T>::SetPrev(ListItem<T>* prev_item) {
	prev = prev_item;
}

template <class T>
ListItem<T>* ListItem<T>::GetPrev() const {
	return prev;
}

template <class T>
void ListItem<T>::Print() const {
	std::cout << data << std::endl;
}



/*
template <class T>
List<T>::~List() {
	ListItem<T>* current = head;

	while (current) {
		ListItem<T>* next_item = current->GetNext();
		delete(current);
		current = next_item;
	}
}
*/


// O(1)
template <class T>
ListItem<T>* List<T>::PushFront(const T& value) {
	ListItem<T>* item = new(std::nothrow) ListItem<T>(value);

	if (item) {
		if (!head) {
			// Special case, list is empty
			head = item;
			tail = item;
		}
		else {
			item->SetNext(head);
			head = item;
		}
		return item;
	}
	else {
		return NULL; //  memory allocation problam
	}
}

// O(1)
template <class T>
ListItem<T>* List<T>::PushBack(const T& value) {
	ListItem<T> *  item;
	item = new (std::nothrow) ListItem<T>(value);

	if (item) {
		if (!head) {
			// Special case, list is empty
			head = item;
			tail = item;
		}
		else {
			tail->SetNext(item);
			item->SetPrev(tail);
			tail = item;
		}
		return item;
	}
	else {
		return NULL;
	}
}


/*
// O(1)
template <class T>
bool List<T>::PopFront(T* value) {
	if (value && head) {
		*value = head->GetValue();

		ListItem<T>* new_head = head->GetNext();
		delete head;
		head = new_head;

		return false;
	}
	else
		return true; // Error: nullptr passed as parameter or list empty
}

// O(n)
template <class T>
bool List<T>::PopBack(T* value) {
	if (value && tail) {
		*value = tail->GetValue();

		// Special case: one item list
		if (head == tail) {
			delete head;
			head = NULL;
			tail = NULL;
		}
		else {
			ListItem<T>* new_tail;

			new_tail = head;

			while (new_tail->GetNext() != tail) // Skip elements till the item before tail
				new_tail = new_tail->GetNext();

			new_tail->SetNext(NULL);
			delete tail;
			tail = new_tail;
		}
		return false;
	}
	else
		return true; // Error: nullptr passed as parameter or list empty
}
*/



// O(n)
template <class T>
void List<T>::Print() const {
	ListItem<T>* e;
	for (e = head; e; e = e->GetNext())
		e->Print();
	std::cout << std::endl << "end of the list bitch" << std::endl << std::endl;
}

// O(n)
template <class T>
int List<T>::size() const {
	ListItem<T>* e;
	int count = 0;

	for (e = head; e; e = e->GetNext()) ++count;

	return count;
}


// best O(1), avg O(n), wst O(n)
/* not nedded for us
template <class T>
bool List<T>::Insert(const T& value, const int& position) {
	if (position < 0)
		return true;

	if (!position) // Special case: position = 0, insert into head
		return PushFront(value);

	ListItem<T>* item_before = head;

	for (int count = 0; count < position - 1; ++count) {
		item_before = item_before->GetNext();

		if (!item_before)
			return true; // Out of bounds
	}

	ListItem<T>* new_element = new (std::nothrow) ListItem<T>(value);
	if (!new_element)
		return true;

	new_element->SetNext(item_before->GetNext());
	item_before->SetNext(new_element);

	if (item_before == tail) // Special case: insert at the end
		tail = new_element;

	return false;
}
*/

template <class T>
ListItem<T>* List<T>::Insert_after(ListItem<T>* item_before, const T& value) {
	ListItem<T>* new_element = new (std::nothrow) ListItem<T>(value);
	if (!new_element)
		return NULL;

	new_element->SetNext(item_before->GetNext());
	new_element->SetPrev(item_before);
	item_before->SetNext(new_element);
	return new_element;
}


/*
template <class T>
bool List<T>::Delete(const int& position) {
	if (position < 0)
		return true;

	if (!head)
		return true; // List empty

	ListItem<T>* target = head;

	if (!position) {// Special case: position = 0, delete head
		head = head->GetNext();
		delete target;

		return false;
	}

	ListItem<T>* item_before = head;
	for (int count = 0; count < position - 1; ++count) {
		item_before = item_before->GetNext();

		if (!item_before)
			return true; // Out of bounds
	}

	target = item_before->GetNext();

	if (!target)
		return true; // Out of bounds

	item_before->SetNext(target->GetNext());

	if (target == tail)
		tail = item_before;

	delete target;

	return false;
}
*/


template <class T>
bool List<T>::DeleteItem(ListItem<T>* item_to_delete) {
	if (!item_to_delete) {
		return false;
	}
	ListItem<T>* prev_item = item_to_delete->GetPrev();
	ListItem<T>* next_item = item_to_delete->GetNext();

	if (item_to_delete == head) {
		head = next_item;
	}

	if (item_to_delete == tail) {
		tail = prev_item;
	}
	if (prev_item != NULL) prev_item->SetNext(next_item);

	if (next_item != NULL) next_item->SetPrev(prev_item);

	delete item_to_delete;
	return true;
}

template <class T>
T List<T>::PopItem(ListItem<T>* item_to_pop) {
	T value_to_return = item_to_pop->GetValue();
	if (DeleteItem(item_to_pop))
		return value_to_return;
	else
		return NULL;

}





/*
template <class T>
int List<T>::Find(const int& value) const {
	int position = -1;

	ListItem<T>* e = head;

	for (int count = 0; e; e = e->GetNext(), ++count) {
		if (e->GetValue() == value) {
			position = count;
			break;
		}
	}

	return position;
}

template <class T>
bool List<T>::Get(const int& position, T* value) {
	if (position < 0 || !value)
		return true; // invalid input

	ListItem<T>* e = head;

	for (int count = 0; e && count < position; e = e->GetNext(), ++count);

	if (!e)
		return true;

	*value = e->GetValue();

	return false;
}
*/




template <class T>
ListItem<T>* List<T>::GetHead() const {
	return head;
}

template <class T>
ListItem<T>* List<T>::GetTail() const {
	return tail;
}



#endif