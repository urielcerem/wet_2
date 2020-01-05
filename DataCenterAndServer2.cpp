#include "Array.h"
#include "list.h"
#include "AVL.h"
#include "DataCenterAndServer2.h"


/**---Server Functions---**/
int &Server::ID() {
    return id;
}

int &Server::Traffic() {
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

Server *DataCenter::getTrafficTree() {
    return reinterpret_cast<Server *>(&trafficTree.root);
}
