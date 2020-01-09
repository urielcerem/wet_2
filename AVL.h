#ifndef AVL_TREE_H_
#define AVL_TREE_H_
#include<iostream>

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

	AVLNode(T d, double k) : data(d), key(k), depth(1), num_of_left(0), 
		num_of_right(0) ,left_sum(0), right_sum(0), left(NULL), right(NULL) {}
	~AVLNode() = default;
};

using namespace std;

/* AVL tree that sort <T> type of datas by keys*/
template <class T>
class AVLTree {
public:
	AVLNode<T>* root;

	AVLTree() : root(NULL) {}
	~AVLTree();

	void Insert(T data, double key);
	void Delete(double key);
	T* Find(double key);

	void PrintInOrder();//for debugging purposes
	AVLNode<T>* GetKHighestNde(int k);
	int GetKHighestSum(int k);
};

using namespace std;

//get the maximum of two numbers
static int getMax(int first, int second) {
	return (first > second) ? first : second;
}

template <class T>
void DeleteTree(AVLNode<T> *root)
{
	if (root != NULL)
	{
		DeleteTree(root->left);
		DeleteTree(root->right);
		delete(root);
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
	if (node == NULL)
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
		node->left_sum = right_node->left_sum + right_node->right_sum + right_node->key;
		node->num_of_left = right_node->num_of_left + right_node->num_of_right + 1;
	}
	else {
		node->left_sum = 0;
		node->num_of_left = 0;
	}

	left_node->right_sum = node->left_sum + node->right_sum + node->key;
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
		node->right_sum = left_node->left_sum + left_node->right_sum + left_node->key;
		node->num_of_right = left_node->num_of_left + left_node->num_of_right + 1;
	}
	else {
		node->right_sum = 0;
		node->num_of_right = 0;
	}

	right_node->left_sum = node->left_sum + node->right_sum + node->key;
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
	if (node == NULL)
		return 0;
	return getDepth(node->left) - getDepth(node->right);
}

//Insert a key of type T into the AVL tree
template <class T>
AVLNode<T>* InsertDataByKey(AVLNode<T>* node, T data, double key, int* not_inserted) {
	if (node == NULL)
	{
		AVLNode<T>* new_node = new AVLNode<T>(data, key);
		return new_node;
	}
	AVLNode<T>* temp_node;
	if (key < node->key) {
		node->left_sum += key;
		node->num_of_left++;
		temp_node = InsertDataByKey(node->left, data, key, not_inserted);
		if (*not_inserted) {
			node->num_of_left--;
			node->left_sum -= key;
		}
		node->left = temp_node;
	}
	else if (key > node->key) {
		node->right_sum += key;
		node->num_of_right++;
		temp_node = InsertDataByKey(node->right, data, key, not_inserted);
		if (*not_inserted) {
			node->num_of_right--;
			node->right_sum -= key;
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
	while (current->left != NULL)
		current = current->left;
	return current;
}

// delete a node from AVL tree with the given key
template <class T>
AVLNode<T>* deleteNode(AVLNode<T>* root, double key, int* not_deleted)
{
	if (root == NULL) {
		*not_deleted = 1;
		return root;
	}

	AVLNode<T>* temp_node;
	if (key < root->key) {
		root->left_sum -= key;
		root->num_of_left--;
		temp_node = deleteNode(root->left, key, not_deleted);
		if (*not_deleted) {
			root->num_of_left++;
			root->left_sum += key;
		}
		root->left = temp_node;
	}
	else if (key > root->key) {
		root->right_sum -= key;
		root->num_of_right--;
		temp_node = deleteNode(root->right, key, not_deleted);
		if (*not_deleted) {
			root->num_of_right++;
			root->right_sum += key;
		}
		root->right = temp_node;
	}

	else
	{
		// node with only one child or no child
		if ((root->right == NULL) ||
			(root->left == NULL))
		{
			AVLNode<T> *temp = root->left ?
				root->left :
				root->right;

			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case
				*root = *temp;
			delete (temp);
		}
		else
		{
			AVLNode<T>* temp = getNextMinValue(root);

			root->num_of_right--;
			root->right_sum -= temp->key;

			root->key = temp->key;
			root->data = temp->data;

			// Delete the inorder successor
			root->right = deleteNode(root->right, temp->key, not_deleted);
		}
	}

	if (root == NULL)
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
	if (root == NULL)
		return root;

	if (key < root->key)
		return findKey(root->left, key);
	else if (key > root->key)
		return findKey(root->right, key);
	else
		return root;
}

template <class T>
void GetRank(AVLNode<T>* node, int key, int* rank) {
	if (node != NULL)
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


template <class T>
void GetReverseRank(AVLNode<T>* node, int key, int* rank) {
	if (node != NULL)
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

template <class T>
int Rank(AVLNode<T> *root, int key) {
	int rank = 0;
	GetRank(root, key, &rank);
	return rank;
}


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

	if (node != NULL)
	{
		inOrder(root, node->left);
		cout << node->key << "  left sum = " << node->left_sum <<
			" right sum = " << node->right_sum << "  left num = " << node->num_of_left <<
			" right num = " << node->num_of_right << " rank = "
			<< Rank(root, node->key) << " reverse rank = "
			<< ReverseRank(root, node->key) << std::endl;
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
	if (node == NULL)
		return NULL;
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
void GetKSum(AVLNode<T>* node, int key , int* sum) {
	if (node->key < key)
		GetKSum(node->right, key, sum);
	else if (node->key > key) {
		*sum += node->right_sum + node->key;
		GetKSum(node->left, key, sum);
	}
	else
		*sum += node->right_sum + node->key;
}


template <class T>
int AVLTree<T>::GetKHighestSum(int k) {
	if (root == NULL)
		return 0;
	int sum = 0;
	AVLNode<T>* k_node = GetKHighestNde(k);
	if (k_node == NULL)
		sum = root->right_sum + root->left_sum + root->key;
	else
		GetKSum(root, k_node->key, &sum);
	return sum;
}






#endif
