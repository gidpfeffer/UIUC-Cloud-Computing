/**********************************
 * FILE NAME: MP2Node.cpp
 *
 * DESCRIPTION: MP2Node class definition
 **********************************/
#include "MP2Node.h"
#include <algorithm>

/**
 * constructor
 */
MP2Node::MP2Node(Member *memberNode, Params *par, EmulNet * emulNet, Log * log, Address * address) {
	this->memberNode = memberNode;
	this->par = par;
	this->emulNet = emulNet;
	this->log = log;
	ht = new HashTable();
	this->memberNode->addr = *address;
}

/**
 * Destructor
 */
MP2Node::~MP2Node() {
	delete ht;
	delete memberNode;
}

/**
 * FUNCTION NAME: updateRing
 *
 * DESCRIPTION: This function does the following:
 * 				1) Gets the current membership list from the Membership Protocol (MP1Node)
 * 				   The membership list is returned as a vector of Nodes. See Node class in Node.h
 * 				2) Constructs the ring based on the membership list
 * 				3) Calls the Stabilization Protocol
 */
void MP2Node::updateRing() {
	/*
	 * Implement this. Parts of it are already implemented
	 */
	vector<Node> curMemList;
    
	/*
	 *  Step 1. Get the current membership list from Membership Protocol / MP1
	 */
	curMemList = getMembershipList();

	/*
	 * Step 2: Construct the ring
	 */
	// Sort the list based on the hashCode
	sort(curMemList.begin(), curMemList.end());
    ring = curMemList;

	/*
	 * Step 3: Run the stabilization protocol IF REQUIRED
	 */
    // Run stabilization protocol if the hash table size is greater than zero and if there has been a changed in the ring
    stabilizationProtocol();
}

/**
 * FUNCTION NAME: getMemberhipList
 *
 * DESCRIPTION: This function goes through the membership list from the Membership protocol/MP1 and
 * 				i) generates the hash code for each member
 * 				ii) populates the ring member in MP2Node class
 * 				It returns a vector of Nodes. Each element in the vector contain the following fields:
 * 				a) Address of the node
 * 				b) Hash code obtained by consistent hashing of the Address
 */
vector<Node> MP2Node::getMembershipList() {
	unsigned int i;
	vector<Node> curMemList;
	for ( i = 0 ; i < this->memberNode->memberList.size(); i++ ) {
		Address addressOfThisMember;
		int id = this->memberNode->memberList.at(i).getid();
		short port = this->memberNode->memberList.at(i).getport();
		memcpy(&addressOfThisMember.addr[0], &id, sizeof(int));
		memcpy(&addressOfThisMember.addr[4], &port, sizeof(short));
		curMemList.emplace_back(Node(addressOfThisMember));
	}
	return curMemList;
}

/**
 * FUNCTION NAME: hashFunction
 *
 * DESCRIPTION: This functions hashes the key and returns the position on the ring
 * 				HASH FUNCTION USED FOR CONSISTENT HASHING
 *
 * RETURNS:
 * size_t position on the ring
 */
size_t MP2Node::hashFunction(string key) {
	std::hash<string> hashFunc;
	size_t ret = hashFunc(key);
	return ret%RING_SIZE;
}

/**
 * FUNCTION NAME: clientCreate
 *
 * DESCRIPTION: client side CREATE API
 * 				The function does the following:
 * 				1) Constructs the message
 * 				2) Finds the replicas of this key
 * 				3) Sends a message to the replica
 */
void MP2Node::clientCreate(string key, string value) {
	/*
	 * Implement this
	 */
    processClient(CREATE, key, value, g_transID++);
}

/**
 * FUNCTION NAME: clientRead
 *
 * DESCRIPTION: client side READ API
 * 				The function does the following:
 * 				1) Constructs the message
 * 				2) Finds the replicas of this key
 * 				3) Sends a message to the replica
 */
void MP2Node::clientRead(string key){
	/*
	 * Implement this
	 */
    processClient(READ, key, "", g_transID++);
}

/**
 * FUNCTION NAME: clientUpdate
 *
 * DESCRIPTION: client side UPDATE API
 * 				The function does the following:
 * 				1) Constructs the message
 * 				2) Finds the replicas of this key
 * 				3) Sends a message to the replica
 */
void MP2Node::clientUpdate(string key, string value){
	/*
	 * Implement this
	 */
    processClient(UPDATE, key, value, g_transID++);
}

/**
 * FUNCTION NAME: clientDelete
 *
 * DESCRIPTION: client side DELETE API
 * 				The function does the following:
 * 				1) Constructs the message
 * 				2) Finds the replicas of this key
 * 				3) Sends a message to the replica
 */
void MP2Node::clientDelete(string key){
	/*
	 * Implement this
	 */
    processClient(DELETE, key, "", g_transID++);
}

