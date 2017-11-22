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
