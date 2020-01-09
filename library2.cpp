//
// Created by Uriel on 07-Jan-20.
//

#include "library2.h"
#include "DataStructureManager.h"

void *Init(int n) {
    DSManager *DS = new DSManager(n);
    return  (void*)DS;
}

StatusType MergeDataCenters(void *DS, int dataCenter1, int dataCenter2) {
    return (StatusType)(((DSManager*)DS)->MergeDataCenters((DSManager*)DS, dataCenter1,dataCenter2));
}

StatusType AddServer(void *DS, int dataCenterID, int serverID) {
    return (StatusType)(((DSManager*)DS)->AddServer((DSManager*)DS, dataCenterID, serverID));
}

StatusType RemoveServer(void *DS, int serverID) {
    return (StatusType)(((DSManager*)DS)->RemoveServer((DSManager*)DS, serverID));
}

StatusType SetTraffic(void *DS, int serverID, int traffic) {
    return (StatusType)(((DSManager*)DS)->SetTraffic((DSManager*)DS, serverID, traffic));
}

StatusType
SumHighestTrafficServers(void *DS, int dataCenterID, int k, int *traffic) {
    return (StatusType)(((DSManager*)DS)->SumHighestTrafficServers((DSManager*)DS,dataCenterID,k,traffic));
}

void Quit(void **DS) {
	delete ((DSManager*)(*DS));
	*DS = NULL;
}