/**
 * FUNCTION NAME: createKeyValue
 *
 * DESCRIPTION: Server side CREATE API
 * 			   	The function does the following:
 * 			   	1) Inserts key value into the local hash table
 * 			   	2) Return true or false based on success or failure
 */
bool MP2Node::createKeyValue(string key, string value, ReplicaType replica) {
	/*
	 * Implement this
	 */
	// Insert key, value, replicaType into the hash table
    
    return ht->create(key, value);
}

/**
 * FUNCTION NAME: readKey
 *
 * DESCRIPTION: Server side READ API
 * 			    This function does the following:
 * 			    1) Read key from local hash table
 * 			    2) Return value
 */
string MP2Node::readKey(string key) {
	/*
	 * Implement this
	 */
	// Read key from local hash table and return value
    
    return ht->read(key);
}

/**
 * FUNCTION NAME: updateKeyValue
 *
 * DESCRIPTION: Server side UPDATE API
 * 				This function does the following:
 * 				1) Update the key to the new value in the local hash table
 * 				2) Return true or false based on success or failure
 */
bool MP2Node::updateKeyValue(string key, string value, ReplicaType replica) {
	/*
	 * Implement this
	 */
	// Update key in local hash table and return true or false
    
    return ht->update(key, value);
}

/**
 * FUNCTION NAME: deleteKey
 *
 * DESCRIPTION: Server side DELETE API
 * 				This function does the following:
 * 				1) Delete the key from the local hash table
 * 				2) Return true or false based on success or failure
 */
bool MP2Node::deletekey(string key) {
	/*
	 * Implement this
	 */
	// Delete the key from the local hash table
    
    return ht->deleteKey(key);
}

/**
 * FUNCTION NAME: checkMessages
 *
 * DESCRIPTION: This function is the message handler of this node.
 * 				This function does the following:
 * 				1) Pops messages from the queue
 * 				2) Handles the messages according to message types
 */
void MP2Node::checkMessages() {
	/*
	 * Implement this. Parts of it are already implemented
	 */
	char * data;
	int size;

	/*
	 * Declare your local variables here
	 */

	// dequeue all messages and handle them
	while ( !memberNode->mp2q.empty() ) {
		/*
		 * Pop a message from the queue
		 */
		data = (char *)memberNode->mp2q.front().elt;
		size = memberNode->mp2q.front().size;
		memberNode->mp2q.pop();

		string message(data, data + size);

		/*
		 * Handle the message types here
		 */

        Message *msg = new Message(string(data, data + size));
        handleMessage(msg);
	}

	/*
	 * This function should also ensure all READ and UPDATE operation
	 * get QUORUM replies
	 */
    for (auto it = expecting.cbegin(); it != expecting.cend();)
    {
        if (finalize(it->first))
        {
            wrapper* w = it->second;
            expecting.erase(it++);
            free(w);
        }
        else
        {
            ++it;
        }
    }
}

/**
 * FUNCTION NAME: findNodes
 *
 * DESCRIPTION: Find the replicas of the given keyfunction
 * 				This function is responsible for finding the replicas of a key
 */
vector<Node> MP2Node::findNodes(string key) {
	size_t pos = hashFunction(key);
	vector<Node> addr_vec;
	if (ring.size() >= 3) {
		// if pos <= min || pos > max, the leader is the min
		if (pos <= ring.at(0).getHashCode() || pos > ring.at(ring.size()-1).getHashCode()) {
			addr_vec.emplace_back(ring.at(0));
			addr_vec.emplace_back(ring.at(1));
			addr_vec.emplace_back(ring.at(2));
		}
		else {
			// go through the ring until pos <= node
			for (int i=1; i<ring.size(); i++){
				Node addr = ring.at(i);
				if (pos <= addr.getHashCode()) {
					addr_vec.emplace_back(addr);
					addr_vec.emplace_back(ring.at((i+1)%ring.size()));
					addr_vec.emplace_back(ring.at((i+2)%ring.size()));
					break;
				}
			}
		}
	}
	return addr_vec;
}

/**
 * FUNCTION NAME: recvLoop
 *
 * DESCRIPTION: Receive messages from EmulNet and push into the queue (mp2q)
 */
bool MP2Node::recvLoop() {
    if ( memberNode->bFailed ) {
    	return false;
    }
    else {
    	return emulNet->ENrecv(&(memberNode->addr), this->enqueueWrapper, NULL, 1, &(memberNode->mp2q));
    }
}

/**
 * FUNCTION NAME: enqueueWrapper
 *
 * DESCRIPTION: Enqueue the message from Emulnet into the queue of MP2Node
 */
int MP2Node::enqueueWrapper(void *env, char *buff, int size) {
	Queue q;
	return q.enqueue((queue<q_elt> *)env, (void *)buff, size);
}
/**
 * FUNCTION NAME: stabilizationProtocol
 *
 * DESCRIPTION: This runs the stabilization protocol in case of Node joins and leaves
 * 				It ensures that there always 3 copies of all keys in the DHT at all times
 * 				The function does the following:
 *				1) Ensures that there are three "CORRECT" replicas of all the keys in spite of failures and joins
 *				Note:- "CORRECT" replicas implies that every key is replicated in its two neighboring nodes in the ring
 */
