/**********************************
 * FILE NAME: MP1Node.cpp
 *
 * DESCRIPTION: Membership protocol run by this Node.
 * 				Definition of MP1Node class functions.
 **********************************/

#include "MP1Node.h"

/*
 * Note: You can change/add any functions in MP1Node.{h,cpp}
 */

/**
 * Overloaded Constructor of the MP1Node class
 * You can add new members to the class if you think it
 * is necessary for your logic to work
 */
MP1Node::MP1Node(Member *member, Params *params, EmulNet *emul, Log *log, Address *address) {
	for( int i = 0; i < 6; i++ ) {
		NULLADDR[i] = 0;
	}
	this->memberNode = member;
	this->emulNet = emul;
	this->log = log;
	this->par = params;
	this->memberNode->addr = *address;
}

/**
 * Destructor of the MP1Node class
 */
MP1Node::~MP1Node() {}

/**
 * FUNCTION NAME: recvLoop
 *
 * DESCRIPTION: This function receives message from the network and pushes into the queue
 * 				This function is called by a node to receive messages currently waiting for it
 */
int MP1Node::recvLoop() {
    if ( memberNode->bFailed ) {
    	return false;
    }
    else {
    	return emulNet->ENrecv(&(memberNode->addr), enqueueWrapper, NULL, 1, &(memberNode->mp1q));
    }
}

/**
 * FUNCTION NAME: enqueueWrapper
 *
 * DESCRIPTION: Enqueue the message from Emulnet into the queue
 */
int MP1Node::enqueueWrapper(void *env, char *buff, int size) {
	Queue q;
	return q.enqueue((queue<q_elt> *)env, (void *)buff, size);
}

/**
 * FUNCTION NAME: nodeStart
 *
 * DESCRIPTION: This function bootstraps the node
 * 				All initializations routines for a member.
 * 				Called by the application layer.
 */
void MP1Node::nodeStart(char *servaddrstr, short servport) {
    Address joinaddr;
    joinaddr = getJoinAddress();

    // Self booting routines
    if( initThisNode(&joinaddr) == -1 ) {
#ifdef DEBUGLOG
        log->LOG(&memberNode->addr, "init_thisnode failed. Exit.");
#endif
        exit(1);
    }

    if( !introduceSelfToGroup(&joinaddr) ) {
        finishUpThisNode();
#ifdef DEBUGLOG
        log->LOG(&memberNode->addr, "Unable to join self to group. Exiting.");
#endif
        exit(1);
    }

    return;
}

/**
 * FUNCTION NAME: initThisNode
 *
 * DESCRIPTION: Find out who I am and start up
 */
int MP1Node::initThisNode(Address *joinaddr) {
	/*
	 * This function is partially implemented and may require changes
	 */
	memberNode->bFailed = false;
	memberNode->inited = true;
	memberNode->inGroup = false;
    // node is up!
	memberNode->nnb = 0;
	memberNode->heartbeat = 0;
	memberNode->pingCounter = TFAIL;
	memberNode->timeOutCounter = -1;
    initMemberListTable(memberNode);

    return 0;
}

/**
 * FUNCTION NAME: introduceSelfToGroup
 *
 * DESCRIPTION: Join the distributed system
 */
int MP1Node::introduceSelfToGroup(Address *joinaddr) {
	MessageHdr *msg;
#ifdef DEBUGLOG
    static char s[1024];
#endif

    if ( 0 == memcmp((char *)&(memberNode->addr.addr), (char *)&(joinaddr->addr), sizeof(memberNode->addr.addr))) {
        // I am the group booter (first process to join the group). Boot up the group
#ifdef DEBUGLOG
        log->LOG(&memberNode->addr, "Starting up group...");
#endif
        memberNode->inGroup = true;
    }
    else {
        size_t msgsize = sizeof(MessageHdr) + sizeof(joinaddr->addr) + sizeof(long) + 1;
        msg = (MessageHdr *) malloc(msgsize * sizeof(char));

        // create JOINREQ message: format of data is {struct Address myaddr}
        msg->msgType = JOINREQ;
        memcpy((char *)(msg+1), &memberNode->addr.addr, sizeof(memberNode->addr.addr));
        memcpy((char *)(msg+1) + 1 + sizeof(memberNode->addr.addr), &memberNode->heartbeat, sizeof(long));

#ifdef DEBUGLOG
        sprintf(s, "Trying to join...");
        log->LOG(&memberNode->addr, s);
#endif

        // send JOINREQ message to introducer member
        emulNet->ENsend(&memberNode->addr, joinaddr, (char *)msg, msgsize);

        free(msg);
    }

    return 1;

}

/**
 * FUNCTION NAME: finishUpThisNode
 *
 * DESCRIPTION: Wind up this node and clean up state
 */
int MP1Node::finishUpThisNode(){
   /*
    * Your code goes here
    */
    return 0;
}

/**
 * FUNCTION NAME: nodeLoop
 *
 * DESCRIPTION: Executed periodically at each member
 * 				Check your messages in queue and perform membership protocol duties
 */
