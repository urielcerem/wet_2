
#ifndef WET_1_DATACENTER_SERVER_H
#define WET_1_DATACENTER_SERVER_H

#include "AVL.h"
#include <iostream>
using namespace std;

typedef enum {
	SUCCESS_DC = 0,
	FAILURE_DC = -1,
	ALLOCATION_ERROR_DC = -2,
	INVALID_INPUT_DC = -3
} StatusTypeDC;

class Server {
	int id;
    int DCParentId;
    int traffic;

public:

	explicit Server(int id, int DCParentId) :
		id(id), DCParentId(DCParentId), traffic(0) {}
	~Server() = default;

    int &ID();
	int Traffic();
    int & BelongsToDataCenter();
	StatusTypeDC updateTraffic(int new_traffic);
	StatusTypeDC updateDataCenter(int Id);

	friend ostream & operator << (ostream& out, const Server &s);
};


class DataCenter {
	int num_of_servers;
public:
	AVLTree <Server>* trafficTree;
	DataCenter();
	~DataCenter() {
		delete trafficTree;
	}
	DataCenter(DataCenter & DC) = default;
	int & NumOfServers();
    AVLTree <Server>* getTrafficTree();
    StatusTypeDC AddServer();
    StatusTypeDC RemoveServer();
};
#endif //WET_1_DATACENTER_SERVER_H