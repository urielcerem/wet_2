//
// Created by Uriel on 05-Jan-20.
//

#ifndef WET_1_DATASTRUCTUREMANAGER_H
#define WET_1_DATASTRUCTUREMANAGER_H
#include "Array.h"
#include "list.h"
#include "AVL.h"
#include "hash_table.h"
#include "Union-Find.h"
#include "DataCenterAndServer2.h"

typedef enum {
    SUCCESS_DSM = 0,
    FAILURE_DSM = -1,
    ALLOCATION_ERROR_DSM = -2,
    INVALID_INPUT_DSM = -3
} StatusTypeDSM;

class DSManager {
    int num_of_DS;
    int num_of_server;
    HASH_TABLE <Server>* servers;
    UnionFindSet <DataCenter>* data_centers;
    AVLTree <Server>* traffic_tree;

public:
    explicit DSManager(int numOfDS);
    ~DSManager() = default;
    DSManager * Init(int num);
    StatusTypeDSM MergeDataCenters(DSManager *DS, int dataCenter1, int dataCenter2);
    StatusTypeDSM AddServer(DSManager *DS, int dataCenterID, int serverID);
    StatusTypeDSM RemoveServer(DSManager *DS, int serverID);
    StatusTypeDSM SetTraffic(DSManager *DS, int serverID, int traffic);
    StatusTypeDSM SumHighestTrafficServers(DSManager *DS, int dataCenterID, int k, int *traffic);
    void Quit(DSManager **DS);
};

#endif //WET_1_DATASTRUCTUREMANAGER_H
