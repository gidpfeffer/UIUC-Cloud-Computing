# Week 3 Notes

## Peer 2 Peer Systems

### Napster

Files are stored locally, not remotely. The remote servers hold file and peer pointers. When a client searches, a message goes out to the server with the keywords in them. The group of servers talk to each other, search their databases for matches, and returns them to the client. When a download starts, TCP sockets are used to download the file from the remote user to the local user. DNS routing delivers the ip of the server for the peer to connect to when starting up.

**Problems**: Centralized servers lead to a single point of failure. Overloaded servers could become very slow. 

### Gnutella

Takes out the middle man server and uses clients to search and recieve. Peers store their own files and communicate with one another. Peers store pointers to other peers (like neighbors).

#### 5 Main Messages

* Query (search)

* QueryHit (response to query)

* Ping (probe network for peers)

* Pong (respond to ping with adress of another peer)

* Push (initiates file transfer)

#### Header

[ descriptor ID : payload descriptor : decremented at each step (time to live, done when 0) : hops : payload length ] : payload

Return messages are in the form of QueryHits. They contain the original descriptor ID so that they can be reverse routed. The requestor sends an HTTP get request to start the download. 

Gnutella tries to set up an HTTP connection from sender to reciever. If it fails, it assumes there is a firewall on the recieving end. It then sets up a TCP push to be reverse routed through the network. The reciever can then download off that connection.

Uses ping pong to continually refresh neighbor list.

### FastTrack and BitTorrent

FastTrack is proprietary, though we know some of the details. Similar to Gnutella, though there is a subset of nodes called supernodes. Peers are selected to be supernodes based on participation level. Nodes now search supernodes first. This reduces probability of having to continue the search.

BitTorrent incentivizes files uploads. "Chokes" neighbors to limit the number of concurrent uploads.

### Chord

One of the first peer 2 peer systems to come out of academia. 

#### Distributed Hash Table

Stores objects at nodes or machines. Some concerns are load-balancing, fault tolerance, efficiency, and locality (want messages to be transmitted among nodes that are close by in terms of topology).

#### Runtimes

|          | Memory             | Lookup Latency | Number of Messages for Lookup |
|----------|--------------------|----------------|-------------------------------|
| Napster  | O(1) O(N) @ server | O(1)           | O(1)                          |
| Gnutella | O(N)               | O(N)           | O(N)                          |
| Chord    | O(log(N))          | O(log(N))      | O(log(N))                     |

#### Neighbor Selection

Uses consistent hashing to SHA-1 the ip adress and port. Make a clockwise pointer pattern using these sha's to the power of 2^m where m is some integer. Finger tables are used for each machine to keep track of the logarithmic pointers in the table.

File locations are based on hashing the name of the files and figuring out the first node to the clockwise of the location.

#### Handling Failures in Chord

One solution, nodes maintain up to r successor entries instead of just one. With high probability of success, r needs to be 2 * log(N). In order to protect from when the peer storing the file fails, store duplicate files in other peers.

#### Dealing with Dynamic Changes

##### Churn 

is defined as a a node joining, leaving, or failing inside the network within a specific period of time (an hour for example). Some services may have 100% churn in an hour.

##### Joining

A new peer joins by contacting a well known introducer through DNS. The introdcuer gives the requesting server a number which it will use to join the network and predecessor as well as sucessor nodes in the system. The joined server initializes its finger tables entries based on its sucessor's gfinger table. Over time, the server periodically talks to neighbors to update its own finger tables.

If the rate of stabilization is fast enough, it can keep up with churn in the system.

A new entry affects O(logn) by symettry on average. Therefore, the number of messages per new peer join is on averag O(log n x log n)

##### Leaving

Leaving protocol is similar to failing. You also need a failure detector. Mechanisms for such have been previously discussed.

##### Stabilization

Updating of peers ensures non-looping lookups. Each stabilization round requires a constant number of messages. Strong stabilization takes O(n^2) rounds to ensure synchronized tables.

### Pastry

Similar to Chord, assigns id's to nodes based on consistent hashing. Neighbors are handled in a slightly different way. A left set is maintained by every node. This contains pointers to predecessors and successors. Routing tables are based on prefix matching. Prefixes are in binary. 

Uses a stabilization protocol with the same objective as Chord.

### Kelips

Brings constant lookup cost to a distributed hash cluster. Uses k "affinity groups" where k ~ (N)^0.5. There are virtual groups. Each node is hashed to a group. A peer knows about all other peers in its affinity group. Each node also maintains a pointer to one node in the k - 1 other affintity groups. Files get stored at the node for the user that uploaded them. The servers amintain metadata from the uploads. Nodes in the same affinity group maintain metadata for all other uploads in its affinity group.

Gossip based protocol is used to update membership lists. The time to disseminate information is O(log n). Meta information is also updated using gossip style heartbeats from the source.
