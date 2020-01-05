#include "Array.h"
#include "list.h"
#include "AVL.h"
#include "hash_table.h"
#include "Union-Find.h"
#include "DataCenterAndServer2.h"
#include "DataStructureManager.h"


DSManager::DSManager(int numOfDS) {

}

DSManager *DSManager::Init(int num) {
    return nullptr;
}

StatusTypeDSM
DSManager::MergeDataCenters(DSManager *DS, int dataCenter1, int dataCenter2) {
    return FAILURE_DSM;
}

StatusTypeDSM
DSManager::AddServer(DSManager *DS, int dataCenterID, int serverID) {
    return FAILURE_DSM;
}

StatusTypeDSM DSManager::RemoveServer(DSManager *DS, int serverID) {
    return FAILURE_DSM;
}

StatusTypeDSM DSManager::SetTraffic(DSManager *DS, int serverID, int traffic) {
    return FAILURE_DSM;
}

StatusTypeDSM
DSManager::SumHighestTrafficServers(DSManager *DS, int dataCenterID, int k,
                                    int *traffic) {
    return FAILURE_DSM;
}

void DSManager::Quit(DSManager **DS) {

}
