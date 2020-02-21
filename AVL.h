#ifndef AVL_TREE_H_
#define AVL_TREE_H_
#include<iostream>
#include<math.h>

/* AVL node */
template <class T>
class AVLNode {
public:
	T data;
	double key;
	int left_sum, right_sum;
	int num_of_left, num_of_right;
	AVLNode *left, *right;
	int depth;

	AVLNode(T d , double k = 0) : data(d), key(k), depth(1), num_of_left(0), 
		num_of_right(0) ,left_sum(0), right_sum(0), left(nullptr), right(nullptr) {}
	~AVLNode() = default;
};

using namespace std;

/* AVL tree that sort <T> type of datas by keys*/
template <class T>
class AVLTree {
public:
	AVLNode<T>* root;

	AVLTree() : root(nullptr) {}
	~AVLTree();

	void Insert(T data, double key);
	void Delete(double key);
	T* Find(double key);

	void PrintInOrder();//for debugging purposes
	AVLNode<T>* GetKHighestNde(int k);
	int GetKHighestSum(int k);
};

using namespace std;


// functions for merging 2 trees //
template <class T>
void TreeToArray(AVLNode<T> *node, AVLNode<T>** arr, int* index) {
	if (node != nullptr)
	{
		TreeToArray(node->left, arr, index);
		arr[*index] = new AVLNode<T>(node->data, node->key);
		(*index)++;
		TreeToArray(node->right, arr, index);
	}
}

// functions for merging 2 trees //
template <class T>
void mergeArrays(AVLNode<T>** arr1, AVLNode<T>** arr2, AVLNode<T>** merged_arr, int size1, int size2) {
	int index1 = 0, index2 = 0, index = 0;
	for (int i = 0; i < size1 + size2; i++) {
		if (index2 >= size2 || ( index1 < size1 && (arr1[index1])->key < (arr2[index2])->key) )
		{
			merged_arr[index] = arr1[index1];
			index1++;
		}
		else
		{
			merged_arr[index] = arr2[index2];
			index2++;
		}
		index++;
	}
}

// functions for merging 2 trees //
template <class T>
AVLNode<T>* makeEmptyTree(int hight) {
	if (hight == 0)
		return nullptr;
	T nothing(0,0); //only used to set up the tree, value has no meaning
	AVLNode<T>* node = new AVLNode<T>(nothing);
	node->depth = hight;
	node->left = makeEmptyTree<T>(hight - 1);
	node->right = makeEmptyTree<T>(hight - 1);
	return node;
}

// functions for merging 2 trees //
template <class T>
AVLNode<T>* removeTopLeafs(AVLNode<T>* node, int* num_of_leafs_to_remove) {
	if (*num_of_leafs_to_remove > 0 && node->depth == 1) {
		delete node;
		(*num_of_leafs_to_remove)--;
		return nullptr;
	}
	if (*num_of_leafs_to_remove > 0 && node != nullptr)
	{
		node->right = removeTopLeafs(node->right, num_of_leafs_to_remove);
		node->left = removeTopLeafs(node->left, num_of_leafs_to_remove);
	}
	return node;
}

// functions for merging 2 trees //
template <class T>
void insertArrayIntoTree(AVLNode<T>* node, AVLNode<T>** arr, int* index) {
	if (node == nullptr)
		return;
	insertArrayIntoTree(node->left, arr, index);
	node->key = arr[*index]->key;
	node->data = arr[*index]->data;
	delete arr[*index];
	(*index)++;
	insertArrayIntoTree(node->right, arr, index);
	if (node->left != nullptr) {
		node->num_of_left = node->left->num_of_left + node->left->num_of_right + 1;
		node->left_sum = node->left->left_sum + node->left->right_sum + (int)node->left->key;
	}
	if (node->right != nullptr) {
		node->num_of_right = node->right->num_of_left + node->right->num_of_right + 1;
		node->right_sum = node->right->left_sum + node->right->right_sum + (int)node->right->key;
	}
}

// functions for merging 2 trees //
template <class T>
AVLTree<T>* makeArrayToTree(AVLNode<T>** arr, int size) {
	int hight = 0;
	while (pow(2, hight) < (size / 2) * 2)
		hight++;
	hight++;
	AVLNode<T>* root = makeEmptyTree<T>(hight);
	int num_of_leafs_to_remove = (int)pow(2, hight) - size - 1;
	removeTopLeafs(root, &num_of_leafs_to_remove);
	int index = 0;
	insertArrayIntoTree(root, arr, &index);
	AVLTree<T>* tree = new AVLTree<T>;
	tree->root = root;
	return tree;
}