void MP2Node::stabilizationProtocol() {
	/*
	 * Implement this
	 */
    
}

void MP2Node::handleMessage(Message *msg){
    /*
     * Implement responses to messages
     */
    
    Message* res;
    switch (msg->type){
        case CREATE:
            res = new Message(msg->transID, memberNode->addr, REPLY, createKeyValue(msg->key, msg->value, msg->replica));
            emulNet->ENsend(&memberNode->addr, &msg->fromAddr, res->toString());
            delete res;
            break;
        case READ:
            res = new Message(msg->transID, memberNode->addr, readKey(msg->key));
            emulNet->ENsend(&memberNode->addr, &msg->fromAddr, res->toString());
            delete res;
            break;
        case UPDATE:
            res = new Message(msg->transID, memberNode->addr, REPLY, updateKeyValue(msg->key, msg->value, msg->replica));
            emulNet->ENsend(&memberNode->addr, &msg->fromAddr, res->toString());
            delete res;
            break;
        case DELETE:
            res = new Message(msg->transID, memberNode->addr, REPLY, deletekey(msg->key));
            emulNet->ENsend(&memberNode->addr, &msg->fromAddr, res->toString());
            delete res;
            break;
        case REPLY:
            updateExpecting(msg);
            break;
        case READREPLY:
            updateExpecting(msg);
            break;
    }
    
}

wrapper* MP2Node::initWrapper(MessageType type, string key, string val){
    wrapper* w = (wrapper*) malloc(sizeof(wrapper));
    w->responses = 0;
    w->successes = 0;
    w->key = new string(key);
    w->val = new string(val);
    w->time = par->getcurrtime();
    w->type = type;
    return w;
}

void MP2Node::processClient(MessageType type, string key, string val, int process_id) {
    Message* msg;
    vector<Node> replicas = findNodes(key);
    
    if(replicas.size() == 3){
        expecting[process_id] = initWrapper(type, key, val);
        
        msg = new Message(process_id, memberNode->addr, type, key, val, PRIMARY);
        emulNet->ENsend(&memberNode->addr, &replicas[0].nodeAddress, msg->toString());
        delete(msg);
        
        msg = new Message(process_id, memberNode->addr, type, key, val, SECONDARY);
        emulNet->ENsend(&memberNode->addr, &replicas[1].nodeAddress, msg->toString());
        delete(msg);
        
        msg = new Message(process_id, memberNode->addr, type, key, val, TERTIARY);
        emulNet->ENsend(&memberNode->addr, &replicas[2].nodeAddress, msg->toString());
        delete(msg);
    }
}

void MP2Node::updateExpecting(Message* msg){
    int id = msg->transID;
    if(expecting.find(id) != expecting.end()){
        expecting[id]->responses++;
        if(msg->success) { expecting[id]->successes++; }
        else{
            if(msg->value != ""){
                expecting[id]->successes++;
                expecting[id]->val=new string(msg->value);
            }
        }
        if(finalize(id)){
            wrapper* w = expecting[id];
            expecting.erase(id);
            free(w);
        }
    }
}

bool MP2Node::finalize(int id){
    if(expecting.find(id) != expecting.end()){
        if(expecting[id]->successes >= 2){
            switch (expecting[id]->type) {
                case CREATE:
                    log->logCreateSuccess(&memberNode->addr, true, id, *(expecting[id]->key), *(expecting[id]->val));
                    break;
                case READ:
                    log->logReadSuccess(&memberNode->addr, true, id, *(expecting[id]->key), *(expecting[id]->val));
                    break;
                case UPDATE:
                    log->logUpdateSuccess(&memberNode->addr, true, id, *(expecting[id]->key), *(expecting[id]->val));
                    break;
                case DELETE:
                    log->logDeleteSuccess(&memberNode->addr, true, id, *(expecting[id]->key));
                    break;
                default:
                    break;
            }
            return true;
            
        } else if(expecting[id]->responses >= 3 || par->getcurrtime() - expecting[id]->time > TIMEOUT) {
            printf("timeout with %d responses\n", expecting[id]->responses);
            switch (expecting[id]->type) {
                case CREATE:
                    log->logCreateFail(&memberNode->addr, true, id, *(expecting[id]->key),  *(expecting[id]->val));
                    break;
                case READ:
                    log->logReadFail(&memberNode->addr, true, id, *(expecting[id]->key));
                    break;
                case UPDATE:
                    log->logUpdateFail(&memberNode->addr, true, id, *(expecting[id]->key),  *(expecting[id]->val));
                    break;
                case DELETE:
                    log->logDeleteFail(&memberNode->addr, true, id, *(expecting[id]->key));
                    break;
                default:
                    break;
            }
            return true;
        }
    }
    return false;
}
