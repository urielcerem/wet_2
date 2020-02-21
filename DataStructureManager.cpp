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


DSManager::~DSManager() {
    delete servers;
    delete data_centers;
    delete traffic_tree;
}

static bool isValidDCID(int Id, int numOfDC){
	return ((Id > 0) && (Id <= numOfDC));
}

StatusTypeDSM
DSManager::MergeDataCenters(DSManager *DS, int dataCenter1, int dataCenter2) {
    if (DS == nullptr || !isValidDCID(dataCenter1, (*DS).num_of_DS) || !isValidDCID(dataCenter2,(*DS).num_of_DS))
        return INVALID_INPUT_DSM;
    int root1 = (*DS).data_centers->Find(dataCenter1)->getID();
    int root2 = (*DS).data_centers->Find(dataCenter2)->getID();
	if (root1 == root2) return SUCCESS_DSM;
    DataCenter* head1 = (*DS).data_centers->Find(root1)->getData();
    DataCenter* head2 = (*DS).data_centers->Find(root2)->getData();
	int new_root = (*DS).data_centers->Union(root1, root2)->getID();
	if (new_root == root1) {
		head1->trafficTree = mergeTrees<Server>(head1->getTrafficTree(), head2->getTrafficTree());
		head2->trafficTree = nullptr;
	}
	else {
		head2->trafficTree = mergeTrees<Server>(head2->getTrafficTree(), head1->getTrafficTree());
		head1->trafficTree = nullptr;
	}
    return SUCCESS_DSM;
}


StatusTypeDSM
DSManager::AddServer(DSManager *DS, int dataCenterID, int serverID) {
    if (DS == nullptr || !isValidDCID(dataCenterID, (*DS).num_of_DS) || (serverID <=0))
        return INVALID_INPUT_DSM;
    if (servers->Find(serverID) != nullptr)
        return FAILURE_DSM;
	Server* new_server = new Server(serverID, dataCenterID);
    if (&new_server == nullptr) {
        return ALLOCATION_ERROR_DSM;
    }
	switch (servers->Insert(serverID, new_server)){
        case FAILURE_HT:
            return FAILURE_DSM;
        case ALLOCATION_ERROR_HT:
            return ALLOCATION_ERROR_DSM;
        case SUCCESS_HT:
            break;
    }
    ++num_of_server;
    return (StatusTypeDSM)data_centers->Find(dataCenterID)->getData()->AddServer();
}

StatusTypeDSM DSManager::RemoveServer(DSManager *DS, int serverID) {
    if (DS == nullptr ||  (serverID <=0))
        return INVALID_INPUT_DSM;
    Server* to_remove = servers->Find(serverID);
	if (to_remove == nullptr)
		return FAILURE_DSM;
    int data_center_id = to_remove->BelongsToDataCenter();
    int traffic = to_remove->Traffic();
    if (to_remove == nullptr)
        return FAILURE_DSM;
    traffic_tree->Delete((double)traffic + (double)(1 / ((double)serverID * 2)));
    data_centers->Find(data_center_id)->getData()->getTrafficTree()->Delete((double)traffic + (double)(1 / ((double)serverID * 2)));
    return (((StatusTypeDSM)servers->Remove(serverID)==SUCCESS_DSM) &&
            ((StatusTypeDSM)data_centers->Find(data_center_id)->getData()->RemoveServer() == SUCCESS_DSM))
            ? SUCCESS_DSM: FAILURE_DSM;
}

StatusTypeDSM DSManager::SetTraffic(DSManager *DS, int serverID, int traffic) {
	if (serverID <= 0 || traffic < 0 || DS == nullptr)
		return INVALID_INPUT_DSM;
	Server *server = DS->servers->Find(serverID);
	if (server == nullptr)
		return FAILURE_DSM;
	int data_center = server->BelongsToDataCenter();
	double key = (double)traffic + (double)(1 / ((double)serverID * 2));
	if ((*server).Traffic() != 0){
		double old_key = (double)(server->Traffic())
			+ (double)(1 / ((double)serverID * 2));
		DS->traffic_tree->Delete(old_key);
		DS->data_centers->Find(data_center)->getData()->getTrafficTree()->
			Delete(old_key);
	}
	(*server).updateTraffic(traffic);
	DS->traffic_tree->Insert(*server, key);
	DS->data_centers->Find(data_center)->getData()->getTrafficTree()->
		Insert(*server, key);
    return SUCCESS_DSM;
}

StatusTypeDSM
DSManager::SumHighestTrafficServers(DSManager *DS, int dataCenterID, int k,
                                    int *traffic) {
	if (k < 0 || DS == nullptr || dataCenterID > DS->num_of_DS ||
		dataCenterID < 0 || traffic == nullptr)
		return INVALID_INPUT_DSM;
	if (dataCenterID == 0)
		*traffic = DS->traffic_tree->GetKHighestSum(k);
	else {
		*traffic = DS->data_centers->Find(dataCenterID)->
			getData()->getTrafficTree()->GetKHighestSum(k);
	}
	return SUCCESS_DSM;

}