void MP1Node::nodeLoop() {
    if (memberNode->bFailed) {
    	return;
    }

    // Check my messages
    checkMessages();

    // Wait until you're in the group...
    if( !memberNode->inGroup ) {
    	return;
    }

    // ...then jump in and share your responsibilites!
    nodeLoopOps();

    return;
}

/**
 * FUNCTION NAME: checkMessages
 *
 * DESCRIPTION: Check messages in the queue and call the respective message handler
 */
void MP1Node::checkMessages() {
    void *ptr;
    int size;

    // Pop waiting messages from memberNode's mp1q
    while ( !memberNode->mp1q.empty() ) {
    	ptr = memberNode->mp1q.front().elt;
    	size = memberNode->mp1q.front().size;
    	memberNode->mp1q.pop();
    	recvCallBack((void *)memberNode, (char *)ptr, size);
    }
    return;
}

/**
 * FUNCTION NAME: recvCallBack
 *
 * DESCRIPTION: Message handler for different message types
 */
bool MP1Node::recvCallBack(void *env, char *data, int size) {
	/*
	 * Your code goes here
	 */
    MessageHdr *msg = (MessageHdr *) data;
    if (msg->msgType == JOINREQ) {
        recJoinReq(msg);
    }
    else if (msg->msgType == JOINREP) {
        recJoinRep(msg);
    }
    else if (msg->msgType == LISTREQ){
        recListReq(msg);
    }
    else if (msg->msgType == LISTREP){
        recListRep(msg, size);
    }
    return 0;
}

/**
 * FUNCTION NAME: nodeLoopOps
 *
 * DESCRIPTION: Check if any node hasn't responded within a timeout period and then delete
 * 				the nodes
 * 				Propagate your membership list
 */
void MP1Node::nodeLoopOps() {

	/*
	 * Your code goes here
	 */
    memberNode->heartbeat++;
    std::vector<MemberListEntry>::iterator myEntry = memberNode->memberList.begin();
    myEntry->heartbeat = memberNode->heartbeat;
    myEntry->timestamp = myEntry->heartbeat;

    requestMembership();

    return;
}

/**
 * FUNCTION NAME: isNullAddress
 *
 * DESCRIPTION: Function checks if the address is NULL
 */
int MP1Node::isNullAddress(Address *addr) {
	return (memcmp(addr->addr, NULLADDR, 6) == 0 ? 1 : 0);
}

/**
 * FUNCTION NAME: getJoinAddress
 *
 * DESCRIPTION: Returns the Address of the coordinator
 */
Address MP1Node::getJoinAddress() {
    Address joinaddr;

    memset(&joinaddr, 0, sizeof(Address));
    *(int *)(&joinaddr.addr) = 1;
    *(short *)(&joinaddr.addr[4]) = 0;

    return joinaddr;
}

/**
 * FUNCTION NAME: initMemberListTable
 *
 * DESCRIPTION: Initialize the membership list
 */
void MP1Node::initMemberListTable(Member *memberNode) {
	memberNode->memberList.clear();

    //added by student
    int id = *(int*)(&memberNode->addr.addr);
    short port = *(short*)(&memberNode->addr.addr[4]);
    MemberListEntry entry(id, port, memberNode->heartbeat, memberNode->heartbeat);
    memberNode->memberList.push_back(entry);
    memberNode->myPos = memberNode->memberList.begin();
}

/**
 * FUNCTION NAME: printAddress
 *
 * DESCRIPTION: Print the Address
 */
void MP1Node::printAddress(Address *addr)
{
    printf("%d.%d.%d.%d:%d \n",  addr->addr[0],addr->addr[1],addr->addr[2],
                                                       addr->addr[3], *(short*)&addr->addr[4]) ;    
}


void MP1Node::recJoinReq(MessageHdr *msg)
{
    //Adding new member to initiator list
    Address joinaddr;
    memcpy(&joinaddr.addr, (char *)(msg + 1), sizeof(joinaddr.addr));

    printf("recieved join req from ");
    printAddress(&joinaddr);

    long heartbeat;
    memcpy(&heartbeat, (char *)(msg+1) + 1 + sizeof(memberNode->addr.addr), sizeof(long));

    int id = *(int*)(&joinaddr.addr);
    short port = *(short*)(&joinaddr.addr[4]);

    put(id, port, heartbeat);

    // responding with initiator address and heartbeat
    MessageHdr *rep;
    size_t msgsize = sizeof(MessageHdr) + sizeof(joinaddr.addr) + sizeof(long) + 1;
    rep = (MessageHdr *) malloc(msgsize * sizeof(char));

    rep->msgType = JOINREP;
    memcpy((char *)(rep+1), &memberNode->addr.addr, sizeof(memberNode->addr.addr));
    memcpy((char *)(rep+1) + 1 + sizeof(memberNode->addr.addr), &memberNode->heartbeat, sizeof(long));

    emulNet->ENsend(&memberNode->addr, &joinaddr, (char *)rep, msgsize);
    free(rep);
}

