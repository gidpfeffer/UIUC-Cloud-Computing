# Week 4

## Key Value Stores

### NoSQL

Stands for not only sequel. Generally stored by column not by row. Supports gets and puts (potentially some additional other calls). Generally doesn't contain foreign keys. Scales horizontally where as relational databases scale vertically.

### Cassandra

A distributed key value storage system maintained by Apache. 

#### Determination of Key Storage Node

Uses a circular distributed hash table like the one seen for chord. There is no routing (finger) table in Cassandra like there was in Chord. This means that every server needs to know information about all the other servers in the ring. Partition strategies may differ from that of Chord though. Servers can be referenced like Chord, using hash functions (called RandomPartitioner). They can also be partioned using ByteOrdering which makes range queries easier (ex. ordering based on timestamp of tweets). These are SimpleStrategies. For NetworkTopologyStrategies, you can assign multiple keys per data center.

Hinted Handoff writes locally for a down server until it comes back up. After it comes back up it writes the updated data there.

#### Snitching

Snitches map ip adresses to racks and DCs. There are a variety of inferring techniques and can be used/configured as needed.

#### Writing

Writes to files when memory capacity is met. Writes base don sorted string. Also uses an index table to quickly find elements without having to parse through the whole data file. Uses a Bloom filter to determine whether or not an item is present in the database. Also merges SSTables after a certain amount of time and deletes the unmerged files. Called compaction.

### X-Cap Theorem

In a distributed system, there are 3 topics considered to be very important.

1. **Consistency**: All nodes see the same data at the same time and reads see most recently written data.
2. **Availability**: The system allows operations all the time and the operations return quicky.
3. **Parition-tolerance**: The system continues to work in spite of network partitions.

The theorem states that you can satisfy at most, 2 of theabove properties. Never all three. Partition tolerance is very important, so we generally denote that we have to choose between consistency and availability.

Cassandra prefers availability over consistency. Traditional relational database models generally choose consistency over availability.

Relational databases provide **ACID**

**A**tomicity
**C**onsistency
**I**solation
**D**urability

Key value stores like Cassandra provide **BASE**

**B**asically
**A**vailable
**S**oft-state
**E**ventual

The value of X is known as the consistency level in Cassandra. Consistency levels incluse any (any server can write, may not be a replica), one (at least one replica), all (all replicas), quorum (quorum across all replicas in all datacenters). 

### The Consistency Spectrum

Cassandra supports eventual consistency. This means that if all reads and writes stopped, the values woudle eventually converge.

**Per key sequential**: ensures that per key, all operations have a global order. 

**CRDTs (Commutative data type)**: data structure for which commutative writes give the same result. For example, an incrementor is not order specific.

**Red-blue consistency**: rewrites ops into red and blue ops. Blue are communtative. Red are order specific.

**Causal Consistency**: Reads respect partial order based on information flow.

#### Strong COnsistency Models

**Linearizability**: Each operation by a client is instantaneously available to all other clients. Very difficult to support in computing clusters.

**Sequential Consistency**: Finds a reasonable ordering which obeys consistency across all clients.

The stronger the consistency, the slower the reads and writes.

### HBase

Google's big table was the first "blob based" storage system. They released a paper on this and Yahoo released the first open source implementation of this called HBase.

Supports get, put, multiput, and scan in a particular range. Unlike Cassandra, prefers availability over consistency under partitions.

Split into multiple regions, replicated across servers. Column families are a subset of columns with similar query patterns. There is one store per combination of store + region. Each store contains a memstore. The memstore maintains an in memory version of the latest updates. When the memstore is full, it is flushed to disk. There are store files for each store which is where the data lives on disk.

A slave master protocol is used to produce replicas. A zookeeper is used like a file system to store control information.

## Time and Ordering

Asynchronous distributed system models assumes that computers are using different system clocks, the message delays are unpredictable, and the time to process a message is unpredicatble.

Clock skew refers to the difference in the current time of the process clocks.

Clock drift refers to the relative difference in clock movement speed.

Internal Sychronization synchronizes in a cluster.

External synchronizes with respect to an external time source. External implis internal to an extent, but not the other way around.

### Cristian's Algorithm

min1 indicates the min time to send from machine 1 to machine 2. min2 is the opposite. We send a request for the time from machine 1 to machine 2, keepting track of the round trip time (RTT).

The actual time when machine 1 recieves the message is bounded by [t + min2, t + RTT - min1].

Cristian's algorithm sets its clock to be 

clock = t + (min2 + RTT - min1)/2

Error is bounded by 

(RTT - min1 - min2)/2

### NTP

Servers are organized in a tree

  Primary Server
  |          |
Secondary Servers
  |          |
 Tertiary Server
 
Child sends protocol to parent requesting time. Two time sent and time recieved values are then derived from 2 messages. They are then used to update the clocks. The error is bounded by half of the round trip time.

### Lamport Timestamps

One of the most important building blocks in distributed systems. Assigning timestamps to events need to ensure causality. Caulsality means that if an event A happend before event B, the timestamp associated with A is earlier than the timestamp associated with B. IN other words, one event leads to another (there is a path from one event to another event).

Logical or Lamport ordering define s a logical Happens-Before ordering among pairs of events. There are three rules that are used to determine whether or not two events are related by the Happens Before relation:

1. On the same process, a -> if time(a) < time(b) (using local clock)
2. If p1 sends m to p2: send(m) < recieve(m)
3. Transitivity: if a < b and b < c then a < c (creates a partial ordering among events)

#### Assigning Timestamps

Each process uses a local counter (clock) which is an integer. Value starts at 0

A process increments its counter when a send or instruction happens at it. The counter is assigned to the event at its timestamp.

A send event carries its timestamp.

For a recieve action, the counter is updated by max(local clock, message timestamp) + 1.

Events with no causal path are knows as **concurrent events**.

if e1 -> e2 then time(e1) < time(e2)

if time(e1) < time(e2) either e1 -> e2 or they are concurrent process.

### Vector Clocks

Vector clocks work silimar to lamport clocks except a vector of clocks for every process is kept.

Using Vi to denote the vector clock on the ith machine.

Vi[i] gets incremented whenever a task is perfomed on machine i.
Vi[j] get set to the max(incoming message Vi[j], local Vi[j]) for all incoming messages.

Note, recieving a message is a performed task (local counter is incremented).

Where as concurrent events could not be identified using Lamport timestamps, they can be using vector clocks. See slides for formulas used to determine causality and concurrency.
