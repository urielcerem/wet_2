//
// Created by Uriel on 30-Dec-19.
//

#ifndef WET_1_UNION_FIND_H
#define WET_1_UNION_FIND_H



#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif // _DEBUG

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif




typedef enum {
    SUCCESS_UFC = 0,
    FAILURE_UFC = -1,
    ALLOCATION_ERROR_UFC = -2,
    INVALID_INPUT_UFC = -3
} StatusTypeUFSet;

template <class T>
class UFSetNode{
    int Id;
    T* data;
public:
    UFSetNode <T> * parent;
    int NumOfNodes;

    explicit UFSetNode(int Id, T* data) :Id(Id), data(data){
        parent = NULL;
        NumOfNodes = 1;
    }
	~UFSetNode() {
		delete data;
	}
    int getID () {
        return Id;
    }
    T * getData (){
        return data;
    }

};

template <class T>
class UnionFindSet {
    int NumOfObjects;
     UFSetNode<T> ** array;

public:
    /**--constructor as MakeSet---**/
    explicit UnionFindSet (int NumOfObjects): NumOfObjects(NumOfObjects) {
        array = new UFSetNode<T>*[NumOfObjects];
        for (int i = 0; i <NumOfObjects ; ++i) {
            array[i] = new UFSetNode <T> (i + 1, (new T));
        }
    }
    ~UnionFindSet () {
        for (int i = 0; i <NumOfObjects ; ++i)
            delete array[i];
        delete [] array;
    }

    /**--Find with path compression---**/
    UFSetNode <T> * Find (int i){
        UFSetNode <T> * root = array[i-1];
        UFSetNode <T> * connect_to_Root = array[i-1];
		//std::cout << root << std::endl;
		//std::cout << connect_to_Root << std::endl;
		UFSetNode <T> * next;
        while (root->parent != NULL)
            root = root->parent;
        while (connect_to_Root != root && connect_to_Root->parent != root) {
            UFSetNode <T> * next = connect_to_Root->parent;
            connect_to_Root->parent = root;
            connect_to_Root = next;
        }
        return  root;
    }

    /**--Union by size---**/
	UFSetNode <T> *Union (int i, int j) {
		UFSetNode <T> * root1 = array[i - 1];
		UFSetNode <T> * root2 = array[j - 1];
        if (root1->NumOfNodes <= root2->NumOfNodes){
            root2->NumOfNodes += root1->NumOfNodes;
            root1->parent = root2;
            root1->NumOfNodes = -1;
			//array[i] = root2;
			return root2;
        }
        else{
            root1->NumOfNodes += root2->NumOfNodes;
            root2->parent = root1;
            root2->NumOfNodes = -1;
			//array[j] = root1;
			return root1;
        }
    }
};

#endif //WET_1_UNION_FIND_H