void MP1Node::recJoinRep(MessageHdr *msg)
{
    Address joinaddr;
    memcpy(&joinaddr.addr, (char *)(msg + 1), sizeof(joinaddr.addr));

    long heartbeat;
    memcpy(&heartbeat, (char *)(msg+1) + 1 + sizeof(memberNode->addr.addr), sizeof(long));

    int id = *(int*)(&joinaddr.addr);
    short port = *(short*)(&joinaddr.addr[4]);

    put(id, port, heartbeat);

    memberNode->inGroup = true;
}

bool MP1Node::put(int id, short port, long heartbeat)
{
    long timestamp = memberNode->heartbeat;

    printf("put id: %d, port: %hd at ", id, port);
    printAddress(&memberNode->addr);

    for(std::vector<MemberListEntry>::iterator it = memberNode->memberList.begin(); it != memberNode->memberList.end(); ++it) {
        if(it->id == id && it->port == port){
            if(heartbeat > it->heartbeat){
                it->heartbeat = heartbeat;
                it->timestamp = timestamp;
            }
            return true;
        }
    }

    MemberListEntry memberEntry(id, port, heartbeat, timestamp);
    memberNode->memberList.push_back(memberEntry);
    return false;
}

void MP1Node::requestMembership()
{
    int neighbors = memberNode->memberList.size();
    if (neighbors == 2) {
        sendMemReq(&memberNode->memberList[1]);
    }
    else {
        int first = rand() % (neighbors - 1) + 1;
        int second = rand() % (neighbors - 2) + 1;
        if(first == second) first = neighbors - 1;
        sendMemReq(&memberNode->memberList[first]);
        sendMemReq(&memberNode->memberList[second]);
    }
}

void MP1Node::sendMemReq(MemberListEntry *entry)
{
    Address addr;
    memcpy(&addr.addr, &entry->id, sizeof(int));
    memcpy(&addr.addr[4], &entry->port, sizeof(short));

    MessageHdr *rep;
    size_t msgsize = sizeof(MessageHdr) + sizeof(memberNode->addr.addr) + sizeof(long) + 1;
    rep = (MessageHdr *) malloc(msgsize * sizeof(char));

    rep->msgType = LISTREQ;
    memcpy((char *)(rep+1), &memberNode->addr.addr, sizeof(memberNode->addr.addr));
    memcpy((char *)(rep+1) + 1 + sizeof(memberNode->addr.addr), &memberNode->heartbeat, sizeof(long));

    emulNet->ENsend(&memberNode->addr, &addr, (char *)rep, msgsize);
    free(rep);
}

void MP1Node::recListReq(MessageHdr *msg)
{
    Address reqaddr;
    memcpy(&reqaddr.addr, (char *)(msg + 1), sizeof(reqaddr.addr));

    long heartbeat;
    memcpy(&heartbeat, (char *)(msg+1) + 1 + sizeof(memberNode->addr.addr), sizeof(long));

    sendList(&reqaddr);
}

void MP1Node::sendList(Address *reqaddr)
{
    MessageHdr *rep;
    size_t msgsize = sizeof(reqaddr->addr) + sizeof(long) + 1;
    size_t size = msgsize * memberNode->memberList.size() + sizeof(MessageHdr);
    rep = (MessageHdr *) malloc(size * sizeof(char));

    rep->msgType = LISTREP;

    char *data = (char *)(rep+1);

    for(std::vector<MemberListEntry>::iterator it = memberNode->memberList.begin(); it != memberNode->memberList.end(); ++it) {
        Address addr;
        memcpy(&addr.addr, &it->id, sizeof(int));
        memcpy(&addr.addr[4], &it->port, sizeof(short));

        memcpy(data, &addr.addr, sizeof(addr.addr));
        memcpy((char *)data + 1 + sizeof(addr.addr), &it->heartbeat, sizeof(long));
        data += 1 + sizeof(addr.addr) + sizeof(long);
    }

    emulNet->ENsend(&memberNode->addr, reqaddr, (char *)rep, size);
    free(rep);
}

void MP1Node::recListRep(MessageHdr *msg, int size)
{
    size_t msgsize = sizeof(memberNode->addr.addr) + sizeof(long) + 1;
    int entries = (size - sizeof(MessageHdr)) / msgsize;
    char *data = (char *) msg;
    data = (char *)(msg+1);
    
    for(int i = 0; i < entries; i++){
        Address joinaddr;
        memcpy(&joinaddr.addr, data, sizeof(joinaddr.addr));

        long heartbeat;
        memcpy(&heartbeat, data + 1 + sizeof(memberNode->addr.addr), sizeof(long));

        int id = *(int*)(&joinaddr.addr);
        short port = *(short*)(&joinaddr.addr[4]);

        put(id, port, heartbeat);

        data += 1 + sizeof(memberNode->addr.addr) + sizeof(long);
    }
}
