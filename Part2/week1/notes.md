# Week 1

## Leader Election

### Problem

A leader may be needed to control replicas across multiple servers for example. Problems may arise when a leader fails, servers disagree about who the leader is, etc.

When leaders are elected, every server that is part of the group needs to be informed about this decision.

Any process may be able to detect that the leader has failed and therefor any server should be able to call for a leader election. Multiple processes can call for a new election, but only one leader should be elected at a time.

New leaders are generally picked base don an attribute (ip adress, available memory, etc). Elections should ensure safety and liveness.

Safety: The new leader elected is not faulty.
Liveness: The new leader is selected for every run (the election process terminates).

### Ring Leader Election

Messages circle around, update the message with the best candidate. Once the best candidate recieves its own message, it send out an election message. All servers set their elected status, and forward. The new elected server terminates the forwarding once it recieves a duplicate of its sent message. O(n)

Consensus can be reduced to leader election. Therefore it is impossible to ensure liveness and safety in asynchronous systems.

### Zookeer and Google Chubby

Use a variation of Paxos. Wuorem based leader election guarentees safety, but not liveness.

### Bully Leader Election

Every server maintains a list of all other servers. When a leader is determined to have failed, the next highest attribute leader becomes the new leader. The new bully sends a cordinator message to all other servers. Or an election message is sent from a lower server to the higher ones until a response is recieved.
