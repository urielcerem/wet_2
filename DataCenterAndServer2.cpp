#include "AVL.h"
#include "DataCenterAndServer2.h"


/**---Server Functions---**/
int &Server::ID() {
    return id;
}

int Server::Traffic() {
    return traffic;
}

int &Server::BelongsToDataCenter() {
    return DCParentId;
}

StatusTypeDC Server::updateTraffic(int new_traffic) {
    if (traffic < 0)
        return INVALID_INPUT_DC;
    this->traffic = new_traffic;
    return SUCCESS_DC;
}

StatusTypeDC Server::updateDataCenter(int Id) {
    if (Id < 0)
        return INVALID_INPUT_DC;
    this->id = Id;
    return SUCCESS_DC;
}


/**---DataCenter Functions---**/
int &DataCenter::NumOfServers() {
    return num_of_servers;
}

AVLTree <Server>*DataCenter::getTrafficTree() {
    return &trafficTree;
}

StatusTypeDC DataCenter::AddServer() {
    ++num_of_servers;
    return SUCCESS_DC;
}

StatusTypeDC DataCenter::RemoveServer() {
    --num_of_servers;
    return SUCCESS_DC;
}

ostream & operator << (ostream& out, const Server &s) {
	out << "  id = " << s.id << "    parentd id = " << s.DCParentId << "    traffic = " << s.traffic;
	return out;
}