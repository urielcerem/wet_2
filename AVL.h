#ifndef AVL_TREE_H_
#define AVL_TREE_H_
#include<iostream>

/* AVL node */
template <class T>
class AVLNode {
public:
	T data;
	double key;
	AVLNode *left, *right;
	int depth;

	AVLNode(T d , double k) : data(d), key(k), depth(1), left(NULL), right(NULL) {}
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
};

using namespace std;

//get the maximum of two numbers
static int getMax(int first, int second){
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
AVLNode<T>* InsertDataByKey(AVLNode<T>* node, T data , double key) {
    if (node == NULL)
    {
        AVLNode<T>* new_node = new AVLNode<T>(data, key);
        return new_node;
    }

    if (key < node->key)
        node->left = InsertDataByKey(node->left, data, key);
    else if (key > node->key)
        node->right = InsertDataByKey(node->right, data, key);
    else // in case keys are equel
        return node;

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
AVLNode<T>* deleteNode(AVLNode<T>* root, double key)
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);

    else if (key > root->key)
        root->right = deleteNode(root->right, key);

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

            root->key = temp->key;
			root->data = temp->data;

            // Delete the inorder successor
            root->right = deleteNode(root->right,
                                     temp->key);
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
AVLNode<T>* findKey(AVLNode<T>* root,double key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        return findKey(root->left, key);
    else if (key > root->key)
        return findKey(root->right, key);
    else
        return root;
}

// prints AVL tree in an inorder way
template <class T>
void inOrder(AVLNode<T> *root)
{
    if (root != NULL)
    {
        inOrder(root->left);
        cout << root->key << std::endl;
        inOrder(root->right);
    }
}

template <class T>
void AVLTree<T>::Delete(double key) {
    root = deleteNode(root, key);
}

template <class T>
void AVLTree<T>::Insert(T data, double key) {
    root = InsertDataByKey(root, data, key);
}

template <class T>
T* AVLTree<T>::Find(double key) {
    AVLNode<T>* data_node;
    data_node = findKey(root, key);
    return &(data_node->data);
}


template <class T>
void AVLTree<T>::PrintInOrder() {
    inOrder(root);
}

#endif