// functions for merging 2 trees //
template <class T>
void getSizeOfTree(AVLNode<T>* node, int* size) {
	if (node == nullptr)
		return;
	getSizeOfTree(node->left, size);
	(*size)++;
	getSizeOfTree(node->right, size);
}

// functions for merging 2 trees //
template <class T>
AVLTree<T>* mergeTrees(AVLTree<T>* tree1, AVLTree<T>* tree2) {
	int size1 = 0, size2 = 0;
	getSizeOfTree(tree1->root, &size1); 
	getSizeOfTree(tree2->root, &size2);
	if (size1 == 0 && size2 == 0) {
		delete tree2;
		return tree1; }
	AVLNode<T>** merged_arr = new AVLNode<T>*[size1 + size2];
	AVLNode<T>** arr1 = new AVLNode<T>*[size1];
	AVLNode<T>** arr2 = new AVLNode<T>*[size2];
	int index = 0;
	TreeToArray(tree1->root, arr1, &index);
	index = 0;
	TreeToArray(tree2->root, arr2, &index);
	delete tree1;
	delete tree2;
	mergeArrays(arr1, arr2, merged_arr, size1, size2);
	delete[] arr1;
	delete[] arr2;
	AVLTree<T>* tree = makeArrayToTree(merged_arr, size1 + size2);
	delete[] merged_arr;
	return tree;
}



//get the maximum of two numbers
static int getMax(int first, int second) {
	return (first > second) ? first : second;
}

template <class T>
void DeleteTree(AVLNode<T> *root)
{
	if (root != nullptr)
	{
		DeleteTree(root->left);
		DeleteTree(root->right);
		delete root;
	}
}

template <class T>
AVLTree<T>::~AVLTree()
{
	DeleteTree(root);
}

//function to get height of the tree
template <class T>
int getDepth(AVLNode<T>* node)
{
	if (node == nullptr)
		return 0;
	return node->depth;
}

// rotating a tree/sub-tree that has node as its own root
template <class T>
AVLNode<T>* RotateRight(AVLNode<T>* node)
{
	AVLNode<T>* left_node = node->left;
	AVLNode<T>* right_node = left_node->right;

	// Perform rotation
	left_node->right = node;
	node->left = right_node;

	//updates sums
	if (right_node) {
		node->left_sum = right_node->left_sum + right_node->right_sum + (int)right_node->key;
		node->num_of_left = right_node->num_of_left + right_node->num_of_right + 1;
	}
	else {
		node->left_sum = 0;
		node->num_of_left = 0;
	}

	left_node->right_sum = node->left_sum + node->right_sum + (int)node->key;
	left_node->num_of_right = node->num_of_left + node->num_of_right + 1;

	// Update heights
	node->depth = getMax(getDepth(node->left),
		getDepth(node->right)) + 1;
	left_node->depth = getMax(getDepth(left_node->left),
		getDepth(left_node->right)) + 1;

	// Return new root
	return left_node;
}

// rotating a tree/sub-tree that has node as its own root
template <class T>
AVLNode<T>* RotateLeft(AVLNode<T>* node)
{
	AVLNode<T>* right_node = node->right;
	AVLNode<T>* left_node = right_node->left;

	// Perform rotation
	right_node->left = node;
	node->right = left_node;

	//updates sums 
	if (left_node) {
		node->right_sum = left_node->left_sum + left_node->right_sum + (int)left_node->key;
		node->num_of_right = left_node->num_of_left + left_node->num_of_right + 1;
	}
	else {
		node->right_sum = 0;
		node->num_of_right = 0;
	}

	right_node->left_sum = node->left_sum + node->right_sum + (int)node->key;
	right_node->num_of_left = node->num_of_left + node->num_of_right + 1;

	// Update heights
	node->depth = getMax(getDepth(node->left),
		getDepth(node->right)) + 1;
	right_node->depth = getMax(getDepth(right_node->left),
		getDepth(right_node->right)) + 1;

	// Return new root
	return right_node;
}

