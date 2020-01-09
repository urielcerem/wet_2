#include "AVL.h"
#include "hash_table.h"
#include "Union-Find.h"
#include "DataCenterAndServer2.h"
#include "DataStructureManager.h"

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif // _DEBUG

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif



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
    if (DS == NULL || !isValidDCID(dataCenter1, (*DS).num_of_DS) || !isValidDCID(dataCenter2,(*DS).num_of_DS))
        return INVALID_INPUT_DSM;
    int root1 = (*DS).data_centers->Find(dataCenter1)->getID();
    int root2 = (*DS).data_centers->Find(dataCenter2)->getID();
	//cout << root1 << "   " << root2;
	//(*DS).data_centers->Find(root1)->getData()->getTrafficTree()->PrintInOrder();
	//(*DS).data_centers->Find(root2)->getData()->getTrafficTree()->PrintInOrder();
	//cout << new_root;
	//(*DS).data_centers->Find(new_root)->getData()->getTrafficTree()->PrintInOrder();
    DataCenter* head1 = (*DS).data_centers->Find(root1)->getData();
    DataCenter* head2 = (*DS).data_centers->Find(root2)->getData();
	//head1->getTrafficTree()->PrintInOrder();
	//head2->getTrafficTree()->PrintInOrder();
	int new_root = (*DS).data_centers->Union(root1, root2)->getID();
	//cout << head1->NumOfServers() << endl;
	//cout << head2->NumOfServers() << endl;
    if (new_root == root1) {
        while (head2->getTrafficTree()->root != NULL) {
			(*head1).getTrafficTree()->Insert((*head2).getTrafficTree()->root->data, (*head2).getTrafficTree()->root->key);
            (*head2).getTrafficTree()->Delete((*head2).getTrafficTree()->root->key);
        }
    }
    else {
		while (head1->getTrafficTree()->root != NULL) {
			    (*head2).getTrafficTree()->Insert((*head1).getTrafficTree()->root->data, (*head1).getTrafficTree()->root->key);
                (*head1).getTrafficTree()->Delete((*head1).getTrafficTree()->root->key);
        }
    }
	//(*DS).data_centers->Find(new_root)->getData()->getTrafficTree()->PrintInOrder();
    return SUCCESS_DSM;
}

StatusTypeDSM
DSManager::AddServer(DSManager *DS, int dataCenterID, int serverID) {
    if (DS == NULL || !isValidDCID(dataCenterID, (*DS).num_of_DS) || (serverID <=0))
        return INVALID_INPUT_DSM;
    if (servers->Find(serverID) != NULL)
        return FAILURE_DSM;
	Server* new_server = new Server(serverID, dataCenterID);
    if (&new_server == NULL) {
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
	//cout <<"dataCenterID is :   "<< dataCenterID << "   num of ser = " << data_centers->Find(dataCenterID)->getData()->NumOfServers() + 1 << endl;
    return (StatusTypeDSM)data_centers->Find(dataCenterID)->getData()->AddServer();
}

StatusTypeDSM DSManager::RemoveServer(DSManager *DS, int serverID) {
    if (DS == NULL ||  (serverID <=0))
        return INVALID_INPUT_DSM;
    Server* to_remove = servers->Find(serverID);
	if (to_remove == NULL)
		return FAILURE_DSM;
	//cout << to_remove;
    int data_center_id = to_remove->BelongsToDataCenter();
    int traffic = to_remove->Traffic();
    if (to_remove == NULL)
        return FAILURE_DSM;
    traffic_tree->Delete((double)traffic + (double)(1 / ((double)serverID * 2)));
    data_centers->Find(data_center_id)->getData()->getTrafficTree()->Delete((double)traffic + (double)(1 / ((double)serverID * 2)));
	//cout << "dataCenterID is :   " << data_center_id << "   num of ser = " << data_centers->Find(data_center_id)->getData()->NumOfServers() - 1 << endl;
    return (((StatusTypeDSM)servers->Remove(serverID)==SUCCESS_DSM) &&
            ((StatusTypeDSM)data_centers->Find(data_center_id)->getData()->RemoveServer() == SUCCESS_DSM))
            ? SUCCESS_DSM: FAILURE_DSM;
}

StatusTypeDSM DSManager::SetTraffic(DSManager *DS, int serverID, int traffic) {
	if (serverID <= 0 || traffic < 0 || DS == NULL)
		return INVALID_INPUT_DSM;
	//DS->servers->PrintTable();
	Server *server = DS->servers->Find(serverID);
	if (server == NULL)
		return FAILURE_DSM;
	//cout << (*server).ID() << "   " << (*server).Traffic() << endl;
	int data_center = server->BelongsToDataCenter();
	//std::cout << data_center << std::endl;
	double key = (double)traffic + (double)(1 / ((double)serverID * 2));
	//double x = 1 + 1 / 2;
	//cout << key << endl;
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
	//DS->servers->PrintTable();
    return SUCCESS_DSM;
}

StatusTypeDSM
DSManager::SumHighestTrafficServers(DSManager *DS, int dataCenterID, int k,
                                    int *traffic) {
	if (k < 0 || DS == NULL || dataCenterID > DS->num_of_DS ||
		dataCenterID < 0 || traffic == NULL)
		return INVALID_INPUT_DSM;
	//DS->servers->PrintTable();
	//DS->traffic_tree->PrintInOrder();
	if (dataCenterID == 0)
		*traffic = DS->traffic_tree->GetKHighestSum(k);
	else {
		//DS->data_centers->Find(dataCenterID)->getData()->getTrafficTree()->PrintInOrder();
		*traffic = DS->data_centers->Find(dataCenterID)->
			getData()->getTrafficTree()->GetKHighestSum(k);
	}
	return SUCCESS_DSM;

}
