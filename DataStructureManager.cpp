#include "Array.h"
#include "list.h"
#include "AVL.h"
#include "hash_table.h"
#include "Union-Find.h"
#include "DataCenterAndServer2.h"
#include "DataStructureManager.h"


DSManager::DSManager(int numOfDS): num_of_DS(numOfDS), num_of_server(0){
    servers = new HASH_TABLE <Server>;
    data_centers = new UnionFindSet<DataCenter>(numOfDS);
    traffic_tree = new AVLTree <Server>;
}

DSManager *DSManager::Init(int num) {
    return new DSManager(num);
}

static bool isValidDCID(int Id, int numOfDC){
    return ((Id >0) && (Id<numOfDC));
}

StatusTypeDSM
DSManager::MergeDataCenters(DSManager *DS, int dataCenter1, int dataCenter2) {
    if (DS == NULL || !isValidDCID(dataCenter1, (*DS).num_of_DS) || !isValidDCID(dataCenter2,(*DS).num_of_DS))
        return INVALID_INPUT_DSM;
    int root1 = (*DS).data_centers->Find(dataCenter1)->getID();
    int root2 = (*DS).data_centers->Find(dataCenter2)->getID();
    int new_root = (*DS).data_centers->Union(root1, root2)->getID();
    DataCenter head1 = (*DS).data_centers->Find(root1)->getData();
    DataCenter head2 = (*DS).data_centers->Find(root2)->getData();
    if (new_root == root1) {
        for (int i = 0; i < head2.NumOfServers(); ++i) {
            head1.getTrafficTree()->Insert(head2.getTrafficTree()->root->data,
                                           head2.getTrafficTree()->root->key);
            head2.getTrafficTree()->Delete(head2.getTrafficTree()->root->key);
        }
    }
    else {
            for (int i = 0; i < head1.NumOfServers() ; ++i) {
                head2.getTrafficTree()->Insert(head1.getTrafficTree()->root->data, head1.getTrafficTree()->root->key);
                head1.getTrafficTree()->Delete(head1.getTrafficTree()->root->key);
            }
    }
    return SUCCESS_DSM;
}

StatusTypeDSM
DSManager::AddServer(DSManager *DS, int dataCenterID, int serverID) {
    if (DS == NULL || !isValidDCID(dataCenterID, (*DS).num_of_DS) || (serverID <=0))
        return INVALID_INPUT_DSM;
    Server new_server(serverID, dataCenterID);
    if (&new_server == NULL)
        return ALLOCATION_ERROR_DSM;
    if (servers->Find(serverID) != NULL)
        return FAILURE_DSM;
    switch ((StatusTypeDSM)servers->Insert(serverID, &new_server)){
        case FAILURE_DSM:
            return FAILURE_DSM;
        case ALLOCATION_ERROR_DSM:
            return ALLOCATION_ERROR_DSM;
        case INVALID_INPUT_DSM:
            return INVALID_INPUT_DSM;
        case SUCCESS_DSM:
            break;
    }
    ++num_of_server;
    return (StatusTypeDSM)data_centers->Find(serverID)->getData().AddServer();
}

StatusTypeDSM DSManager::RemoveServer(DSManager *DS, int serverID) {
    if (DS == NULL ||  (serverID <=0))
        return INVALID_INPUT_DSM;
    Server* to_remove = servers->Find(serverID);
    if (to_remove == NULL)
        return FAILURE_DSM;
    traffic_tree->Delete(serverID);
    data_centers->Find(serverID)->getData().getTrafficTree()->Delete(serverID);
    return (((StatusTypeDSM)servers->Remove(serverID)==SUCCESS_DSM) &&
            ((StatusTypeDSM)data_centers->Find(serverID)->getData().RemoveServer() == SUCCESS_DSM))
            ? SUCCESS_DSM: FAILURE_DSM;
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