// Get the Balance factor of a node
template <class T>
int getBalance(AVLNode<T>* node)
{
	if (node == nullptr)
		return 0;
	return getDepth(node->left) - getDepth(node->right);
}

//Insert a key of type T into the AVL tree
template <class T>
AVLNode<T>* InsertDataByKey(AVLNode<T>* node, T data, double key, int* not_inserted) {
	if (node == nullptr)
	{
		AVLNode<T>* new_node = new AVLNode<T>(data, key);
		return new_node;
	}
	AVLNode<T>* temp_node;
	if (key < node->key) {
		node->left_sum += (int)key;
		node->num_of_left++;
		temp_node = InsertDataByKey(node->left, data, key, not_inserted);
		if (*not_inserted) {
			node->num_of_left--;
			node->left_sum -= (int)key;
		}
		node->left = temp_node;
	}
	else if (key > node->key) {
		node->right_sum += (int)key;
		node->num_of_right++;
		temp_node = InsertDataByKey(node->right, data, key, not_inserted);
		if (*not_inserted) {
			node->num_of_right--;
			node->right_sum -= (int)key;
		}
		node->right = temp_node;
	}
	// in case keys are equel
	else {
		*not_inserted = 1;
		return node;
	}

	//update height of ancestor node
	node->depth = 1 + getMax(getDepth(node->left), getDepth(node->right));

	int balance = getBalance(node);//get balance factor

	// balancing the tree based on an avl tree algoritam

	// Left Left Case
	if (balance > 1 && key < node->left->key)
		return RotateRight(node);

	// Right Right Case
	if (balance < -1 && key > node->right->key)
		return RotateLeft(node);
	// Left Right Case
	if (balance > 1 && key > node->left->key)
	{
		node->left = RotateLeft(node->left);
		return RotateRight(node);
	}

	// Right Left Case
	if (balance < -1 && key < node->right->key)
	{
		node->right = RotateRight(node->right);
		return RotateLeft(node);
	}
	return node;
}

// find the Node with minimum value that still has a bigger value then node
template <class T>
AVLNode<T> * getNextMinValue(AVLNode<T>* node)
{
	AVLNode<T>* current = node->right;

	// find the leftmost leaf */
	while (current->left != nullptr)
		current = current->left;
	return current;
}

// delete a node from AVL tree with the given key
template <class T>
AVLNode<T>* deleteNode(AVLNode<T>* root, double key, int* not_deleted)
{
	if (root == nullptr) {
		*not_deleted = 1;
		return root;
	}

	AVLNode<T>* temp_node;
	if (key < root->key) {
		root->left_sum -= (int)key;
		root->num_of_left--;
		temp_node = deleteNode(root->left, key, not_deleted);
		if (*not_deleted) {
			root->num_of_left++;
			root->left_sum += (int)key;
		}
		root->left = temp_node;
	}
	else if (key > root->key) {
		root->right_sum -= (int)key;
		root->num_of_right--;
		temp_node = deleteNode(root->right, key, not_deleted);
		if (*not_deleted) {
			root->num_of_right++;
			root->right_sum += (int)key;
		}
		root->right = temp_node;
	}

	else
	{
		// node with only one child or no child
		if ((root->right == nullptr) ||
			(root->left == nullptr))
		{
			AVLNode<T> *temp = root->left ?
				root->left :
				root->right;

			if (temp == nullptr)
			{
				temp = root;
				root = nullptr;
			}
			else // One child case
				*root = *temp;
			delete (temp);
		}
		else
		{
			AVLNode<T>* temp = getNextMinValue(root);

			root->num_of_right--;
			root->right_sum -= (int)temp->key;

			root->key = temp->key;
			root->data = temp->data;

			// Delete the inorder successor
			root->right = deleteNode(root->right, temp->key, not_deleted);
		}
	}

	if (root == nullptr)
		return root;

	// update depth
	root->depth = 1 + getMax(getDepth(root->left), getDepth(root->right));

	// get balance factor
	int balance = getBalance(root);

	//balancing the tree

	// Left Left Case
	if (balance > 1 &&
		getBalance(root->left) >= 0)
		return RotateRight(root);

	// Left Right Case
	if (balance > 1 && getBalance(root->left) < 0) {
		root->left = RotateLeft(root->left);
		return RotateRight(root);
	}
	// Right Right Case
	if (balance < -1 && getBalance(root->right) <= 0)
		return RotateLeft(root);

	// Right Left Case
	if (balance < -1 && getBalance(root->right) > 0) {
		root->right = RotateRight(root->right);
		return RotateLeft(root);
	}
	return root;
}

template <class T>
AVLNode<T>* findKey(AVLNode<T>* root, double key) {
	if (root == nullptr)
		return root;

	if (key < root->key)
		return findKey(root->left, key);
	else if (key > root->key)
		return findKey(root->right, key);
	else
		return root;
}



// for debugging purpuses only //
template <class T>
void GetRank(AVLNode<T>* node, int key, int* rank) {
	if (node != nullptr)
	{
		if (node->key < key)
		{
			*rank += node->num_of_left + 1;
			GetRank(node->right, key, rank);
		}
		else if (node->key > key)
		{
			GetRank(node->left, key, rank);
		}
		else
			*rank += node->num_of_left + 1;
	}
}

// for debugging purpuses only //
template <class T>
void GetReverseRank(AVLNode<T>* node, int key, int* rank) {
	if (node != nullptr)
	{
		if (node->key > key)
		{
			*rank += node->num_of_right + 1;
			GetReverseRank(node->left, key, rank);
		}
		else if (node->key < key)
		{
			GetReverseRank(node->right, key, rank);
		}
		else
			*rank += node->num_of_right + 1;
	}
}

// for debugging purpuses only //
template <class T>
int Rank(AVLNode<T> *root, int key) {
	int rank = 0;
	GetRank(root, key, &rank);
	return rank;
}

// for debugging purpuses only //
template <class T>
int ReverseRank(AVLNode<T> *root, int key) {
	int rank = 0;
	GetReverseRank(root, key, &rank);
	return rank;
}



// prints AVL tree in an inorder way
template <class T>
void inOrder(AVLNode<T> *root, AVLNode<T> *node)
{

	if (node != nullptr)
	{
		inOrder(root, node->left);
		cout << node->key << "  left sum = " << node->left_sum <<
			" right sum = " << node->right_sum << "  left num = " << node->num_of_left <<
			" right num = " << node->num_of_right << " rank = "
			<< Rank(root, node->key) << " reverse rank = "
			<< ReverseRank(root, node->key) << std::endl;
		cout << "item info :   " << node->data << endl;
		inOrder(root, node->right);
	}
}

template <class T>
void AVLTree<T>::Delete(double key) {
	int not_deleted = 0;
	root = deleteNode(root, key, &not_deleted);
}

template <class T>
void AVLTree<T>::Insert(T data, double key) {
	int not_inserted = 0;
	root = InsertDataByKey(root, data, key, &not_inserted);
}

template <class T>
T* AVLTree<T>::Find(double key) {
	AVLNode<T>* data_node;
	data_node = findKey(root, key);
	return &(data_node->data);
}


template <class T>
void AVLTree<T>::PrintInOrder() {
	cout << "printing tree:         " << endl;
	inOrder(root, root);
}


template <class T>
AVLNode<T>* GetKNode(AVLNode<T>* node , int k) {
	if (node == nullptr)
		return nullptr;
	if (node->num_of_right > k)
		return GetKNode(node->right, k);
	else if (node->num_of_right < k) {
		k -= node->num_of_right + 1;
		return GetKNode(node->left, k);
	}
	else
		return node;
}


template <class T>
AVLNode<T>* AVLTree<T>::GetKHighestNde(int k) {
	return GetKNode(root, k - 1);
}


template <class T>
void GetKSum(AVLNode<T>* node, double key , int* sum) {
	//cout << node->key << "    " << *sum << endl;
	if (node->key < key)
		GetKSum(node->right, key, sum);
	else if (node->key > key) {
		*sum += node->right_sum + (int)node->key;
		GetKSum(node->left, key, sum);
	}
	else
		*sum += node->right_sum + (int)node->key;
}


template <class T>
int AVLTree<T>::GetKHighestSum(int k) {
	if (root == nullptr)
		return 0;
	int sum = 0;
	if (k == 0) return 0;
	AVLNode<T>* k_node = GetKHighestNde(k);
	//cout << k_node->key << endl;
	//cout << root->key << endl;
	if (k_node == nullptr)
		sum = root->right_sum + root->left_sum + (int)root->key;
	else
		GetKSum(root, k_node->key, &sum);
	return sum;
}






#endif
